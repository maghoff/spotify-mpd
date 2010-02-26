#ifndef APPLICATION_HPP
#define APPLICATION_HPP

#include <QObject>
#include <QTcpServer>

class application : public QObject {
	Q_OBJECT

	QTcpServer server;

public:
	application();
	~application();

private slots:
	void newConnection();
};

#endif // APPLICATION_HPP
