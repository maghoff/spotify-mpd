#ifndef LOGGER_HPP
#define LOGGER_HPP

#include <set>
#include <string>
#include <vector>
#include <boost/shared_ptr.hpp>
#include "log_level.hpp"


class log_target_base;
typedef boost::shared_ptr<log_target_base> log_target;

struct log_message;


class log_target_base {
public:
	log_target_base(log_level_t);
	virtual ~log_target_base();

	virtual void log(const log_message&) const = 0;

	log_level_t log_level;
};

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

class logger {
	boost::shared_ptr<log_target_container> targets;
	std::vector<std::string> address;

public:
	logger(const boost::shared_ptr<log_target_container>&);
	logger(const logger&, const std::string& name);
	~logger();

	bool should_log(log_level_t level) const;
	void log(log_level_t level, const std::string& message) const;
};


#endif // LOGGER_BASE_HPP
