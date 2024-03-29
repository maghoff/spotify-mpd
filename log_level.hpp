#ifndef LOG_LEVEL_HPP
#define LOG_LEVEL_HPP

namespace log_level {

enum log_level_t {

	NEVER = -1,

	BUG = 10,

	ERROR = 20,

	WARNING = 30,

	NOTICE = 40,

	STATE = 45, // Significant state changes that are part of normal operation

	OPERATION = 50,

	INPUT = 60,

	OUTPUT = 70,

	ANALYSIS = 80,

	TRACE = 90,

	ALL = 100

};

}

using log_level::log_level_t;

#endif // LOG_LEVEL_HPP
