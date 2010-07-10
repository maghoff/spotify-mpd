#ifndef SPOTIFYALBUM_HPP
#define SPOTIFYALBUM_HPP

#include <QMetaType>
#include <QObject>
#include "spotifyartist.hpp"

class sp_album;

namespace Spotify {

class SpotifyLink;

class SpotifyAlbum : public QObject {
	Q_OBJECT

	sp_album* p;

public:
	SpotifyAlbum();
	SpotifyAlbum(sp_album*); //< This will not add a ref
	SpotifyAlbum(const SpotifyLink&);
	~SpotifyAlbum();

	SpotifyAlbum(const SpotifyAlbum&);
	SpotifyAlbum& operator = (const SpotifyAlbum&);

	sp_album* get() const { return p; }

	// All the functions below segfault if get() == 0

	Q_INVOKABLE bool isLoaded() const;
	Q_INVOKABLE bool isAvailable() const;
	Q_INVOKABLE SpotifyArtist* artist() const;
	// Q_INVOKABLE ? cover() const;
	Q_INVOKABLE QString name() const;
	Q_INVOKABLE int year() const;
	// Q_INVOKABLE ? type() const;
};

}

Q_DECLARE_METATYPE(Spotify::SpotifyAlbum*)

#endif // SPOTIFYALBUM_HPP
