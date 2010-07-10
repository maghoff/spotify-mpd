#ifndef APPLICATION_HPP
#define APPLICATION_HPP

#include <QObject>
#include "logger_base.hpp"

class MPDListener;
class SpotifySession;
class SpotifyPlayer;
class AudioOutput;
class ScriptEnvironment;
class ScriptListener;

class application : public QObject {
	Q_OBJECT

	logger local_logger;

	MPDListener* mpd_listener;

	AudioOutput* ao;
	SpotifySession* session;
	SpotifyPlayer* player;
	ScriptListener* scriptListener;

	ScriptEnvironment* terminal;

public:
	application(const logger&);
	~application();
};

#endif // APPLICATION_HPP
