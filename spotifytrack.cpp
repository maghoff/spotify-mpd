#include <spotify/api.h>
#include "spotifylink.hpp"
#include "spotifytrack.hpp"

SpotifyTrack::SpotifyTrack() : p(0) {
}

SpotifyTrack::SpotifyTrack(const SpotifyLink& link) {
	p = sp_link_as_track(link.get());
	if (p) sp_track_add_ref(p);
}

SpotifyTrack::~SpotifyTrack() {
	if (p) sp_track_release(p);
}

SpotifyTrack::SpotifyTrack(const SpotifyTrack& rhs) : p(rhs.p) {
	sp_track_add_ref(p);
}

SpotifyTrack& SpotifyTrack::operator = (const SpotifyTrack& rhs) {
	if (p) sp_track_release(p);
	p = rhs.p;
	sp_track_add_ref(p);
	return *this;
}

bool SpotifyTrack::isLoaded() const {
	return sp_track_is_loaded(p);
}
