#ifndef LOGGER_HPP
#define LOGGER_HPP

#include <string>
#include <vector>
#include <boost/shared_ptr.hpp>
#include "log_level.hpp"

class log_target_container;

class logger {
	boost::shared_ptr<log_target_container> targets;
	std::vector<std::string> address;

public:
	logger(const boost::shared_ptr<log_target_container>&);
	logger(const logger&, const std::string& name);
	~logger();

	bool should_log(log_level_t level) const;
	void log(log_level_t level, const std::string& message) const;
};

#endif // LOGGER_HPP
