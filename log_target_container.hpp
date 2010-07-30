#ifndef LOG_TARGET_CONTAINER_HPP
#define LOG_TARGET_CONTAINER_HPP

#include <set>
#include <boost/shared_ptr.hpp>
#include "log_level.hpp"

class log_target_base;
typedef boost::shared_ptr<log_target_base> log_target;

struct log_message;

class log_target_container;
typedef boost::shared_ptr<log_target_container> log_target_container_ptr;

class log_target_container {
	std::set<log_target> targets;
	typedef std::set<log_target>::const_iterator t_it;
	log_level_t log_level;

	void recalculate_log_level();

public:
	log_target_container();
	~log_target_container();

	bool should_log(log_level_t) const;
	void log(const log_message&) const;

	void insert_target(const log_target&);
	void erase_target(const log_target&);
};

#endif // LOG_TARGET_CONTAINER_HPP
