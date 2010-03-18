#ifndef SCRIPTLISTENER_HPP
#define SCRIPTLISTENER_HPP

#include <QObject>
#include "logger_base.hpp"

class QTcpServer;

class ScriptListener : public QObject {
	Q_OBJECT

	logger local_logger;

	QTcpServer* server;

public:
	ScriptListener(QObject* parent, const logger& local_logger_);
	~ScriptListener();

private slots:
	void newConnection();
};
#endif // SCRIPTLISTENER_HPP
