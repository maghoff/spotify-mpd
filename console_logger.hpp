#ifndef CONSOLE_LOGGER_HPP
#define CONSOLE_LOGGER_HPP

#include <string>
#include <boost/date_time/local_time/local_time.hpp>
#include "logger_base.hpp"

struct log_message;

class console_logger : public logger_base {
	log_level_t max_log_level;
	std::vector<std::string> address;

	boost::local_time::local_date_time timestamp() const;
	void output(const log_message&) const;

public:
	console_logger(log_level_t max_log_level);
	~console_logger();

	bool should_log(log_level_t);
	void log(log_level_t, const std::string&);
};

#endif // CONSOLE_LOGGER_HPP
