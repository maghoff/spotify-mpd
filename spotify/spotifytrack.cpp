#include <cassert>
#include <libspotify/api.h>
#include "spotifylink.hpp"
#include "spotifytrack.hpp"
#include "spotifyobjectwrapper.hpp"

namespace Spotify {

SPOTIFY_OBJECT_WRAPPER(Track, track)

Track::Track(const Link& link) {
	p = sp_link_as_track(link.get());
	if (p) sp_track_add_ref(p);
}

bool Track::isLoaded() const {
	return sp_track_is_loaded(p);
}

bool Track::isAvailable() const {
	return sp_track_is_available(p);
}

int Track::numArtists() const {
	return sp_track_num_artists(p);
}

Artist* Track::artist(int index) const {
	assert(index >= 0);
	assert(index < numArtists());
	return new Artist(sp_track_artist(p, index));
}

Album* Track::album() const {
	return new Album(sp_track_album(p));
}

QString Track::name() const {
	return QString::fromUtf8(sp_track_name(p));
}

// NOTE: QTime is actually made to represent a point in time in a day. It will
// wrap around if the duration is greater than 24 hours -- unlikely for a track.
QTime Track::duration() const {
	QTime t(0, 0);
	t.addMSecs(sp_track_duration(p));
	return t;
}

int Track::popularity() const {
	return sp_track_popularity(p);
}

int Track::disc() const {
	return sp_track_disc(p);
}

int Track::index() const {
	return sp_track_index(p);
}

}
