#ifndef LOG_MESSAGE_HPP
#define LOG_MESSAGE_HPP

#include <string>
#include <vector>
#include <boost/date_time/local_time/local_time.hpp>
#include "log_level.hpp"

struct log_message {
	log_level_t log_level;
	boost::local_time::local_date_time timestamp;
	std::vector<std::string> address;
	std::string message;

	log_message();
};

#endif // LOG_MESSAGE_HPP
