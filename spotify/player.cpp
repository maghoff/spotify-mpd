#include <libspotify/api.h>
#include "link.hpp"
#include "player.hpp"
#include "session.hpp"
#include "track.hpp"
#include "log.hpp"

namespace Spotify {

Player::Player(Session* session_, const logger& local_logger_) :
	QObject(session_),
	session(session_),
	local_logger(local_logger_),
	isPlaying(false)
{
	LLOG(TRACE, __FUNCTION__);

	connect(session, SIGNAL(metadataUpdated()), this, SLOT(metadataUpdated()));
}

Player::~Player() {
	LLOG(TRACE, __FUNCTION__);
}

void Player::tryPlay() {
	if (track.get() && track.isLoaded() && !isPlaying) {
		LLOG(OPERATION, "Track is loaded");
		session->playerLoad(track);
		session->playerPlay(true);
		isPlaying = true;
	}
}

void Player::playTrack(Track* t) {
	track = *t;
	track.setParent(this);
	if (!track.get()) LLOG(ERROR, "Not a track link");

	isPlaying = false;
	tryPlay();
}

void Player::playUri(QUrl trackUrl_) {
	trackUrl = trackUrl_;
	Link link(trackUrl);
	Track t(link);
	playTrack(&t);
}

void Player::playUri(QString trackUrl_) {
	playUri(QUrl(trackUrl_));
}

void Player::metadataUpdated() {
	LLOG(TRACE, __FUNCTION__);
	tryPlay();
}

}
