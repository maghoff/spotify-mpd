#ifndef APPLICATION_HPP
#define APPLICATION_HPP

#include <QObject>
#include <QTcpServer>
#include "logger_base.hpp"

class QIODevice;
class QString;
template <typename T> class QVector;
template <typename K, typename V> class QHash;

class application : public QObject {
	Q_OBJECT

	logger local_logger;

	QTcpServer server;
	QIODevice* io;

	typedef void (application::*mpd_func)(QVector<QString>);
	QHash<QString, mpd_func> MPDFunctions;

	enum command_list_status_t {
		no_command_list,
		command_list_starting,
		command_list,
		command_list_ok_starting,
		command_list_ok,
		command_list_failed
	};

	command_list_status_t command_list_status;
	int command_list_number;

public:
	application(const logger&);
	~application();

private slots:
	void newConnection();
	void readMe();

private:
	void processMessage(QVector<QString> msg);

	void mpd_command_list_begin(QVector<QString> msg);
	void mpd_command_list_ok_begin(QVector<QString> msg);
	void mpd_command_list_end(QVector<QString> msg);
	void mpd_currentsong(QVector<QString> msg);
	void mpd_status(QVector<QString> msg);
};

#endif // APPLICATION_HPP
