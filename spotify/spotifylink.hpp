#ifndef SPOTIFYLINK_HPP
#define SPOTIFYLINK_HPP

#include <QUrl>

class sp_link;

namespace Spotify {

class Link {
	sp_link* p;

public:
	Link();
	Link(QUrl);
	~Link();

	Link(const Link&);
	Link& operator = (const Link&);

	sp_link* get() const { return p; }
};

}

#endif // SPOTIFYLINK_HPP
