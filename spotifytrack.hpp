#ifndef SPOTIFYTRACK_HPP
#define SPOTIFYTRACK_HPP

#include <QObject>
#include <QTime>

class sp_track;
class SpotifyLink;

class SpotifyTrack : public QObject {
	Q_OBJECT

	sp_track* p;

public:
	SpotifyTrack();
	SpotifyTrack(const SpotifyLink&);
	~SpotifyTrack();

	SpotifyTrack(const SpotifyTrack&);
	SpotifyTrack& operator = (const SpotifyTrack&);

	sp_track* get() const { return p; }

	// All the functions below segfault if get() == 0

	Q_INVOKABLE bool isLoaded() const;
	Q_INVOKABLE bool isAvailable() const;
	// Q_INVOKABLE QVector<SpotifyArtist> artists() const;
	// Q_INVOKABLE SpotifyAlbum album() const;
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

#endif // SPOTIFYTRACK_HPP
