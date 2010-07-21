#include <iomanip>
#include <boost/array.hpp>
#include "ansi.hpp"
#include "ansi_log_target.hpp"
#include "log_message.hpp"

ansi_log_target::ansi_log_target(std::ostream& out_, log_level_t level) :
	log_target_base(level),
	out(out_)
{
	using namespace boost::local_time;
	local_time_facet* lf(new local_time_facet("%Y-%m-%dT%H:%M:%S%F%Q"));
	std::cout.imbue(std::locale(std::cout.getloc(), lf));
}

ansi_log_target::~ansi_log_target() {
}

void ansi_log_target::log(const log_message& msg) const {
	/* We should definitely colour the log output on all platforms,
	   including those we don't like. Perhaps in another log_target.
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
		colors.size()
	);

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
