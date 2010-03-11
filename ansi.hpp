#ifndef ANSI_HPP
#define ANSI_HPP

#include <boost/static_assert.hpp>

namespace term {

template<unsigned Value, unsigned Base>
struct display {
	BOOST_STATIC_ASSERT(Value < 10);
	BOOST_STATIC_ASSERT(Base < 10 || Base == 30 || Base == 40);
	static const char value[6];
};

template<unsigned Value, unsigned Base>
const char display<Value, Base>::value[6] = {'\033', '[', '0'+(Base/10), '0'+Value, 'm', 0};

template<unsigned V1, unsigned V2, unsigned B>
const char * operator|(const display<V1, B>& lhs, const display<V2, B>& rhs) {
	return display<V1|V2, B>::value;
}

typedef const display<0, 0> reset;
typedef const display<0, 1> bright;
typedef const display<0, 2> dim;
typedef const display<0, 4> underscore;
typedef const display<0, 5> blink;
typedef const display<0, 7> reverse;
typedef const display<0, 8> hidden;

typedef const display<0, 30> fg_black;
typedef const display<1, 30> fg_red;
typedef const display<2, 30> fg_green;
typedef const display<3, 30> fg_blue;

typedef const display<0, 40> bg_black;
typedef const display<1, 40> bg_red;
typedef const display<2, 40> bg_green;
typedef const display<3, 40> bg_blue;

}

#endif // ANSI_HPP

