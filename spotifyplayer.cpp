#include <spotify/api.h>
#include "spotifylink.hpp"
#include "spotifyplayer.hpp"
#include "spotifysession.hpp"
#include "spotifytrack.hpp"
#include "log.hpp"

SpotifyPlayer::SpotifyPlayer(SpotifySession* session_, const logger& local_logger_, QUrl trackUrl_) :
	QObject(session_),
	session(session_),
	local_logger(local_logger_),
	trackUrl(trackUrl_)
{
	LLOG(TRACE, __FUNCTION__);

	connect(session, SIGNAL(metadataUpdated()), this, SLOT(metadataUpdated()));

	SpotifyLink link(trackUrl);
	track = SpotifyTrack(link);
	if (!track.get()) LLOG(ERROR, "Not a track link");
}

SpotifyPlayer::~SpotifyPlayer() {
	LLOG(TRACE, __FUNCTION__);
}

void SpotifyPlayer::metadataUpdated() {
	LLOG(TRACE, __FUNCTION__);

	if (track.isLoaded()) {
		session->playerLoad(track);
		session->playerPlay(true);
	}
}
