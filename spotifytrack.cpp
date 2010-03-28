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

SpotifyTrack::SpotifyTrack(const SpotifyTrack& rhs) : QObject(rhs.parent()), p(rhs.p) {
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

bool SpotifyTrack::isAvailable() const {
	return sp_track_is_available(p);
}

// QVector<SpotifyArtist> SpotifyTrack::artists() const { }

// SpotifyAlbum SpotifyTrack::album() const { }

QString SpotifyTrack::name() const {
	return QString::fromUtf8(sp_track_name(p));
}

// NOTE: QTime is actually made to represent a point in time in a day. It will
// wrap around if the duration is greater than 24 hours -- unlikely for a track.
QTime SpotifyTrack::duration() const {
	QTime t(0, 0);
	t.addMSecs(sp_track_duration(p));
	return t;
}

int SpotifyTrack::popularity() const {
	return sp_track_popularity(p);
}

int SpotifyTrack::disc() const {
	return sp_track_disc(p);
}

int SpotifyTrack::index() const {
	return sp_track_index(p);
}
