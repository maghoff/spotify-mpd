#include <iostream>
#include <QtCore/QCoreApplication>
#include <QtCore/QSettings>
#include "application.hpp"
#include "spotifysession.hpp"
#include "spotifyplayer.hpp"
#include "alsaaudiooutput.hpp"
#include "log.hpp"
#include "console_logger.hpp"

int main(int argc, char *argv[]) {
	logger local_logger(new console_logger(log_level::ALL));
	LLOG(TRACE, "Welcome");

	QCoreApplication a(argc, argv);
	a.setOrganizationName("No name; Zapp Brannigan");
	a.setApplicationName("spotify-mpd");

	application app(local_logger->create_sublogger("application"));

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

		return 1;
	}

	SpotifySession s(&app, local_logger->create_sublogger("SpotifySession"));

	AlsaAudioOutput ao(&s, local_logger->create_sublogger("AlsaAudioOutput"));
	s.setAudioOutput(&ao);

	s.login(settings.value("username", "").toString(), settings.value("password", "").toString());

	SpotifyPlayer p(&s, local_logger->create_sublogger("player"), QUrl("spotify:track:6JEK0CvvjDjjMUBFoXShNZ"));

	return a.exec();
}
