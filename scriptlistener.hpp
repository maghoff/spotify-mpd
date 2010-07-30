#ifndef SCRIPTLISTENER_HPP
#define SCRIPTLISTENER_HPP

#include <QObject>
#include "logger.hpp"

class log_target_container;
typedef boost::shared_ptr<log_target_container> log_target_container_ptr;

class QTcpServer;

class ScriptListener : public QObject {
	Q_OBJECT

	logger local_logger;
	log_target_container_ptr log_targets;

	QObject* environment;

	QTcpServer* server;

public:
	ScriptListener(QObject* parent, const logger& local_logger_, const log_target_container_ptr&, QObject* environment);
	~ScriptListener();

private slots:
	void newConnection();
};

#endif // SCRIPTLISTENER_HPP
