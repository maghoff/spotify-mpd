#ifndef APPLICATION_HPP
#define APPLICATION_HPP

#include <QObject>
#include "mpdlistener.hpp"
#include "logger_base.hpp"

class application : public QObject {
	Q_OBJECT

	logger local_logger;

	MPDListener mpd_listener;

public:
	application(const logger&);
	~application();
};

#endif // APPLICATION_HPP
