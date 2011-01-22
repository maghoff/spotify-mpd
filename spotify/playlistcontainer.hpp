#ifndef PLAYLISTCONTAINER_HPP
#define PLAYLISTCONTAINER_HPP

#include <QMetaType>
#include <QObject>
#include "playlist.hpp"

struct sp_playlistcontainer;

namespace Spotify {

class PlaylistContainer : public QObject {
	Q_OBJECT

	sp_playlistcontainer* p;

public:
	// sp_playlistcontainers are not reference counted, as far as I can tell.
	// @pre p != 0
	PlaylistContainer(sp_playlistcontainer* p);
	~PlaylistContainer();

	Q_INVOKABLE int numPlaylists() const;
	Q_INVOKABLE Spotify::Playlist* playlist(int) const;

	Q_INVOKABLE Spotify::Playlist* addPlaylist(const Spotify::Link*) const;

signals:
/*
void(* 	playlist_added )(sp_playlistcontainer *pc, sp_playlist *playlist, int position, void *userdata)
void(* 	playlist_removed )(sp_playlistcontainer *pc, sp_playlist *playlist, int position, void *userdata)
void(* 	playlist_moved )(sp_playlistcontainer *pc, sp_playlist *playlist, int position, int new_position, void *userdata)
void(* 	container_loaded )(sp_playlistcontainer *pc, void *userdata)
*/

public slots:

};

/*
void 	sp_playlistcontainer_add_callbacks (sp_playlistcontainer *pc, sp_playlistcontainer_callbacks *callbacks, void *userdata)
void 	sp_playlistcontainer_remove_callbacks (sp_playlistcontainer *pc, sp_playlistcontainer_callbacks *callbacks, void *userdata)

sp_playlist * 	sp_playlistcontainer_add_new_playlist (sp_playlistcontainer *pc, const char *name)
sp_playlist * 	sp_playlistcontainer_add_playlist (sp_playlistcontainer *pc, sp_link *link)

sp_error 	sp_playlistcontainer_remove_playlist (sp_playlistcontainer *pc, int index)
sp_error 	sp_playlistcontainer_move_playlist (sp_playlistcontainer *pc, int index, int new_position)
*/

} // namespace Spotify

Q_DECLARE_METATYPE(Spotify::PlaylistContainer*)

#endif // PLAYLISTCONTAINER_HPP
