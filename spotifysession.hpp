#ifndef SPOTIFYSESSION_HPP
#define SPOTIFYSESSION_HPP

#include <QObject>
#include <QEvent>
#include <QTimer>
#include <spotify/api.h>

class SpotifySession : public QObject {
	Q_OBJECT

	QEvent::Type spotifyNotifyMainThreadEvent;

	QString username, password;
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

public:
	SpotifySession(QObject* parent, QString username, QString password);
	~SpotifySession();

	bool event(QEvent*);

public slots:
	void connect();

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
