#include <QString>
#include "mpdlistener.hpp"
#include "application.hpp"
#include "log.hpp"

application::application(const logger& local_logger_) :
	local_logger(local_logger_),
	mpd_listener(this, local_logger_->create_sublogger("MPDListener"))
{
}

application::~application() {
}
