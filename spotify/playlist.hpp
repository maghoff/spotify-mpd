#ifndef SPOTIFY_PLAYLIST_HPP
#define SPOTIFY_PLAYLIST_HPP

#include <QMetaType>
#include <QObject>
#include "track.hpp"

struct sp_playlist;

namespace Spotify {

class Link;

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

	// All the functions below segfault if get() == 0

	Q_INVOKABLE bool isLoaded() const;
	Q_INVOKABLE int numTracks() const;
	Q_INVOKABLE Spotify::Track* track(int) const;
	Q_INVOKABLE QString name() const;
	/*
	sp_error sp_playlist_rename(sp_playlist *playlist, const char *new_name)
	sp_user * sp_playlist_owner(sp_playlist *playlist)
	*/
	Q_INVOKABLE bool isCollaborative() const;
	Q_INVOKABLE void setCollaborative(bool) const;
	Q_INVOKABLE bool hasPendingChanges() const;
	/*
	sp_error sp_playlist_add_tracks (sp_playlist *playlist, const sp_track **tracks, int num_tracks, int position)
	sp_error sp_playlist_remove_tracks (sp_playlist *playlist, const int *tracks, int num_tracks)
	sp_error sp_playlist_reorder_tracks (sp_playlist *playlist, const int *tracks, int num_tracks, int new_position)
	*/

signals:

public slots:

};

}

Q_DECLARE_METATYPE(Spotify::Playlist*)

#endif // SPOTIFY_PLAYLIST_HPP
