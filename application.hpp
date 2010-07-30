#ifndef APPLICATION_HPP
#define APPLICATION_HPP

#include <boost/shared_ptr.hpp>
#include <QObject>
#include "logger.hpp"

class log_target_container;
typedef boost::shared_ptr<log_target_container> log_target_container_ptr;

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
	application(const logger&, const log_target_container_ptr&);
	~application();

public slots:
	void log(int /* log_level_t */, QString);
};

#endif // APPLICATION_HPP
