#ifndef SCRIPTENVIRONMENT_HPP
#define SCRIPTENVIRONMENT_HPP

#include <QObject>
#include "logger_base.hpp"

class QIODevice;
class QScriptEngine;

class ScriptEnvironment : public QObject {
	Q_OBJECT

	logger local_logger;

	QIODevice* io;
	QScriptEngine* engine;

	QString unexecutedBuffer;

public:
	ScriptEnvironment(QObject* parent, const logger&, QIODevice*);
	~ScriptEnvironment();

private slots:
	void readyRead();
};

#endif // SCRIPTENVIRONMENT_HPP
