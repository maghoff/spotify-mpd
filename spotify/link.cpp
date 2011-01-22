#include <libspotify/api.h>
#include "link.hpp"
#include "objectwrapper.hpp"

namespace Spotify {

SPOTIFY_OBJECT_WRAPPER(Link, link)

Link::Link(QUrl url) {
	QByteArray url_buffer = url.toEncoded();
	p = sp_link_create_from_string(url_buffer.constData());
}

}
