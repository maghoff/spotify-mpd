#include <libspotify/api.h>
#include "spotifyartist.hpp"
#include "spotifylink.hpp"
#include "spotifyobjectwrapper.hpp"

namespace Spotify {

SPOTIFY_OBJECT_WRAPPER(Artist, artist)

Artist::Artist(const Link& link) {
	p = sp_link_as_artist(link.get());
	if (p) sp_artist_add_ref(p);
}

bool Artist::isLoaded() const {
	return sp_artist_is_loaded(p);
}

QString Artist::name() const {
	return QString::fromUtf8(sp_artist_name(p));
}

}
