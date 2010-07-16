#include <cassert>
#include <libspotify/api.h>
#include "link.hpp"
#include "objectwrapper.hpp"
#include "playlist.hpp"
#include "track.hpp"

namespace Spotify {

SPOTIFY_OBJECT_WRAPPER(Playlist, playlist)

bool Playlist::isLoaded() const {
	return sp_playlist_is_loaded(p);
}

int Playlist::numTracks() const {
	return sp_playlist_num_tracks(p);
}

Track* Playlist::track(int t) const {
	sp_track* sp_t = sp_playlist_track(p, t);
	assert(sp_t);
	sp_track_add_ref(sp_t);
	return new Track(sp_t);
}

QString Playlist::name() const {
	return QString::fromUtf8(sp_playlist_name(p));
}

bool Playlist::isCollaborative() const {
	return sp_playlist_is_collaborative(p);
}

void Playlist::setCollaborative(bool b) const {
	sp_playlist_set_collaborative(p, b);
}

bool Playlist::hasPendingChanges() const {
	return sp_playlist_has_pending_changes(p);
}

}
