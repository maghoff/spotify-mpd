#ifndef AUDIOOUTPUT_HPP
#define AUDIOOUTPUT_HPP

#include <QObject>

struct MusicDeliveryData;

// Classes implementing this interface must take into consideration some
// thread safety issues, noted on each function.
class AudioOutput : public QObject {
	Q_OBJECT

public:
	// This class is constructed and destructed on the GUI thread.
	AudioOutput(QObject* parent);

	// The following two functions are called from a thread maintained by libspotify.
	// The only guarantee is that they are called from the same thread.
	virtual int musicDelivery(const MusicDeliveryData&) = 0;
	virtual void endOfTrack() = 0;
};

#endif // AUDIOOUTPUT_HPP
