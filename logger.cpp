#include <cassert>
#include <boost/date_time/local_time/local_time.hpp>
#include "log_message.hpp"
#include "log_target_container.hpp"
#include "logger.hpp"


namespace {

boost::local_time::local_date_time timestamp() {
	// The clock should be virtualized, to pave the way for testing
	using namespace boost::local_time;
	return local_sec_clock::local_time(time_zone_ptr());
}

}


logger::logger(const boost::shared_ptr<log_target_container>& targets_) :
	targets(targets_)
{
}

logger::logger(const logger& l, const std::string& name) {
	*this = l;
	address.push_back(name);
}

logger::~logger() {
}

bool logger::should_log(log_level_t level) const {
	return targets->should_log(level);
}

void logger::log(log_level_t level, const std::string& message) const {
	log_message msg;
	msg.log_level = level;
	msg.timestamp = timestamp();
	msg.address = address;
	msg.message = message;

	targets->log(msg);
}
