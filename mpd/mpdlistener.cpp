#include <QTcpSocket>
#include <QIODevice>
#include "mpdserver.hpp"
#include "mpdlistener.hpp"
#include "log.hpp"

namespace {

std::string utf8_str(QString s) {
	QByteArray a = s.toUtf8();
	return std::string(a.data(), a.data() + a.size());
}

}

MPDListener::MPDListener(QObject* parent, const logger& local_logger_) :
	QObject(parent),
	local_logger(local_logger_)
{
	server.listen(QHostAddress::Any, 6601);
	connect(&server, SIGNAL(newConnection()), this, SLOT(newConnection()));
}

MPDListener::~MPDListener() {
}

void MPDListener::newConnection() {
	LLOG(OPERATION, "New connection");

	QTcpSocket* io = server.nextPendingConnection();

	std::stringstream ss;
	ss << utf8_str(io->peerAddress().toString()) << ':' << io->peerPort();

	new MPDServer(this, local_logger->create_sublogger(ss.str()), io);
}
