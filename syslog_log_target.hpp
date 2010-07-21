#ifndef SYSLOG_LOG_TARGET_HPP
#define SYSLOG_LOG_TARGET_HPP

#include <string>
#include <vector>
#include "log_target_base.hpp"

class syslog_log_target : public log_target_base {
	std::vector<char> ident;

public:
	syslog_log_target(const std::string& ident);
	~syslog_log_target();

	void log(const log_message&) const;
};

#endif // SYSLOG_LOG_TARGET_HPP
