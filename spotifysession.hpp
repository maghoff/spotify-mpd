#ifndef SPOTIFYSESSION_HPP
#define SPOTIFYSESSION_HPP

#include <QObject>
#include <QEvent>
#include <QTimer>
#include <libspotify/api.h>
#include "logger_base.hpp"

class AudioOutput;
class SpotifyTrack;

class SpotifySession : public QObject {
	Q_OBJECT

	logger local_logger, spotify_logger;

	QEvent::Type spotifyNotifyMainThreadEvent;

	sp_session *session;

	AudioOutput *ao;

	static void handle_notify_main_thread(sp_session*);
	static void handle_logged_in(sp_session*, sp_error);
	static void handle_logged_out(sp_session*);
	static void handle_metadata_updated(sp_session*);
	static void handle_play_token_lost(sp_session*);
	static void handle_connection_error(sp_session*, sp_error);
	static void handle_message_to_user(sp_session*, const char*);
	static void handle_log_message(sp_session*, const char*);
	static void handle_streaming_error(sp_session*, sp_error);
	static void handle_userinfo_updated(sp_session*);

	static int handle_music_delivery(sp_session*, const sp_audioformat *format, const void *frames, int num_frames);
	static void handle_end_of_track(sp_session*);

	QTimer spotifyProcessEventsTimer;

	void createSessionObject();

public:
	SpotifySession(QObject* parent, const logger&);
	~SpotifySession();

	bool event(QEvent*);
	void login(QString username, QString password);

	void setAudioOutput(AudioOutput*);

	void playerLoad(const SpotifyTrack&);
	void playerPlay(bool);

private slots:
	void spotifyNotifyMainThread();

signals:
	void loggedIn();
	void loggedOut();
	void metadataUpdated();
	void connectionError(/* error code */);
	void messageToUser(QString);
	void playTokenLost();
	void streamingError(/* error code */);
	void userinfoUpdated();
};

#endif // SPOTIFYSESSION_HPP
