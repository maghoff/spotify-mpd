#include <cassert>
#include <iostream>
#include <sstream>
#include <stdexcept>
#include <string>
#include <QCoreApplication>
#include <QStringList>
#include <libspotify/api.h>
#include "audiooutput.hpp"
#include "appkey.h"
#include "log.hpp"
#include "spotifysession.hpp"
#include "spotifytrack.hpp"
#include "musicdeliverydata.hpp"

namespace Spotify {

namespace {

std::string utf8_str(QString s) {
	QByteArray a = s.toUtf8();
	return std::string(a.data(), a.data() + a.size());
}

SpotifySession* userdata(sp_session* session) {
	return reinterpret_cast<SpotifySession*>(sp_session_userdata(session));
}

}

SpotifySession::SpotifySession(
	QObject* parent,
	const logger& local_logger_
) :
	QObject(parent),
	local_logger(local_logger_),
	spotify_logger(local_logger_->create_sublogger("libspotify")),
	session(0),
	ao(0)
{
	spotifyNotifyMainThreadEvent = static_cast<QEvent::Type>(QEvent::registerEventType());
	spotifyProcessEventsTimer.setSingleShot(true);
	connect(&spotifyProcessEventsTimer, SIGNAL(timeout()), this, SLOT(spotifyNotifyMainThread()));

	createSessionObject();
}

SpotifySession::~SpotifySession() {
	// Would delete the sp_session-object, but it is not allowed by the API
}

bool SpotifySession::event(QEvent* e) {
	if (e->type() == spotifyNotifyMainThreadEvent) {
		spotifyNotifyMainThread();
		return true;
	}

	return QObject::event(e);
}

void SpotifySession::spotifyNotifyMainThread() {
	int timeout = -1;
	sp_session_process_events(session, &timeout);
	spotifyProcessEventsTimer.start(timeout);
}

void SpotifySession::createSessionObject() {
	sp_session_config config;

	// Always do this. It allows libspotify to check for
	// header/library inconsistencies.
	config.api_version = SPOTIFY_API_VERSION;

	// The path of the directory to store the cache. This must be specified.
	// Please read the documentation on preferred values.
	config.cache_location = "tmp";

	// The path of the directory to store the settings. This must be specified.
	// Please read the documentation on preferred values.
	config.settings_location = "tmp";

	// The key of the application. They are generated by Spotify,
	// and are specific to each application using libspotify.
	config.application_key = g_appkey;
	config.application_key_size = g_appkey_size;

	// This identifies the application using some
	// free-text string [1, 255] characters.
	std::string appname = utf8_str(QCoreApplication::applicationName());
	config.user_agent = appname.c_str();

	config.userdata = reinterpret_cast<void*>(this);

	// Register the callbacks.
	static sp_session_callbacks callbacks = {
		&handle_logged_in,
		&handle_logged_out,
		&handle_metadata_updated,
		&handle_connection_error,
		&handle_message_to_user,
		&handle_notify_main_thread,
		&handle_music_delivery,
		&handle_play_token_lost,
		&handle_log_message,
		&handle_end_of_track,
		&handle_streaming_error,
		&handle_userinfo_updated
	};

	config.callbacks = &callbacks;

	sp_error error = sp_session_init(&config, &session);

	if (error != SP_ERROR_OK) {
		std::ostringstream ss;
		ss << "failed to create session: " << sp_error_message(error);
		throw std::runtime_error(ss.str());
	}
}

void SpotifySession::login(QString username, QString password) {
	sp_error error = sp_session_login(session, utf8_str(username).c_str(), utf8_str(password).c_str());

	if (error != SP_ERROR_OK) {
		std::ostringstream ss;
		ss << "failed to login: " << sp_error_message(error);
		throw std::runtime_error(ss.str());
	}
}

void SpotifySession::setAudioOutput(AudioOutput* ao_) {
	ao = ao_;
}

void SpotifySession::playerLoad(const SpotifyTrack& track) {
	sp_error error = sp_session_player_load(session, track.get());

	if (error != SP_ERROR_OK) {
		std::ostringstream ss;
		ss << "sp_session_player_load: " << sp_error_message(error);
		throw std::runtime_error(ss.str());
	}
}

void SpotifySession::playerPlay(bool play) {
	sp_error error = sp_session_player_play(session, play);

	if (error != SP_ERROR_OK) {
		std::ostringstream ss;
		ss << "sp_session_player_play: " << sp_error_message(error);
		throw std::runtime_error(ss.str());
	}
}

// Logger macro for static members:
#define STLOG(level, msg) SLOG(userdata(session)->local_logger, level, msg)

// These events happen on a different thread than main_thread:
void SpotifySession::handle_notify_main_thread(sp_session* session) {
	STLOG(TRACE, __FUNCTION__);

	SpotifySession* self = userdata(session);

	QCoreApplication::postEvent(
		self,
		new QEvent(self->spotifyNotifyMainThreadEvent)
	);
}

// These events happen on the main thread:
void SpotifySession::handle_logged_in(sp_session* session, sp_error error) {
	STLOG(TRACE, __FUNCTION__);

	if (error != SP_ERROR_OK) {
		std::ostringstream ss;
		ss << "Failed to log in to spotify: " << sp_error_message(error);
		STLOG(ERROR, ss.str());

		// This exception propagates to the Qt event loop. This is not supported
		// by Qt, so we should probably consider doing something else.
		throw std::runtime_error(ss.str());
	}

	userdata(session)->loggedIn();
}

void SpotifySession::handle_logged_out(sp_session* session) {
	STLOG(TRACE, __FUNCTION__);
	userdata(session)->loggedOut();
}

void SpotifySession::handle_metadata_updated(sp_session* session) {
	STLOG(TRACE, __FUNCTION__);
	userdata(session)->metadataUpdated();
}

void SpotifySession::handle_play_token_lost(sp_session* session) {
	STLOG(TRACE, __FUNCTION__);
	userdata(session)->playTokenLost();
}

void SpotifySession::handle_connection_error(sp_session* session, sp_error error) {
	STLOG(ERROR, __FUNCTION__ << ": " << sp_error_message(error));
	userdata(session)->connectionError();
}

void SpotifySession::handle_message_to_user(sp_session* session, const char* msg) {
	STLOG(NOTICE, __FUNCTION__ << ": " << msg);
	userdata(session)->messageToUser(QString::fromUtf8(msg));
}

void SpotifySession::handle_log_message(sp_session* session, const char* data) {
	logger& local_logger = userdata(session)->spotify_logger;

	if (!local_logger->should_log(log_level::OPERATION)) return;

	QStringList msgs = QString::fromUtf8(data).split('\n', QString::SkipEmptyParts);
	foreach (QString msg, msgs) {
		LLOG(OPERATION, utf8_str(msg));
	}
}

int SpotifySession::handle_music_delivery(sp_session* session, const sp_audioformat *format, const void *frames, int num_frames) {
//	STLOG(TRACE, __FUNCTION__);

	MusicDeliveryData d;

	d.session = session;
	d.format = format;
	d.frames = frames;
	d.num_frames = num_frames;

	return userdata(session)->ao->musicDelivery(d);
}

void SpotifySession::handle_end_of_track(sp_session* session) {
//	STLOG(TRACE, __FUNCTION__);

	userdata(session)->ao->endOfTrack();
}

void SpotifySession::handle_streaming_error(sp_session* session, sp_error error) {
	STLOG(ERROR, __FUNCTION__ << ": " << sp_error_message(error));
	userdata(session)->streamingError();
}

void SpotifySession::handle_userinfo_updated(sp_session* session) {
	STLOG(TRACE, __FUNCTION__);
	userdata(session)->userinfoUpdated();
}

}
