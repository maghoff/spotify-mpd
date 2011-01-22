#include <stdexcept>
#include <QCoreApplication>
#include <QFile>
#include <QIODevice>
#include <QScriptEngine>
#include <QString>
#include <QTextStream>
#include <libspotify/api.h>
#include "scriptenvironment.hpp"
#include "spotify/artist.hpp"
#include "spotify/album.hpp"
#include "spotify/playlist.hpp"
#include "spotify/playlistcontainer.hpp"
#include "spotify/link.hpp"
#include "spotify/track.hpp"
#include "qlog.hpp"
#include "log_target_container.hpp"
#include "ansi_log_target.hpp"

namespace {

// The caller must take ownership of the returned object
QObject* resolveLink(QString url) {
	Spotify::Link l(url);

	if (l.get() == 0) return 0; //< invalid link

	switch (sp_link_type(l.get())) {
	case SP_LINKTYPE_TRACK: return new Spotify::Track(l);
	case SP_LINKTYPE_ALBUM: return new Spotify::Album(l);
	case SP_LINKTYPE_ARTIST: return new Spotify::Artist(l);
	default: return new Spotify::Link(l);
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

template<class T>
QScriptValue scriptValueFromType(QScriptEngine* engine, T* const& in) {
	return engine->newQObject(in);
}

template<class T>
void scriptValueToType(const QScriptValue& object, T* &out) {
	out = qobject_cast<T*>(object.toQObject());
}

QString readEntireFile(QString filename) {
	QFile in(filename);
	in.open(QIODevice::ReadOnly);
	size_t size = in.size();
	uchar* data = in.map(0, size);
	if (!data) throw std::runtime_error("QFile::map() returned 0");
	return QString::fromUtf8(reinterpret_cast<char*>(data), size);
}

}

ScriptEnvironment::ScriptEnvironment(
	QObject* parent,
	const logger& local_logger_,
	const log_target_container_ptr& log_targets_,
	QObject* environment,
	QIODevice* io_
) :
	QObject(parent),
	local_logger(local_logger_),
	log_targets(log_targets_),
	io(io_),
	io_device_ostream(io_),
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
		qScriptRegisterMetaType(engine, &scriptValueFromType<Spotify::Type>, &scriptValueToType<Spotify::Type>);

	REGISTER(Album)
	REGISTER(Artist)
	REGISTER(Playlist)
	REGISTER(PlaylistContainer)
	REGISTER(Track)
	REGISTER(Link)

	#undef REGISTER

	engine->evaluate(readEntireFile("init.js"));

	assert(io);
	connect(io, SIGNAL(readyRead()), this, SLOT(readyRead()));

	io_device_log_target.reset(new ansi_log_target(io_device_ostream, log_level::INPUT));
	log_targets->insert_target(io_device_log_target); //< Refactor to RAII?

	QTextStream ts(io);
	ts << ">>> ";
}

ScriptEnvironment::~ScriptEnvironment() {
	log_targets->erase_target(io_device_log_target); //< Refactor to RAII?

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
