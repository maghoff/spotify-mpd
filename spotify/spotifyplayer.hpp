#ifndef SPOTIFYPLAYER_HPP
#define SPOTIFYPLAYER_HPP

#include <QObject>
#include <QUrl>
#include "spotifytrack.hpp"
#include "logger_base.hpp"

namespace Spotify {

class Session;

class Player : public QObject {
	Q_OBJECT

	Session* session;
	logger local_logger;
	bool isPlaying;
	QUrl trackUrl;
	Track track;

	void tryPlay();

public:
	// The Session-object is used as parent for QObject
	Player(Session*, const logger&);

	~Player();

public slots:
	void playTrack(Track*);
	void playUri(QUrl);
	void playUri(QString);

private slots:
	void metadataUpdated();
};

}

#endif // SPOTIFYPLAYER_HPP