#include <QIODevice>
#include <QScriptEngine>
#include <QString>
#include <QTextStream>
#include "scriptenvironment.hpp"
#include "qlog.hpp"

ScriptEnvironment::ScriptEnvironment(QObject* parent, const logger& local_logger_, QObject* environment, QIODevice* io_) :
	QObject(parent),
	local_logger(local_logger_),
	io(io_),
	engine(new QScriptEngine(this))
{
	QLOG(TRACE, "Constructed");

	QScriptValue applicationObject = engine->newQObject(environment);
	engine->globalObject().setProperty("application", applicationObject);

	assert(io);
	connect(io, SIGNAL(readyRead()), this, SLOT(readyRead()));

	QTextStream ts(io);
	ts << ">>> ";
}

ScriptEnvironment::~ScriptEnvironment() {
	QLOG(TRACE, "Destructed");
}

void ScriptEnvironment::readyRead() {
	QLOG(TRACE, __FUNCTION__);

	QTextStream ts(io);

	while (io->canReadLine()) {
		QByteArray line = io->readLine();
		QLOG(INPUT, "Read: \"" << QString(line).replace("\n", "\\n") << '"');
		unexecutedBuffer += QString::fromUtf8(line.constData(), line.size());

		if (engine->canEvaluate(unexecutedBuffer)) {
			QLOG(TRACE, "Evaluating: \"" << QString(unexecutedBuffer).replace("\n", "\\n") << '"');
			QScriptValue ret = engine->evaluate(unexecutedBuffer);
			QLOG(TRACE, "Evaluated to: \"" << QString(ret.toString()).replace("\n", "\\n") << '"');

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
