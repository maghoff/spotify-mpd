#ifndef SPOTIFYTRACK_HPP
#define SPOTIFYTRACK_HPP

class sp_track;
class SpotifyLink;

class SpotifyTrack {
	sp_track* p;

public:
	SpotifyTrack();
	SpotifyTrack(const SpotifyLink&);
	~SpotifyTrack();

	SpotifyTrack(const SpotifyTrack&);
	SpotifyTrack& operator = (const SpotifyTrack&);

	sp_track* get() const { return p; }
};

#endif // SPOTIFYTRACK_HPP
