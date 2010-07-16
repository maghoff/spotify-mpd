#ifndef SPOTIFY_PLAYLIST_HPP
#define SPOTIFY_PLAYLIST_HPP

#include <memory>
#include <QMetaType>
#include <QObject>
#include "track.hpp"

struct sp_playlist;
struct sp_playlist_callbacks;
struct sp_track;

namespace Spotify {

class Link;

class Playlist : public QObject {
	Q_OBJECT

	sp_playlist* p;
	std::auto_ptr<sp_playlist_callbacks> cb;

	static void handle_tracks_added(sp_playlist*, sp_track * const *, int, int, void*);
	static void handle_tracks_removed(sp_playlist *pl, const int *tracks, int num_tracks, void *userdata);
	static void handle_tracks_moved(sp_playlist *pl, const int *tracks, int num_tracks, int new_position, void *userdata);
	static void handle_playlist_renamed(sp_playlist *pl, void *userdata);
	static void handle_playlist_state_changed(sp_playlist *pl, void *userdata);
	static void handle_playlist_update_in_progress(sp_playlist *pl, bool done, void *userdata);
	static void handle_playlist_metadata_updated(sp_playlist *pl, void *userdata);

public:
	explicit Playlist(sp_playlist*); //< This will not add a ref
	~Playlist();

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
	void tracksAdded(/* tracks? */ int num_tracks, int position);
	void tracksRemoved(/* tracks? */ int num_tracks);
	void tracksMoved(/* tracks? */ int num_tracks, int new_position);
	void playlistRenamed();
	void playlistStateChanged();
	void playlistUpdateInProgress(bool done);
	void playlistMetadataUpdated();
};

}

Q_DECLARE_METATYPE(Spotify::Playlist*)

#endif // SPOTIFY_PLAYLIST_HPP
