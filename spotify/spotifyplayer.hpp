#ifndef SPOTIFYPLAYER_HPP
#define SPOTIFYPLAYER_HPP

#include <QObject>
#include <QUrl>
#include "spotifytrack.hpp"
#include "logger_base.hpp"

namespace Spotify {

class SpotifySession;

class SpotifyPlayer : public QObject {
	Q_OBJECT

	SpotifySession* session;
	logger local_logger;
	bool isPlaying;
	QUrl trackUrl;
	SpotifyTrack track;

	void tryPlay();

public:
	// The SpotifySession-object is used as parent for QObject
	SpotifyPlayer(SpotifySession*, const logger&);

	~SpotifyPlayer();

public slots:
	void playTrack(SpotifyTrack*);
	void playUri(QUrl);
	void playUri(QString);

private slots:
	void metadataUpdated();
};

}

#endif // SPOTIFYPLAYER_HPP
