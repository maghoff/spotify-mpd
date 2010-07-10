#include <libspotify/api.h>
#include "spotifyartist.hpp"
#include "spotifylink.hpp"
#include "spotifyobjectwrapper.hpp"

namespace Spotify {

SPOTIFY_OBJECT_WRAPPER(Artist, artist)

SpotifyArtist::SpotifyArtist(const SpotifyLink& link) {
	p = sp_link_as_artist(link.get());
	if (p) sp_artist_add_ref(p);
}

bool SpotifyArtist::isLoaded() const {
	return sp_artist_is_loaded(p);
}

QString SpotifyArtist::name() const {
	return QString::fromUtf8(sp_artist_name(p));
}

}
