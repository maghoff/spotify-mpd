#ifndef SPOTIFYALBUM_HPP
#define SPOTIFYALBUM_HPP

#include <QMetaType>
#include <QObject>
#include "spotifyartist.hpp"

class sp_album;

namespace Spotify {

class Link;

class Album : public QObject {
	Q_OBJECT

	sp_album* p;

public:
	Album();
	Album(sp_album*); //< This will not add a ref
	Album(const Link&);
	~Album();

	Album(const Album&);
	Album& operator = (const Album&);

	sp_album* get() const { return p; }

	// All the functions below segfault if get() == 0

	Q_INVOKABLE bool isLoaded() const;
	Q_INVOKABLE bool isAvailable() const;
	Q_INVOKABLE Artist* artist() const;
	// Q_INVOKABLE ? cover() const;
	Q_INVOKABLE QString name() const;
	Q_INVOKABLE int year() const;
	// Q_INVOKABLE ? type() const;
};

}

Q_DECLARE_METATYPE(Spotify::Album*)

#endif // SPOTIFYALBUM_HPP
