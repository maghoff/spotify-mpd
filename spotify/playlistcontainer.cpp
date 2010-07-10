#include <cassert>
#include <stdexcept>
#include <libspotify/api.h>
#include "playlistcontainer.hpp"
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


} // namespace Spotify
