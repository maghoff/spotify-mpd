#ifndef LOG_TARGET_BASE_HPP
#define LOG_TARGET_BASE_HPP

#include "log_level.hpp"

struct log_message;

class log_target_base {
public:
	log_target_base(log_level_t);
	virtual ~log_target_base();

	virtual void log(const log_message&) const = 0;

	log_level_t log_level;
};

#endif // LOG_TARGET_BASE_HPP
