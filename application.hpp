#ifndef APPLICATION_HPP
#define APPLICATION_HPP

#include <QObject>
#include "logger_base.hpp"

class MPDListener;
class AudioOutput;
class ScriptEnvironment;
class ScriptListener;

namespace Spotify {
	class SpotifySession;
	class SpotifyPlayer;
}

class application : public QObject {
	Q_OBJECT

	logger local_logger;

	MPDListener* mpd_listener;

	AudioOutput* ao;
	Spotify::SpotifySession* session;
	Spotify::SpotifyPlayer* player;
	ScriptListener* scriptListener;

	ScriptEnvironment* terminal;

public:
	application(const logger&);
	~application();
};

#endif // APPLICATION_HPP
