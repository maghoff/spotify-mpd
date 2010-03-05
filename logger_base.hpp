#ifndef LOGGER_BASE_HPP
#define LOGGER_BASE_HPP

#include <ostream>
#include <boost/shared_ptr.hpp>
#include "log_level.hpp"

class logger_base {
public:
	~logger_base();

	virtual bool should_log(log_level_t) = 0;
	virtual void log(log_level_t, const std::string&) = 0;
};

typedef boost::shared_ptr<logger_base> logger;

#endif // LOGGER_BASE_HPP
