#ifndef MUSICDELIVERYDATA_HPP
#define MUSICDELIVERYDATA_HPP

struct sp_session;
struct sp_audioformat;

struct MusicDeliveryData {
	sp_session *session;
	const sp_audioformat *format;
	const void *frames;
	int num_frames;
};

#endif // MUSICDELIVERYDATA_HPP
