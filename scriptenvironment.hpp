#ifndef SCRIPTENVIRONMENT_HPP
#define SCRIPTENVIRONMENT_HPP

#include <QObject>
#include "iodevice_ostream.hpp"
#include "logger.hpp"

class log_target_base;
typedef boost::shared_ptr<log_target_base> log_target;

class log_target_container;
typedef boost::shared_ptr<log_target_container> log_target_container_ptr;

class QIODevice;
class QScriptEngine;

class ScriptEnvironment : public QObject {
	Q_OBJECT

	logger local_logger;
	log_target_container_ptr log_targets;

	QIODevice* io;

	IODevice_ostream io_device_ostream;
	log_target io_device_log_target;

	QScriptEngine* engine;
	QString unexecutedBuffer;

public:
	ScriptEnvironment(QObject* parent, const logger&, const log_target_container_ptr&, QObject* environment, QIODevice*);
	~ScriptEnvironment();

private slots:
	void readyRead();
};

#endif // SCRIPTENVIRONMENT_HPP
