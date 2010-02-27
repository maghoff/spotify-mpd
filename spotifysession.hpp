#ifndef SPOTIFYSESSION_HPP
#define SPOTIFYSESSION_HPP

#include <QObject>

struct sp_session;

struct SpotifyCallbacks {
	virtual void sp_loggedIn() = 0;
	virtual void sp_loggedOut() = 0;
	virtual void sp_metadataUpdated() = 0;
	virtual void sp_connectionError(/* error code */) = 0;
	virtual void sp_messageToUser(QString) = 0;
	virtual void sp_playTokenLost() = 0;
	virtual void sp_logMessage(QString) = 0;
};

class SpotifySession : public QObject, SpotifyCallbacks {
	Q_OBJECT

	QString username, password;
	sp_session *session;

	void sp_loggedIn();
	void sp_loggedOut();
	void sp_metadataUpdated();
	void sp_connectionError(/* error code */);
	void sp_messageToUser(QString);
	void sp_playTokenLost();
	void sp_logMessage(QString);

public:
	SpotifySession(QObject* parent, QString username, QString password);
	~SpotifySession();

	bool event(QEvent*);

public slots:
	void connect();

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
