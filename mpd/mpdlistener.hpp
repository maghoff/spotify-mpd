#ifndef MPDLISTENER_HPP
#define MPDLISTENER_HPP

#include <QObject>
#include <QTcpServer>
#include "logger.hpp"

class MPDListener : public QObject {
	Q_OBJECT

	logger local_logger;

	QTcpServer server;

public:
	MPDListener(QObject* parent, const logger& local_logger_);
	~MPDListener();

private slots:
	void newConnection();
};

#endif // MPDLISTENER_HPP
