#ifndef SPOTIFYARTIST_HPP
#define SPOTIFYARTIST_HPP

#include <QMetaType>
#include <QObject>

struct sp_artist;

namespace Spotify {

class SpotifyLink;

class SpotifyArtist : public QObject {
	Q_OBJECT

	sp_artist* p;

public:
    SpotifyArtist();
	SpotifyArtist(sp_artist*); //< This will not add a ref
	SpotifyArtist(const SpotifyLink&);
	~SpotifyArtist();

	SpotifyArtist(const SpotifyArtist&);
	SpotifyArtist& operator = (const SpotifyArtist&);

	Q_INVOKABLE bool isLoaded() const;
	Q_INVOKABLE QString name() const;
};

}

Q_DECLARE_METATYPE(Spotify::SpotifyArtist*)

#endif // SPOTIFYARTIST_HPP
