app = application;
p = app.spotify.player;
t = terminate;

app.spotify.loggedIn.connect(function() {
	pc = app.spotify.playlistContainer();
});


jukebox = function(playlist_no) {

	if (playlist_no < 0 || playlist_no >= pc.numPlaylists()) {
		print("playlist_no out of range");
		return;
	}

	if (pc === undefined) {
		print("pc is undefined. Are you logged in?");
		return;
	}

	var pl = pc.playlist(playlist_no);
	var current_track = 0;
	var playing = false;

	var tryPlay = function() {
		if (current_track < pl.numTracks()) {
			p.playTrack(pl.track(current_track));
			playing = true;
		} else {
			playing = false;
		}
	};

	app.spotify.endOfTrack.connect(function() {
		current_track += 1;
		tryPlay();
	});

	pl.tracksAdded.connect(function(num_tracks, position) {
		if (playing) {
			if (position <= current_track) current_track += num_tracks;
		} else {
			tryPlay();
		};
	});

	tryPlay();
};
