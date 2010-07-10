#ifndef PLAYLISTCONTAINER_HPP
#define PLAYLISTCONTAINER_HPP

#include <QMetaType>
#include <QObject>

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

signals:

public slots:

};

} // namespace Spotify

Q_DECLARE_METATYPE(Spotify::PlaylistContainer*)

#endif // PLAYLISTCONTAINER_HPP
