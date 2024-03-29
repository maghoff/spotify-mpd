#include <cassert>
#include <iostream>
#include <sstream>
#include <stdexcept>
#include <string>
#include <QCoreApplication>
#include <QThread>
#include <QStringList>
#include <libspotify/api.h>
#include "audiooutput.hpp"
#include "appkey.h"
#include "link.hpp"
#include "log.hpp"
#include "playlist.hpp"
#include "playlistcontainer.hpp"
#include "session.hpp"
#include "track.hpp"
#include "musicdeliverydata.hpp"

namespace Spotify {

namespace {

std::string utf8_str(QString s) {
	QByteArray a = s.toUtf8();
	return std::string(a.data(), a.data() + a.size());
}

Session* userdata(sp_session* session) {
	return reinterpret_cast<Session*>(sp_session_userdata(session));
}

}

Session::Session(
	QObject* parent,
	const logger& local_logger_
) :
	QObject(parent),
	local_logger(local_logger_),
	spotify_logger(logger(local_logger_, "libspotify")),
	session(0),
	ao(0)
{
	spotifyNotifyMainThreadEvent = static_cast<QEvent::Type>(QEvent::registerEventType());
	spotifyProcessEventsTimer.setSingleShot(true);
	connect(&spotifyProcessEventsTimer, SIGNAL(timeout()), this, SLOT(spotifyNotifyMainThread()));

	spotifyProcessAlwaysTimer.setSingleShot(false);
	connect(&spotifyProcessAlwaysTimer, SIGNAL(timeout()), this, SLOT(spotifyNotifyMainThread()));
	spotifyProcessAlwaysTimer.start(1000);

	createSessionObject();
}

Session::~Session() {
	// Would delete the sp_session-object, but it is not allowed by the API
}

bool Session::event(QEvent* e) {
	if (e->type() == spotifyNotifyMainThreadEvent) {
		spotifyNotifyMainThread();
		return true;
	}

	return QObject::event(e);
}

void Session::spotifyNotifyMainThread() {
	int timeout = -1;
	do {
		sp_session_process_events(session, &timeout);
	} while (timeout == 0);
	spotifyProcessEventsTimer.start(timeout);
}

void Session::createSessionObject() {
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

	sp_error error = sp_session_create(&config, &session);

	if (error != SP_ERROR_OK) {
		std::ostringstream ss;
		ss << "failed to create session: " << sp_error_message(error);
		throw std::runtime_error(ss.str());
	}
}

void Session::login(QString username, QString password) {
	sp_error error = SP_ERROR_OK; sp_session_login(session, utf8_str(username).c_str(), utf8_str(password).c_str());

	if (error != SP_ERROR_OK) {
		std::ostringstream ss;
		ss << "failed to login: " << sp_error_message(error);
		throw std::runtime_error(ss.str());
	}
}

void Session::setAudioOutput(AudioOutput* ao_) {
	ao = ao_;
}

void Session::playerLoad(const Track& track) {
	sp_error error = sp_session_player_load(session, track.get());

	if (error != SP_ERROR_OK) {
		std::ostringstream ss;
		ss << "sp_session_player_load: " << sp_error_message(error);
		throw std::runtime_error(ss.str());
	}
}

void Session::playerPlay(bool play) {
	sp_error error = SP_ERROR_OK; sp_session_player_play(session, play);

	if (error != SP_ERROR_OK) {
		std::ostringstream ss;
		ss << "sp_session_player_play: " << sp_error_message(error);
		throw std::runtime_error(ss.str());
	}
}

PlaylistContainer* Session::playlistContainer() {
	spotifyProcessEventsTimer.start(0);

	sp_playlistcontainer* p = sp_session_playlistcontainer(session);
	if (!p) return 0;

	PlaylistContainer* pc = new PlaylistContainer(p);
	pc->setParent(this);
	return pc;
}

Spotify::Playlist* Session::playlistCreate(Spotify::Link* l) {
	spotifyProcessEventsTimer.start(0);

	LLOG(ERROR, "Link type: " << sp_link_type(l->get()));
	sp_playlist* pl = sp_playlist_create(session, l->get());
	LLOG(ERROR, "sp_playlist: " << pl);
	if (!pl) return 0;
	return new Playlist(pl);
}

// Logger macro for static members:
#define STLOG(level, msg) SLOG(userdata(session)->local_logger, level, msg)

// These events happen on a different thread than main_thread:
void Session::handle_notify_main_thread(sp_session* session) {
	STLOG(TRACE, __FUNCTION__);

	Session* self = userdata(session);

	QCoreApplication::postEvent(
		self,
		new QEvent(self->spotifyNotifyMainThreadEvent)
	);
}

// These events happen on the main thread:
void Session::handle_logged_in(sp_session* session, sp_error error) {
	STLOG(TRACE, __FUNCTION__);

	if (error != SP_ERROR_OK) {
		STLOG(ERROR, "Failed to log in to spotify: " << sp_error_message(error));
		// The failure to log in should be reported by an event
	} else {
		userdata(session)->loggedIn();
	}
}

void Session::handle_logged_out(sp_session* session) {
	STLOG(TRACE, __FUNCTION__);
	userdata(session)->loggedOut();
}

void Session::handle_metadata_updated(sp_session* session) {
	STLOG(TRACE, __FUNCTION__);
	userdata(session)->metadataUpdated();
}

void Session::handle_play_token_lost(sp_session* session) {
	STLOG(TRACE, __FUNCTION__);
	userdata(session)->playTokenLost();
}

void Session::handle_connection_error(sp_session* session, sp_error error) {
	STLOG(ERROR, __FUNCTION__ << ": " << sp_error_message(error));
	userdata(session)->connectionError();
}

void Session::handle_message_to_user(sp_session* session, const char* msg) {
	STLOG(NOTICE, __FUNCTION__ << ": " << msg);
	userdata(session)->messageToUser(QString::fromUtf8(msg));
}

void Session::handle_log_message(sp_session* session, const char* data) {
	logger& local_logger = userdata(session)->spotify_logger;

	if (!local_logger.should_log(log_level::OPERATION)) return;

	QStringList msgs = QString::fromUtf8(data).split('\n', QString::SkipEmptyParts);
	foreach (QString msg, msgs) {
		LLOG(OPERATION, utf8_str(msg));
	}
}

int Session::handle_music_delivery(sp_session* session, const sp_audioformat *format, const void *frames, int num_frames) {
//	STLOG(TRACE, __FUNCTION__);

	MusicDeliveryData d;

	d.session = session;
	d.format = format;
	d.frames = frames;
	d.num_frames = num_frames;

	return userdata(session)->ao->musicDelivery(d);
}

void Session::handle_end_of_track(sp_session* session) {
//	STLOG(TRACE, __FUNCTION__);

	userdata(session)->ao->endOfTrack();

	// Emitting a signal on the wrong thread is OK, because the connection
	// type is "auto" by default, which means that if the signal is emitted
	// on a different thread from the slot's event loop, the signal will be
	// posted as an event instead.

	userdata(session)->endOfTrack();
}

void Session::handle_streaming_error(sp_session* session, sp_error error) {
	STLOG(ERROR, __FUNCTION__ << ": " << sp_error_message(error));
	userdata(session)->streamingError();
}

void Session::handle_userinfo_updated(sp_session* session) {
	STLOG(TRACE, __FUNCTION__);
	userdata(session)->userinfoUpdated();
}

}
