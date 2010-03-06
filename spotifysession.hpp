#ifndef SPOTIFYSESSION_HPP
#define SPOTIFYSESSION_HPP

#include <QObject>
#include <QEvent>
#include <QTimer>
#include <spotify/api.h>
#include "logger_base.hpp"

class SpotifySession : public QObject {
	Q_OBJECT

	logger local_logger;

	QEvent::Type spotifyNotifyMainThreadEvent;

	sp_session *session;

	static void handle_notify_main_thread(sp_session*);
	static void handle_logged_in(sp_session*, sp_error);
	static void handle_logged_out(sp_session*);
	static void handle_metadata_updated(sp_session*);
	static void handle_play_token_lost(sp_session*);
	static void handle_connection_error(sp_session*, sp_error);
	static void handle_message_to_user(sp_session*, const char*);
	static void handle_log_message(sp_session*, const char*);

	QTimer spotifyProcessEventsTimer;

	void createSessionObject();

public:
	SpotifySession(QObject* parent, const logger&);
	~SpotifySession();

	bool event(QEvent*);
	void login(QString username, QString password);

private slots:
	void spotifyNotifyMainThread();

signals:
	void loggedIn();
	void loggedOut();
	void metadataUpdated();
	void connectionError(/* error code */);
	void messageToUser(QString);
	void playTokenLost();
	void logMessage(QString);

	/* Not handled:
	int music_delivery(...);
	void end_of_track();
	*/
};

#endif // SPOTIFYSESSION_HPP
