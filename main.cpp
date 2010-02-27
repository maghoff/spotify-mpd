#include <iostream>
#include <QtCore/QCoreApplication>
#include <QtCore/QSettings>
#include <Phonon/MediaObject>
#include <Phonon/MediaSource>
#include <Phonon/AudioOutput>
#include "application.hpp"
#include "spotifysession.hpp"

int main(int argc, char *argv[]) {
	QCoreApplication a(argc, argv);
	a.setOrganizationName("No name; Zapp Brannigan");
	a.setApplicationName("spotify-mpd");

	application app;

	/*
	Phonon::MediaObject *mediaObject = new Phonon::MediaObject(&app);
	mediaObject->setCurrentSource(Phonon::MediaSource("/home/mag/Downloads/gishdemo/music/sewer.ogg"));
	Phonon::AudioOutput *audioOutput = new Phonon::AudioOutput(Phonon::MusicCategory, &app);
	Phonon::Path path = Phonon::createPath(mediaObject, audioOutput);

	mediaObject->play();
	*/

	QSettings settings;

	settings.beginGroup("spotify");
	if (
		(settings.value("username", "").toString().size() == 0) ||
		(settings.value("password", "").toString().size() == 0)
	) {
		std::cerr <<
			"ERROR: Please enter your spotify username and password in the "
			"configuration file: '" << settings.fileName().toStdString() << '\'' <<
			std::endl;

		// Write some junk to have the path and file created:
		if (!settings.contains("username")) settings.setValue("username", "");
		if (!settings.contains("password")) settings.setValue("password", "");
		settings.sync();

		return 1;
	}

	SpotifySession s(&app, settings.value("username", "").toString(), settings.value("password", "").toString());
	s.connect();

	return a.exec();
}
