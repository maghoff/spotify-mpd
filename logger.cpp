#include <cassert>
#include <boost/date_time/local_time/local_time.hpp>
#include "log_message.hpp"
#include "logger.hpp"


namespace {

boost::local_time::local_date_time timestamp() {
	// The clock should be virtualized, to pave the way for testing
	using namespace boost::local_time;
	return local_sec_clock::local_time(time_zone_ptr());
}

}


log_target_base::log_target_base(log_level_t log_level_) :
	log_level(log_level_)
{
}

log_target_base::~log_target_base() {
}


log_target_container::log_target_container() :
	log_level(log_level::NEVER)
{
}

log_target_container::~log_target_container() {
}

void log_target_container::recalculate_log_level() {
	log_level = log_level::NEVER;

	// The set should perhaps be sorted by loglevel, to
	// make it possible to break out of this iteration:

	for (t_it i = targets.begin(), end = targets.end(); i != end; ++i) {
		if ((*i)->log_level > log_level) log_level = (*i)->log_level;
	}
}

bool log_target_container::should_log(log_level_t requested_level) const {
	return requested_level <= log_level;
}

void log_target_container::log(const log_message& msg) const {
	for (t_it i = targets.begin(), end = targets.end(); i != end; ++i) {
		if (msg.log_level <= (*i)->log_level) (*i)->log(msg);
	}
}

void log_target_container::insert_target(const log_target& target) {
	if (target->log_level > log_level) log_level = target->log_level;
	targets.insert(target);
}

void log_target_container::erase_target(const log_target& target) {
	targets.erase(target);
	assert(target->log_level <= log_level);
	if (target->log_level == log_level) recalculate_log_level();
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
