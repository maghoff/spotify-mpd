#include <QCoreApplication>
#include <QIODevice>
#include <QScriptEngine>
#include <QString>
#include <QTextStream>
#include <libspotify/api.h>
#include "scriptenvironment.hpp"
#include "spotify/spotifyartist.hpp"
#include "spotify/spotifyalbum.hpp"
#include "spotify/spotifylink.hpp"
#include "spotify/spotifytrack.hpp"
#include "qlog.hpp"

namespace {

// The caller must take ownership of the returned object
QObject* resolveLink(QString url) {
	Spotify::Link l(url);

	if (l.get() == 0) return 0; //< invalid link

	switch (sp_link_type(l.get())) {
	case SP_LINKTYPE_TRACK: return new Spotify::Track(l);
	case SP_LINKTYPE_ALBUM: return new Spotify::Album(l);
	case SP_LINKTYPE_ARTIST: return new Spotify::Artist(l);
	default: return 0;
	};
}

QScriptValue wrapResolveLink(QScriptContext* context, QScriptEngine* engine) {
	if (context->argumentCount() != 1) return QScriptValue();
	QObject* obj = resolveLink(context->argument(0).toString());
	return engine->newQObject(obj, QScriptEngine::ScriptOwnership);
}

void terminate() {
	QCoreApplication::exit(0);
}

QScriptValue wrapTerminate(QScriptContext* context, QScriptEngine*) {
	if (context->argumentCount() != 0) return QScriptValue();
	terminate();
	return QScriptValue();
}

#define QOBJECT_QSCRIPT_CONVERTERS(Type) \
	QScriptValue scriptValueFrom##Type(QScriptEngine *engine, Spotify::Type* const &in) { \
		return engine->newQObject(in); \
	} \
	\
	void scriptValueTo##Type(const QScriptValue &object, Spotify::Type* &out) { \
		out = qobject_cast<Spotify::Type*>(object.toQObject()); \
	}

QOBJECT_QSCRIPT_CONVERTERS(Album)
QOBJECT_QSCRIPT_CONVERTERS(Artist)
QOBJECT_QSCRIPT_CONVERTERS(Track)

}

ScriptEnvironment::ScriptEnvironment(QObject* parent, const logger& local_logger_, QObject* environment, QIODevice* io_) :
	QObject(parent),
	local_logger(local_logger_),
	io(io_),
	engine(new QScriptEngine(this))
{
	QLOG(TRACE, "Constructed");

	QScriptValue applicationObject = engine->newQObject(environment);
	engine->globalObject().setProperty("application", applicationObject);

	QScriptValue resolveLinkObject = engine->newFunction(wrapResolveLink, 1);
	engine->globalObject().setProperty("resolveLink", resolveLinkObject);

	QScriptValue terminateObject = engine->newFunction(wrapTerminate, 1);
	engine->globalObject().setProperty("terminate", terminateObject);

	#define REGISTER(Type) \
		qScriptRegisterMetaType(engine, &scriptValueFrom##Type, &scriptValueTo##Type);

	REGISTER(Album)
	REGISTER(Artist)
	REGISTER(Track)

	#undef REGISTER

	engine->evaluate(
		"app = application;\n"
		"p = app.spotify.player;\n"
		"t = terminate;\n"
	);

	assert(io);
	connect(io, SIGNAL(readyRead()), this, SLOT(readyRead()));

	QTextStream ts(io);
	ts << ">>> ";
}

ScriptEnvironment::~ScriptEnvironment() {
	QLOG(TRACE, "Destructed");
}

QString repr(const QString in) {
	QString result;
	QTextStream ts(&result, QIODevice::WriteOnly);

	ts << '"';

	QByteArray inUtf8 = in.toUtf8();
	foreach (char c, inUtf8) {
		if (c < 0x20 || c >= 0x7F) {
			if (c == '\t') ts << "\\t";
			else if (c == '\r') ts << "\\r";
			else if (c == '\n') ts << "\\n";
			else {
				ts << "\\x";
				ts.setPadChar('0');
				ts.setFieldWidth(2);
				ts << hex << (unsigned)(unsigned char)c;
				ts.setFieldWidth(0);
			}
		} else {
			if (c == '\\' || c == '"') ts << '\\';
			ts << c;
		}
	}

	ts << '"';

	ts.flush();
	return result;
}

void ScriptEnvironment::readyRead() {
	QLOG(TRACE, __FUNCTION__);

	QTextStream ts(io);

	while (io->canReadLine()) {
		QByteArray line = io->readLine();
		QLOG(INPUT, "Read: " << repr(line));
		unexecutedBuffer += QString::fromUtf8(line.constData(), line.size());

		if (engine->canEvaluate(unexecutedBuffer)) {
			QLOG(TRACE, "Evaluating: " << repr(unexecutedBuffer));
			QScriptValue ret = engine->evaluate(unexecutedBuffer);
			QLOG(TRACE, "Evaluated to: " << repr(ret.toString()));

			if (!ret.isUndefined()) {
				ts << ret.toString() << '\n';
			}

			unexecutedBuffer.clear();
		}

		if (unexecutedBuffer.size() == 0) {
			ts << ">>> ";
		} else {
			ts << "... ";
		}
	}
}
