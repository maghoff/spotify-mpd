#include <QTcpServer>
#include <QTcpSocket>
#include <QIODevice>
#include "scriptenvironment.hpp"
#include "scriptlistener.hpp"
#include "log.hpp"

namespace {

std::string utf8_str(QString s) {
	QByteArray a = s.toUtf8();
	return std::string(a.data(), a.data() + a.size());
}

}

ScriptListener::ScriptListener(QObject* parent, const logger& local_logger_, QObject* environment_) :
	QObject(parent),
	local_logger(local_logger_),
	environment(environment_)
{
	server = new QTcpServer(this);
	server->listen(QHostAddress::Any, 6602);
	connect(server, SIGNAL(newConnection()), this, SLOT(newConnection()));
}

ScriptListener::~ScriptListener() {
}

void ScriptListener::newConnection() {
	LLOG(OPERATION, "New connection");

	QTcpSocket* io = server->nextPendingConnection();

	std::stringstream ss;
	ss << utf8_str(io->peerAddress().toString()) << ':' << io->peerPort();

	new ScriptEnvironment(this, local_logger->create_sublogger(ss.str()), environment, io);
}
