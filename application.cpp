#include <cstdio>
#include <stdexcept>
#include <QTcpSocket>
#include <QSettings>
#include <QString>
#include "application.hpp"
#include "aoaudiooutput.hpp"
#include "mpd/mpdlistener.hpp"
#include "scriptenvironment.hpp"
#include "scriptlistener.hpp"
#include "spotify/player.hpp"
#include "spotify/session.hpp"
#include "log.hpp"

application::application(const logger& local_logger_) :
	local_logger(local_logger_)
{
	QSettings settings;

	settings.beginGroup("spotify");
	if (
		(settings.value("username", "").toString().size() == 0) ||
		(settings.value("password", "").toString().size() == 0)
	) {
		LLOG(ERROR,
			"Please enter your spotify username and password in the "
			"configuration file: '" << settings.fileName().toStdString() << '\''
		);

		// Write some junk to have the path and file created:
		if (!settings.contains("username")) settings.setValue("username", "");
		if (!settings.contains("password")) settings.setValue("password", "");
		settings.sync();

		throw std::runtime_error("Lul");
	}

	mpd_listener = new MPDListener(this, logger(local_logger, "mpd"));

	session = new Spotify::Session(this, logger(local_logger, "session"));
	session->setObjectName("spotify");

	ao = new AOAudioOutput(session, logger(local_logger, "AOAudioOutput"));
	session->setAudioOutput(ao);

	session->login(settings.value("username", "").toString(), settings.value("password", "").toString());

	player = new Spotify::Player(session, logger(local_logger, "player"));
	player->setObjectName("player");

	scriptListener = new ScriptListener(this, logger(local_logger, "script_listener"), this);

	// Inspired by http://stackoverflow.com/questions/1312922/detect-if-stdin-is-a-terminal-or-pipe-in-c-c-qt/1312957#1312957
	unsigned stdin_fileno = fileno(stdin);

	if (isatty(stdin_fileno)) {
		LLOG(ANALYSIS, "stdio is a terminal. Enabling interactive session");
		QTcpSocket* stdIO = new QTcpSocket(this);
		stdIO->setSocketDescriptor(stdin_fileno);

		terminal = new ScriptEnvironment(this, logger(local_logger, "script_console"), this, stdIO);
	} else {
		LLOG(ANALYSIS, "stdio is not a terminal. Not enabling interactive session");
		terminal = 0;
	}
}

application::~application() {
}

void application::log(int level_, QString msg) {
	log_level_t level(static_cast<log_level_t>(level_));
	if (local_logger.should_log(level)) {
		local_logger.log(level, std::string(msg.toUtf8().constData()));
	}
}
