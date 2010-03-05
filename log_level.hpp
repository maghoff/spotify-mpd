#ifndef LOG_LEVEL_HPP
#define LOG_LEVEL_HPP

namespace log_level {

enum log_level_t {

	BUG = 10,

	ERROR = 20,

	WARNING = 30,

	NOTICE = 40,

	OPERATION = 70,

	ANALYSIS = 80,

	TRACE = 90,

	ALL = 100

};

}

using log_level::log_level_t;

#endif // LOG_LEVEL_HPP
