#ifndef ALSAAUDIOOUTPUT_HPP
#define ALSAAUDIOOUTPUT_HPP

#include <pthread.h>
#include "audiooutput.hpp"
#include "logger_base.hpp"
#include "import/audio.h"

class AOAudioOutput : public AudioOutput {
	logger local_logger;

	audio_fifo_t audiofifo;
	bool playback_done;
	pthread_mutex_t notify_mutex;
	pthread_cond_t notify_cond;

public:
	AOAudioOutput(QObject* parent, const logger&);

	int musicDelivery(const MusicDeliveryData&);
	void endOfTrack();
};

#endif // ALSAAUDIOOUTPUT_HPP
