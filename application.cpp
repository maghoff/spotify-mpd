#include <cassert>
#include <iostream>
#include <QString>
#include <QVector>
#include <QTcpSocket>
#include "application.hpp"
#include "mpd_utils.hpp"

application::application() {
	server.listen(QHostAddress::Any, 6601);
	connect(&server, SIGNAL(newConnection()), this, SLOT(newConnection()));
}

application::~application() {
}

void application::newConnection() {
	std::cout << "New connection" << std::endl;

	io = server.nextPendingConnection();
	assert(io);

	io->write("OK MPD 0.0.0\n");

	connect(io, SIGNAL(readyRead()), this, SLOT(readMe()));
}

void application::readMe() {
	assert(io);

	while (io->canReadLine()) {
		QByteArray line = io->readLine();
		QVector<QString> msg = splitMessage(line);

		std::cout << "Received line: [";
		for (QVector<QString>::const_iterator i = msg.begin(); i != msg.end(); ++i) {
			if (i != msg.begin()) std::cout << ", ";
			std::cout << '"' << (*i).toUtf8().data() << '\"';
		}
		std::cout << ']' << std::endl;

		io->write("ACK [5@0] {} unknown command\n");
	}
}
