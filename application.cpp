#include <stdexcept>
#include <QTcpSocket>
#include <QSettings>
#include <QString>
#include "application.hpp"
#include "alsaaudiooutput.hpp"
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

	ao = new AlsaAudioOutput(session, local_logger->create_sublogger("AlsaAudioOutput"));
	session->setAudioOutput(ao);

	session->login(settings.value("username", "").toString(), settings.value("password", "").toString());

	player = new SpotifyPlayer(session, local_logger->create_sublogger("player"));
	player->play(QUrl("spotify:track:6JEK0CvvjDjjMUBFoXShNZ"));

	scriptListener = new ScriptListener(this, local_logger->create_sublogger("script_listener"));

	QTcpSocket* stdIO = new QTcpSocket(this);
	stdIO->setSocketDescriptor(0);

	script = new ScriptEnvironment(this, local_logger->create_sublogger("script_console"), stdIO);
}

application::~application() {
}
