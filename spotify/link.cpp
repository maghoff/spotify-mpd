#include <libspotify/api.h>
#include "link.hpp"

namespace Spotify {

Link::Link() : p(0) {
}

Link::Link(QUrl url) {
	QByteArray url_buffer = url.toEncoded();
	p = sp_link_create_from_string(url_buffer.constData());
}

Link::Link(const Link& rhs) : p(rhs.p) {
	sp_link_add_ref(p);
}

Link::~Link() {
	if (p) sp_link_release(p);
}

Link& Link::operator = (const Link& rhs) {
	if (p) sp_link_release(p);
	p = rhs.p;
	sp_link_add_ref(p);
	return *this;
}

}
