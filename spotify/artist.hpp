#ifndef SPOTIFYARTIST_HPP
#define SPOTIFYARTIST_HPP

#include <QMetaType>
#include <QObject>

struct sp_artist;

namespace Spotify {

class Link;

class Artist : public QObject {
	Q_OBJECT

	sp_artist* p;

public:
    Artist();
	Artist(sp_artist*); //< This will not add a ref
	Artist(const Link&);
	~Artist();

	Artist(const Artist&);
	Artist& operator = (const Artist&);

	Q_INVOKABLE bool isLoaded() const;
	Q_INVOKABLE QString name() const;
};

}

Q_DECLARE_METATYPE(Spotify::Artist*)

#endif // SPOTIFYARTIST_HPP
