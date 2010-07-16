#ifndef SPOTIFY_PLAYLIST_HPP
#define SPOTIFY_PLAYLIST_HPP

#include <QMetaType>
#include <QObject>

struct sp_playlist;

namespace Spotify {

class Playlist : public QObject {
	Q_OBJECT

	sp_playlist* p;

public:
	Playlist();
	explicit Playlist(sp_playlist*); //< This will not add a ref
	~Playlist();

	Playlist(const Playlist&);
	Playlist& operator = (const Playlist&);

	sp_playlist* get() const { return p; }


signals:

public slots:

};

}

Q_DECLARE_METATYPE(Spotify::Playlist*)

#endif // SPOTIFY_PLAYLIST_HPP
