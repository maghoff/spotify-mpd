#include <libspotify/api.h>
#include "link.hpp"
#include "album.hpp"
#include "objectwrapper.hpp"

namespace Spotify {

SPOTIFY_OBJECT_WRAPPER(Album, album)

Album::Album(const Link& link) {
	p = sp_link_as_album(link.get());
	if (p) sp_album_add_ref(p);
}

bool Album::isLoaded() const {
	return sp_album_is_loaded(p);
}

bool Album::isAvailable() const {
	return sp_album_is_available(p);
}

Artist* Album::artist() const {
	return new Artist(sp_album_artist(p));
}

QString Album::name() const {
	return QString::fromUtf8(sp_album_name(p));
}

int Album::year() const {
	return sp_album_year(p);
}

}
