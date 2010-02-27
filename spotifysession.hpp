#ifndef SPOTIFYSESSION_HPP
#define SPOTIFYSESSION_HPP

#include <QObject>

struct sp_session;

class SpotifySession : public QObject {
	Q_OBJECT

	QString username, password;
	sp_session *session;

public:
	SpotifySession(QObject* parent = 0);
	SpotifySession(QObject* parent, QString username, QString password);
	~SpotifySession();

public slots:
	void connect();

signals:
	void connected();
	void disconnected();
};

#endif // SPOTIFYSESSION_HPP
