#ifndef SPOTIFYOBJECTWRAPPER_HPP
#define SPOTIFYOBJECTWRAPPER_HPP

#define SPOTIFY_OBJECT_WRAPPER(Type, type) \
	Spotify##Type::Spotify##Type() : p(0) { \
	} \
	\
	Spotify##Type::Spotify##Type(sp_##type* p_) : p(p_) { \
		/* Do not add a ref. Implicit transfer of one ref */ \
	} \
	\
	Spotify##Type::~Spotify##Type() { \
		if (p) sp_##type##_release(p); \
	} \
	\
	Spotify##Type::Spotify##Type(const Spotify##Type& rhs) : QObject(rhs.parent()), p(rhs.p) { \
		sp_##type##_add_ref(p); \
	} \
	\
	Spotify##Type& Spotify##Type::operator = (const Spotify##Type& rhs) { \
		if (p) sp_##type##_release(p); \
		p = rhs.p; \
		sp_##type##_add_ref(p); \
		return *this; \
	}

#endif // SPOTIFYOBJECTWRAPPER_HPP
