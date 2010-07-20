#include "log_message.hpp"
#include "log_target_base.hpp"
#include "log_target_container.hpp"

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
