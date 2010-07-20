#ifndef APPLICATION_HPP
#define APPLICATION_HPP

#include <QObject>
#include "logger.hpp"

class MPDListener;
class AudioOutput;
class ScriptEnvironment;
class ScriptListener;

namespace Spotify {
	class Session;
	class Player;
}

class application : public QObject {
	Q_OBJECT

	logger local_logger;

	MPDListener* mpd_listener;

	AudioOutput* ao;
	Spotify::Session* session;
	Spotify::Player* player;
	ScriptListener* scriptListener;

	ScriptEnvironment* terminal;

public:
	application(const logger&);
	~application();
};

#endif // APPLICATION_HPP
