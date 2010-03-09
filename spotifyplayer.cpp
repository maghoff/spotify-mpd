#include "spotifysession.hpp"
#include "spotifyplayer.hpp"
#include "log.hpp"

SpotifyPlayer::SpotifyPlayer(SpotifySession* session_, const logger& local_logger_, QUrl trackUrl_) :
	QObject(session_),
	session(session_),
	local_logger(local_logger_),
	trackUrl(trackUrl_)
{
	LLOG(TRACE, __FUNCTION__);

	connect(session, SIGNAL(metadataUpdated()), this, SLOT(metadataUpdated()));
}

SpotifyPlayer::~SpotifyPlayer() {
	LLOG(TRACE, __FUNCTION__);
}

void SpotifyPlayer::metadataUpdated() {
	LLOG(TRACE, __FUNCTION__);
}
