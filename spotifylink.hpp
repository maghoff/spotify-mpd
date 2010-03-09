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
};

#endif // SPOTIFYLINK_HPP
