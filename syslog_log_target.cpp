#include <syslog.h>
#include <sstream>
#include "log_message.hpp"
#include "syslog_log_target.hpp"

syslog_log_target::syslog_log_target(const std::string& ident) :
	log_target_base(log_level::ANALYSIS)
{
	// Should translate from UTF-8 to the encoding openlog expects:
	openlog(ident.c_str(), LOG_PID, LOG_DAEMON);
}

syslog_log_target::~syslog_log_target() {
	closelog();
}

void syslog_log_target::log(const log_message& msg) const {
	int syslog_level;

	const log_level_t& level = msg.log_level;
	if (level >= log_level::INPUT) syslog_level = LOG_DEBUG;
	else if (level >= log_level::STATE) syslog_level = LOG_INFO;
	else if (level >= log_level::NOTICE) syslog_level = LOG_NOTICE;
	else if (level >= log_level::WARNING) syslog_level = LOG_WARNING;
	else if (level >= log_level::ERROR) syslog_level = LOG_ERR;
	else syslog_level = LOG_ALERT;

	// syslog adds its own timestamp

	std::ostringstream out;
	out << std::setw(2) << std::setfill('0') << static_cast<int>(msg.log_level) << " [";

	std::vector<std::string>::const_iterator begin = msg.address.begin(), end = msg.address.end(), i;
	for (i = begin; i != end; ++i) {
		if (i != begin) out << ", ";
		out << (*i);
	}

	out << "] " << msg.message << std::flush;

	syslog(syslog_level, "%s", out.str().c_str());
}
