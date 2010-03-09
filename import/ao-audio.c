#include <stdlib.h>
#ifdef _WIN32
#include <windows.h>
#else
#include <pthread.h>
#endif

#include "audio.h"

#include <ao/ao.h>
#include <math.h>

ao_device* init_ao(audio_fifo_data_t *afd) {
    ao_sample_format fmt;
    fmt.byte_format = AO_FMT_LITTLE;
    fmt.rate = afd->rate;
    fmt.channels = afd->channels;
    fmt.bits = 16;

    int default_driver = ao_default_driver_id();
    ao_device *device = ao_open_live(default_driver, &fmt, NULL);
    if (device == NULL) {
        fprintf(stderr, "Error opening device.\n");
        return NULL;
    }
    return device;
}

static void* ao_audio_start(void *aux) {
    audio_fifo_t *af = aux;
    audio_fifo_data_t *afd;
    ao_device *output = NULL;
    int cur_channels = 0;
    int cur_rate = 0;

    for (;;) {
        pthread_mutex_lock(&af->mutex);
        while (!(afd = TAILQ_FIRST(&af->q))) {
            pthread_cond_wait(&af->cond, &af->mutex);
        }

        TAILQ_REMOVE(&af->q, afd, link);
        af->qlen -= afd->nsamples;

        pthread_mutex_unlock(&af->mutex);

        if (!output || cur_rate != afd->rate || cur_channels != afd->channels) {
            if (output) ao_close(output);

            cur_rate = afd->rate;
            cur_channels = afd->channels;

            output = init_ao(afd);
            fprintf(stderr, "Reinitializing AO\n");
        }
        fprintf(stderr, "Playing %lu samples.\n", sizeof(int16_t)*afd->nsamples*afd->channels);
        ao_play(output, (void*)afd->samples, sizeof(int16_t)*afd->nsamples*afd->channels);
        free(afd);
    }
    if (output) {
        fprintf(stderr, "Closing AO\n");
        ao_close(output);
    }
}



void audio_init(audio_fifo_t *af) {
#ifdef _WIN32
#else
        pthread_t tid;
#endif
        TAILQ_INIT(&af->q);
        af->qlen = 0;

#ifdef _WIN32
#else
        pthread_mutex_init(&af->mutex, NULL);
        pthread_cond_init(&af->cond, NULL);

        pthread_create(&tid, NULL, ao_audio_start, af);
#endif
        ao_initialize();
}

void audio_fifo_flush(audio_fifo_t *af) {
        audio_fifo_data_t *afd;

        pthread_mutex_lock(&af->mutex);

        while((afd = TAILQ_FIRST(&af->q))) {
                TAILQ_REMOVE(&af->q, afd, link);
                free(afd);
        }

        af->qlen = 0;
        pthread_mutex_unlock(&af->mutex);
}


