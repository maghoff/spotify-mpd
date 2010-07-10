#ifndef SPOTIFYOBJECTWRAPPER_HPP
#define SPOTIFYOBJECTWRAPPER_HPP

#define SPOTIFY_OBJECT_WRAPPER(Type, type) \
	Type::Type() : p(0) { \
	} \
	\
	Type::Type(sp_##type* p_) : p(p_) { \
		/* Do not add a ref. Implicit transfer of one ref */ \
	} \
	\
	Type::~Type() { \
		if (p) sp_##type##_release(p); \
	} \
	\
	Type::Type(const Type& rhs) : QObject(rhs.parent()), p(rhs.p) { \
		sp_##type##_add_ref(p); \
	} \
	\
	Type& Type::operator = (const Type& rhs) { \
		if (p) sp_##type##_release(p); \
		p = rhs.p; \
		sp_##type##_add_ref(p); \
		return *this; \
	}

#endif // SPOTIFYOBJECTWRAPPER_HPP
