util = function() {
	var map = function(fn, list) {
		var result = [];
		for (var key in list) {
			result.push(fn(list[key], key));
		}
		return result;
	};

	var dir = function(obj) {
		return map(function(val, key) { return key; }, obj);
	};

	return {
		'map': map,
		'dir': dir
	};
}();


core = function() {
	var playlistByName = function(playlistcontainer, name) {
		var pc = playlistcontainer;
		if (pc === undefined || name === undefined) {
			print("Usage: core.playlistByName(playlistcontainer, name)");
			return;
		}

		for (var i=0; i<pc.numPlaylists(); ++i) {
			var pl = pc.playlist(i);
			if (pl.name() == name) return pl;
		}

		return null;
	};

	return {
		'playlistByName': playlistByName
	};
}();


watchPlaylistForContent = function(playlist, callback) {
	var pl = playlist;
	var log = function(msg) { application.log(50, msg); };

	var oldState = (pl.numTracks() != 0);
	var updateState = function() {
		var newState = (pl.numTracks() != 0);
		if (newState != oldState) callback(newState);
		oldState = newState;
	};

	pl.playlistStateChanged.connect(function() {
		log("playlistStateChanged()");
		updateState();
	});

	pl.playlistUpdateInProgress.connect(function(b) {
		log("playlistUpdateInProgress(" + b + ")");
		if (b) updateState();
	});
}

PlaylistPlayer = function(pl) {
	var app = application;
	var p = app.spotify.player;
	var isPlaying = false;

	var endOfTrack = function() {
		app.log(50, "PlaylistPlayer.endOfTrack()");
		pl.removeTracks([0]);
		isPlaying = false;
		app.spotify.endOfTrack.disconnect(endOfTrack);
		play();
	};

	var play = function() {
		app.log(50, "PlaylistPlayer.play()");
		if (isPlaying) return false;
		if (pl.numTracks() == 0) return false;

		app.log(50, "Playing track: " + pl.track(0).name());
		app.spotify.endOfTrack.connect(endOfTrack);
		p.playTrack(pl.track(0));
		isPlaying = true;
		return true;
	};

	return {
		"play": play,
		"isPlaying": function() { return isPlaying; }
	};
};


initBlackRiderSetup = function() {
	application.log(40, "initBlackRiderSetup");

	// Assume c is a CinemadControl-object
	var pc = app.spotify.playlistContainer();
	var pl = core.playlistByName(pc, "Black rider");

	var player = PlaylistPlayer(pl);

	watchPlaylistForContent(pl, c.setDesiredState);
	c.playbackReady.connect(function(b) { if (b) player.play(); });
};


// Console convenience:
app = application;
p = app.spotify.player;
t = terminate;
dir = util.dir;

app.spotify.loggedIn.connect(function() {
	pc = app.spotify.playlistContainer();
	//initBlackRiderSetup();
});
