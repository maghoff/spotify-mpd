#ifndef SCRIPTLISTENER_HPP
#define SCRIPTLISTENER_HPP

#include <QObject>
#include "logger_base.hpp"

class QTcpServer;

class ScriptListener : public QObject {
	Q_OBJECT

	logger local_logger;

	QObject* environment;

	QTcpServer* server;

public:
	ScriptListener(QObject* parent, const logger& local_logger_, QObject* environment);
	~ScriptListener();

private slots:
	void newConnection();
};
#endif // SCRIPTLISTENER_HPP
