#ifndef SPOTIFYLINK_HPP
#define SPOTIFYLINK_HPP

#include <QMetaType>
#include <QUrl>

class sp_link;

namespace Spotify {

class Link : public QObject {
	Q_OBJECT

	sp_link* p;

public:
	Link();
	Link(sp_link*);
	Link(QUrl);
	~Link();

	Link(const Link&);
	Link& operator = (const Link&);

	sp_link* get() const { return p; }
};

}

Q_DECLARE_METATYPE(Spotify::Link*)

#endif // SPOTIFYLINK_HPP
