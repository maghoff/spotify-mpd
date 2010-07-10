#ifndef SPOTIFYTRACK_HPP
#define SPOTIFYTRACK_HPP

#include <QMetaType>
#include <QObject>
#include <QTime>
#include "album.hpp"
#include "artist.hpp"

struct sp_track;

namespace Spotify {

class Link;

class Track : public QObject {
	Q_OBJECT

	sp_track* p;

public:
	Track();
	Track(sp_track*); //< This will not add a ref
	Track(const Link&);
	~Track();

	Track(const Track&);
	Track& operator = (const Track&);

	sp_track* get() const { return p; }

	// All the functions below segfault if get() == 0

	Q_INVOKABLE bool isLoaded() const;
	Q_INVOKABLE bool isAvailable() const;
	Q_INVOKABLE int numArtists() const;
	Q_INVOKABLE Spotify::Artist* artist(int) const;
	Q_INVOKABLE Spotify::Album* album() const;
	Q_INVOKABLE QString name() const;
	Q_INVOKABLE QTime duration() const;
	Q_INVOKABLE int popularity() const; //< 0 <= popularity <= 100

	/* Disc index. Possible values are [1, total number of discs on album] This
	   function returns valid data only for tracks appearing in a browse artist
	   or browse album result (otherwise returns 0). */
	Q_INVOKABLE int disc() const;

	/* Track position, starts at 1 (relative the corresponding disc) This
	   function returns valid data only for tracks appearing in a browse artist
	   or browse album result (otherwise returns 0). */
	Q_INVOKABLE int index() const;
};

}

Q_DECLARE_METATYPE(Spotify::Track*)

#endif // SPOTIFYTRACK_HPP
