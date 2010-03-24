#include <spotify/api.h>
#include "spotifylink.hpp"
#include "spotifyplayer.hpp"
#include "spotifysession.hpp"
#include "spotifytrack.hpp"
#include "log.hpp"

SpotifyPlayer::SpotifyPlayer(SpotifySession* session_, const logger& local_logger_) :
	QObject(session_),
	session(session_),
	local_logger(local_logger_),
	isPlaying(false)
{
	LLOG(TRACE, __FUNCTION__);

	connect(session, SIGNAL(metadataUpdated()), this, SLOT(metadataUpdated()));
}

SpotifyPlayer::~SpotifyPlayer() {
	LLOG(TRACE, __FUNCTION__);
}

void SpotifyPlayer::tryPlay() {
	if (track.isLoaded() && !isPlaying) {
		LLOG(OPERATION, "Track is loaded");
		session->playerLoad(track);
		session->playerPlay(true);
		isPlaying = true;
	}
}

void SpotifyPlayer::play(QUrl trackUrl_) {
	trackUrl = trackUrl_;
	SpotifyLink link(trackUrl);
	track = SpotifyTrack(link);
	if (!track.get()) LLOG(ERROR, "Not a track link");

	isPlaying = false;
	tryPlay();
}

void SpotifyPlayer::metadataUpdated() {
	LLOG(TRACE, __FUNCTION__);
	tryPlay();
}
