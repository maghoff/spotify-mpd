#ifndef QLOG_HPP
#define QLOG_HPP

#include <QByteArray>
#include <QTextStream>
#include "log_level.hpp"

#define QSLOG(logger, level, msg) \
	if (logger->should_log(log_level::level)) do { \
		QByteArray data; \
		{ \
			/* QTextStream chooses the default locale for the system */ \
			QTextStream ts(&data); \
			ts << msg; \
		} \
		logger->log(log_level::level, std::string(data.begin(), data.end())); \
	} while(false)

#define QLOG(level, msg) QSLOG(local_logger, level, msg)

#endif // QLOG_HPP
