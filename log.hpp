#ifndef LOG_HPP
#define LOG_HPP

#include <sstream>
#include "log_level.hpp"

#define SLOG(logger, level, message) \
	if (logger.should_log(log_level::level)) do { \
		std::ostringstream ss; \
		ss << message; \
		logger.log(log_level::level, ss.str()); \
	} while (false)

#define LLOG(level, message) SLOG(local_logger, level, message)

#endif // LOG_HPP
