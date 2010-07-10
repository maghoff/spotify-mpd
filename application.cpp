#include <cstdio>
#include <stdexcept>
#include <QTcpSocket>
#include <QSettings>
#include <QString>
#include "application.hpp"
#include "aoaudiooutput.hpp"
#include "mpdlistener.hpp"
#include "scriptenvironment.hpp"
#include "scriptlistener.hpp"
#include "spotifyplayer.hpp"
#include "spotifysession.hpp"
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

	mpd_listener = new MPDListener(this, local_logger_->create_sublogger("mpd"));

	session = new SpotifySession(this, local_logger->create_sublogger("session"));

	ao = new AOAudioOutput(session, local_logger->create_sublogger("AOAudioOutput"));
	session->setAudioOutput(ao);

	session->login(settings.value("username", "").toString(), settings.value("password", "").toString());

	player = new SpotifyPlayer(session, local_logger->create_sublogger("player"));
	player->setObjectName("player");

	scriptListener = new ScriptListener(this, local_logger->create_sublogger("script_listener"), this);

	// Inspired by http://stackoverflow.com/questions/1312922/detect-if-stdin-is-a-terminal-or-pipe-in-c-c-qt/1312957#1312957
	unsigned stdin_fileno = fileno(stdin);

	if (isatty(stdin_fileno)) {
		LLOG(ANALYSIS, "stdio is a terminal. Enabling interactive session");
		QTcpSocket* stdIO = new QTcpSocket(this);
		stdIO->setSocketDescriptor(stdin_fileno);

		terminal = new ScriptEnvironment(this, local_logger->create_sublogger("script_console"), this, stdIO);
	} else {
		LLOG(ANALYSIS, "stdio is not a terminal. Not enabling interactive session");
		terminal = 0;
	}
}

application::~application() {
}
