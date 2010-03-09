#include <spotify/api.h>
#include "spotifylink.hpp"

SpotifyLink::SpotifyLink() : p(0) {
}

SpotifyLink::SpotifyLink(QUrl url) {
	QByteArray url_buffer = url.toEncoded();
	p = sp_link_create_from_string(url_buffer.constData());
}

SpotifyLink::SpotifyLink(const SpotifyLink& rhs) : p(rhs.p) {
	sp_link_add_ref(p);
}

SpotifyLink::~SpotifyLink() {
	if (p) sp_link_release(p);
}

SpotifyLink& SpotifyLink::operator = (const SpotifyLink& rhs) {
	if (p) sp_link_release(p);
	p = rhs.p;
	sp_link_add_ref(p);
	return *this;
}
