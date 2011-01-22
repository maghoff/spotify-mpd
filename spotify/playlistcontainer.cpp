#include <cassert>
#include <stdexcept>
#include <libspotify/api.h>
#include "playlistcontainer.hpp"
#include "link.hpp"
#include "objectwrapper.hpp"

namespace Spotify {

PlaylistContainer::PlaylistContainer(sp_playlistcontainer* p_) :
	p(p_)
{
	assert(p);
	if (p == 0) throw std::logic_error("PlaylistContainer::PlaylistContainer does not accept a null parameter");
}

PlaylistContainer::~PlaylistContainer() {
	// As far as I can tell, there is no cleaning up possible for an sp_playlistcontainer.
}

int PlaylistContainer::numPlaylists() const {
	return sp_playlistcontainer_num_playlists(p);
}

Playlist* PlaylistContainer::playlist(int i) const {
	sp_playlist* sp_p = sp_playlistcontainer_playlist(p, i);
	assert(sp_p);
	sp_playlist_add_ref(sp_p);
	return new Playlist(sp_p);
}

Playlist* PlaylistContainer::addPlaylist(const Link* lp) const {
	const Link& l = *lp;
	sp_playlist* sp_p = sp_playlistcontainer_add_playlist(p, l.get());
	if (!sp_p) return 0;
	sp_playlist_add_ref(sp_p);
	return new Playlist(sp_p);
}

} // namespace Spotify
