#include <cassert>
#include <sstream>
#include <stdexcept>
#include <QString>
#include <QVector>
#include <QTcpSocket>
#include <QIODevice>
#include "log.hpp"
#include "mpd_utils.hpp"
#include "mpdserver.hpp"

class mpd_error : public std::runtime_error {
public:
	mpd_error(int code, const std::string& description);

	int code;
};

mpd_error::mpd_error(int code_, const std::string& description) :
	std::runtime_error(description),
	code(code_)
{
}

MPDServer::MPDServer(QObject* parent, const logger& local_logger_, QIODevice* io_) :
	QObject(parent),
	local_logger(local_logger_),
	io(io_),
	command_list_status(no_command_list)
{

	#define DICT_BIND(n) \
		MPDFunctions.insert(#n, &MPDServer::mpd_##n);

	DICT_BIND(command_list_begin);
	DICT_BIND(command_list_ok_begin);
	DICT_BIND(command_list_end);
	DICT_BIND(status);
	DICT_BIND(currentsong);

	#undef DICT_BIND

	assert(io);
	io->write("OK MPD 0.0.0\n");
	connect(io, SIGNAL(readyRead()), this, SLOT(readMe()));
}

MPDServer::~MPDServer() {
}

void MPDServer::readMe() {
	assert(io);

	while (io->canReadLine()) {
		QByteArray line = io->readLine();
		QVector<QString> msg = splitMessage(line);
		processMessage(msg);
	}
}

void MPDServer::processMessage(QVector<QString> msg) {
	if (local_logger->should_log(log_level::INPUT)) {
		std::ostringstream ss;
		ss << "Handling message: [";
		for (QVector<QString>::const_iterator i = msg.begin(); i != msg.end(); ++i) {
			if (i != msg.begin()) ss << ", ";
			ss << '"' << (*i).toUtf8().data() << '\"';
		}
		ss << ']';

		local_logger->log(log_level::INPUT, ss.str());
	}

	if (command_list_status == command_list_failed) {
		if (msg.size() == 1 && msg[0] == "command_list_end") {
			command_list_status = no_command_list;
		}
		return;
	}

	if (msg.size() >= 1 && MPDFunctions.contains(msg[0])) {
		mpd_func f = MPDFunctions.value(msg[0]);
		try {
			(this->*f)(msg);
			if (
				command_list_status == no_command_list ||
				command_list_status == command_list
			) {
				io->write("OK\n");
			} else if (command_list_status == command_list_ok) {
				io->write("list_OK\n");
			} else {
				assert(
					command_list_status == command_list_starting ||
					command_list_status == command_list_ok_starting
				);
				reinterpret_cast<int&>(command_list_status)++;
			}
		}
		catch (const mpd_error& err) {
			std::ostringstream ss;
			ss <<
				"ACK [" << err.code << "@" << command_list_number << "] "
				"{" << msg[0].toStdString() << "} " << err.what() << std::endl;
			std::string s = ss.str();
			io->write(QByteArray(s.data(), s.length()));
			if (command_list_status != no_command_list) {
				command_list_status = command_list_failed;
			}
		}
	} else {
		if (local_logger->should_log(log_level::OPERATION)) {
			std::stringstream ss;
			ss << "Unhandled message: [";
			for (QVector<QString>::const_iterator i = msg.begin(); i != msg.end(); ++i) {
				if (i != msg.begin()) ss << ", ";
				ss << '"' << (*i).toUtf8().data() << '\"';
			}
			ss << ']';
			local_logger->log(log_level::OPERATION, ss.str());
		}

		//io->write("ACK [5@0] {} unknown command\n");

		LLOG(OPERATION, "Cheating, and saying OK");
		io->write("OK\n");
	}
	++command_list_number;
}

void MPDServer::mpd_command_list_begin(QVector<QString> msg) {
	assert(msg.size() == 1);
	assert(command_list_status == no_command_list);
	command_list_status = command_list_starting;
	command_list_number = 0;
}

void MPDServer::mpd_command_list_ok_begin(QVector<QString> msg) {
	assert(msg.size() == 1);
	assert(command_list_status == no_command_list);
	command_list_status = command_list_ok_starting;
	command_list_number = 0;
}

void MPDServer::mpd_command_list_end(QVector<QString> msg) {
	assert(msg.size() == 1);
	assert(
		command_list_status == command_list ||
		command_list_status == command_list_ok
	);
	command_list_status = no_command_list;
}

void MPDServer::mpd_currentsong(QVector<QString> msg) {
	assert(msg.size() == 1);
	// We are not playing any song ;)
}

void MPDServer::mpd_status(QVector<QString> msg) {
	assert(msg.size() == 1);
	// Let's see how little we can get away with ;)
	io->write(
		"volume: 100\n"
		"repeat: 0\n"
		"random: 0\n"
		"single: 0\n"
		"consume: 0\n"
		"playlist: 1\n"
		"playlistlength: 0\n"
		"xfade: 0\n"
		"state: stop\n"
	);
}
