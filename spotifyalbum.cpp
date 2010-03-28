#include <spotify/api.h>
#include "spotifylink.hpp"
#include "spotifyalbum.hpp"
#include "spotifyobjectwrapper.hpp"

SPOTIFY_OBJECT_WRAPPER(Album, album)

SpotifyAlbum::SpotifyAlbum(const SpotifyLink& link) {
	p = sp_link_as_album(link.get());
	if (p) sp_album_add_ref(p);
}

bool SpotifyAlbum::isLoaded() const {
	return sp_album_is_loaded(p);
}

bool SpotifyAlbum::isAvailable() const {
	return sp_album_is_available(p);
}

SpotifyArtist* SpotifyAlbum::artist() const {
	return new SpotifyArtist(sp_album_artist(p));
}

QString SpotifyAlbum::name() const {
	return QString::fromUtf8(sp_album_name(p));
}

int SpotifyAlbum::year() const {
	return sp_album_year(p);
}
