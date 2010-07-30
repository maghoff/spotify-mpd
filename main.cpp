#include <iostream>
#include <QtCore/QCoreApplication>
#include <QtCore/QSettings>
#include "ansi_log_target.hpp"
#include "application.hpp"
#include "log.hpp"
#include "log_target_container.hpp"
#include "logger.hpp"
#include "syslog_log_target.hpp"

int main(int argc, char *argv[]) {
	log_target_container_ptr log_targets(new log_target_container);
	log_targets->insert_target(log_target(new syslog_log_target("spotify-mpd")));
	logger local_logger(log_targets);

	LLOG(TRACE, "Welcome");

	QCoreApplication a(argc, argv);
	a.setOrganizationName("No name; Zapp Brannigan");
	a.setApplicationName("spotify-mpd");

	application app(local_logger, log_targets);

	return a.exec();
}
