#ifndef APPLICATION_HPP
#define APPLICATION_HPP

#include <QObject>
#include <QTcpServer>

class QIODevice;

class application : public QObject {
	Q_OBJECT

	QTcpServer server;
	QIODevice* io;

public:
	application();
	~application();

private slots:
	void newConnection();
	void readMe();
};

#endif // APPLICATION_HPP
