#include <iostream>
#include <iomanip>
#include <boost/date_time/local_time/local_time.hpp>
#include <boost/array.hpp>
#include "console_logger.hpp"
#include "log_message.hpp"
#include "ansi.hpp"

console_logger::console_logger(log_level_t max_log_level_) :
	max_log_level(max_log_level_)
{
	// This is pretty misplaced:
	using namespace boost::local_time;
	local_time_facet* lf(new local_time_facet("%Y-%m-%dT%H:%M:%S%F%Q"));
	std::cout.imbue(std::locale(std::cout.getloc(), lf));
}

console_logger::~console_logger() {
}

boost::local_time::local_date_time console_logger::timestamp() const {
	// The clock should be virtualized, to pave the way for testing
	using namespace boost::local_time;
	return local_sec_clock::local_time(time_zone_ptr());
}

void console_logger::output(const log_message& msg) const {
	std::ostream& out = std::cout;
	/* We should definitely colour the log output on all platforms,
	   including those we don't like...
	*/
	static boost::array<const char*, 5> colors = {{
			term::bg_red::value,   // 0-20
			term::fg_red::value,   // 20-40
			term::reset::value,    // ...
			term::fg_green::value,
			term::fg_blue::value,
		}};

	size_t index = std::min(
		static_cast<size_t>(msg.log_level) / 20,
		colors.size());
	out <<
		colors[index] << std::setw(2) << std::setfill('0') << static_cast<int>(msg.log_level) <<
		term::bright::value << term::fg_green::value << ' ' << msg.timestamp << ' ' <<
		term::reset::value << term::bright::value << '[' << term::reset::value;

	std::vector<std::string>::const_iterator begin = msg.address.begin(), end = msg.address.end(), i;
	for (i = begin; i != end; ++i) {
		if (i != begin) out << ", ";
		out << (*i);
	}

	out << term::bright::value << "] " << term::reset::value << msg.message << std::endl;
}

bool console_logger::should_log(log_level_t level) {
	return level <= max_log_level;
}

void console_logger::log(log_level_t level, const std::string& text) {
	log_message msg;
	msg.log_level = level;
	msg.timestamp = timestamp();
	msg.address = address;
	msg.message = text;

	output(msg);
}

logger console_logger::create_sublogger(const std::string& name) {
	boost::shared_ptr<console_logger> child(new console_logger(*this));
	child->address.push_back(name);
	return child;
}
