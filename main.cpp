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

	application app(local_logger);

	return a.exec();
}
