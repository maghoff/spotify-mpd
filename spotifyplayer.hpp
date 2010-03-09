#ifndef SPOTIFYPLAYER_HPP
#define SPOTIFYPLAYER_HPP

#include <QObject>
#include <QUrl>
#include "logger_base.hpp"

class SpotifySession;

class SpotifyPlayer : public QObject {
	Q_OBJECT

	SpotifySession* session;
	logger local_logger;
	QUrl trackUrl;

public:
	// The SpotifySession-object is used as parent for QObject
	SpotifyPlayer(SpotifySession*, const logger&, QUrl);

	~SpotifyPlayer();

private slots:
	void metadataUpdated();
};

#endif // SPOTIFYPLAYER_HPP
