#include <spotify/api.h>
#include "aoaudiooutput.hpp"
#include "import/audio.h"
#include "musicdeliverydata.hpp"
#include "log.hpp"

AOAudioOutput::AOAudioOutput(QObject* parent, const logger& local_logger_) :
	AudioOutput(parent),
	local_logger(local_logger_),
	playback_done(false)
{
	LLOG(TRACE, __FUNCTION__);

	audio_init(&audiofifo);

	pthread_mutex_init(&notify_mutex, NULL);
	pthread_cond_init(&notify_cond, NULL);
}

// This function is adapted from the similar one in the jukebox-example bundled with libspotify
int AOAudioOutput::musicDelivery(const MusicDeliveryData& d) {
//	LLOG(TRACE, __FUNCTION__);

	audio_fifo_t *af = &audiofifo;
	audio_fifo_data_t *afd;
	size_t s;

	if (d.num_frames == 0)
		return 0; // Audio discontinuity, do nothing

	pthread_mutex_lock(&af->mutex);

	/* Buffer one second of audio */
	if (af->qlen > d.format->sample_rate) {
		pthread_mutex_unlock(&af->mutex);

		return 0;
	}

	s = d.num_frames * sizeof(int16_t) * d.format->channels;

	afd = reinterpret_cast<audio_fifo_data_t*>(malloc(sizeof(audio_fifo_data_t) + s));
	memcpy(afd->samples, d.frames, s);

	afd->nsamples = d.num_frames;

	afd->rate = d.format->sample_rate;
	afd->channels = d.format->channels;

	TAILQ_INSERT_TAIL(&af->q, afd, link);
	af->qlen += d.num_frames;

	pthread_cond_signal(&af->cond);
	pthread_mutex_unlock(&af->mutex);

	return d.num_frames;
}

// This function is adapted from the similar one in the jukebox-example bundled with libspotify
void AOAudioOutput::endOfTrack() {
//	LLOG(TRACE, __FUNCTION__);

	pthread_mutex_lock(&notify_mutex);
	playback_done = true;
	pthread_cond_signal(&notify_cond);
	pthread_mutex_unlock(&notify_mutex);
}
