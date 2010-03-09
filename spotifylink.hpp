#ifndef SPOTIFYLINK_HPP
#define SPOTIFYLINK_HPP

#include <QUrl>

class sp_link;

class SpotifyLink {
	sp_link* p;

public:
	SpotifyLink();
	SpotifyLink(QUrl);
	~SpotifyLink();

	SpotifyLink(const SpotifyLink&);
	SpotifyLink& operator = (const SpotifyLink&);

	sp_link* get() const { return p; }
};

#endif // SPOTIFYLINK_HPP
