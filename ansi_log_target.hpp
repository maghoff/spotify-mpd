#ifndef ANSI_LOG_TARGET_HPP
#define ANSI_LOG_TARGET_HPP

#include <ostream>
#include "log_target_base.hpp"

class ansi_log_target : public log_target_base {
	std::ostream& out;

public:
	ansi_log_target(std::ostream& out, log_level_t);
	~ansi_log_target();

	void log(const log_message&) const;
};

#endif // ANSI_LOG_TARGET_HPP
