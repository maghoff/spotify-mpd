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
	connect(server, SIGNAL(newConnection()), this, SLOT(newConnection()));

	const unsigned short port = 6602;

	bool ok = server->listen(QHostAddress::Any, port);
	if (ok) {
		LLOG(STATE, "Listening on port " << port);
	} else {
		LLOG(WARNING,
			"Unable to listen on port " << port << ". "
			"Check for other running instances."
		);
	}
}

ScriptListener::~ScriptListener() {
}

void ScriptListener::newConnection() {
	LLOG(OPERATION, "New connection");

	QTcpSocket* io = server->nextPendingConnection();

	std::stringstream ss;
	ss << utf8_str(io->peerAddress().toString()) << ':' << io->peerPort();

	new ScriptEnvironment(this, logger(local_logger, ss.str()), environment, io);
}
