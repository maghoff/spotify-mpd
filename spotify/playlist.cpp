#include <cassert>
#include <libspotify/api.h>
#include "link.hpp"
#include "objectwrapper.hpp"
#include "playlist.hpp"
#include "track.hpp"

namespace Spotify {

#define INIT_CB \
	Playlist* pl = reinterpret_cast<Playlist*>(userdata); \
	assert(pl->get() == p);

void Playlist::handle_tracks_added(sp_playlist* p, sp_track * const * /* tracks */, int num_tracks, int position, void* userdata) {
	INIT_CB
	pl->tracksAdded(num_tracks, position);
}

void Playlist::handle_tracks_removed(sp_playlist *p, const int *tracks, int num_tracks, void *userdata) {
	INIT_CB
	//pl->tracksRemoved(QVector<int>(tracks, num_tracks));
}

void Playlist::handle_tracks_moved(sp_playlist *p, const int *tracks, int num_tracks, int new_position, void *userdata) {
	INIT_CB
	//pl->tracksMoved(QVector<int>(tracks, num_tracks), new_position);
}

void Playlist::handle_playlist_renamed(sp_playlist *p, void *userdata) {
	INIT_CB
	pl->playlistRenamed();
}

void Playlist::handle_playlist_state_changed(sp_playlist *p, void *userdata) {
	INIT_CB
	pl->playlistStateChanged();
}

void Playlist::handle_playlist_update_in_progress(sp_playlist *p, bool done, void *userdata) {
	INIT_CB
	pl->playlistUpdateInProgress(done);
}

void Playlist::handle_playlist_metadata_updated(sp_playlist *p, void *userdata) {
	INIT_CB
	pl->playlistMetadataUpdated();
}

Playlist::Playlist(sp_playlist* p_) : p(p_) {
	/* Do not add a ref. Implicit transfer of one ref */

	assert(p);

	sp_playlist_callbacks tmp_cb = {
		&handle_tracks_added,
		&handle_tracks_removed,
		&handle_tracks_moved,
		&handle_playlist_renamed,
		&handle_playlist_state_changed,
		&handle_playlist_update_in_progress,
		&handle_playlist_metadata_updated
	};

	cb.reset(new sp_playlist_callbacks(tmp_cb));

	sp_playlist_add_callbacks(p, cb.get(), (void*)this);
}

Playlist::~Playlist() {
	sp_playlist_remove_callbacks(p, cb.get(), (void*)this);
	sp_playlist_release(p);
}

bool Playlist::isLoaded() const {
	return sp_playlist_is_loaded(p);
}

int Playlist::numTracks() const {
	return sp_playlist_num_tracks(p);
}

Track* Playlist::track(int t) const {
	sp_track* sp_t = sp_playlist_track(p, t);
	assert(sp_t);
	sp_track_add_ref(sp_t);
	return new Track(sp_t);
}

QString Playlist::name() const {
	return QString::fromUtf8(sp_playlist_name(p));
}

bool Playlist::isCollaborative() const {
	return sp_playlist_is_collaborative(p);
}

void Playlist::setCollaborative(bool b) const {
	sp_playlist_set_collaborative(p, b);
}

bool Playlist::hasPendingChanges() const {
	return sp_playlist_has_pending_changes(p);
}

void Playlist::removeTracks(QVector<int> tracks) {
	sp_error retval = sp_playlist_remove_tracks(p, tracks.data(), tracks.size());
}

}
