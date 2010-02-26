#include <algorithm>
#include <cassert>
#include <QByteArray>
#include <QString>
#include <QVector>
#include "mpd_utils.hpp"

static const char LWS[] = { ' ', '\t' };

QVector<QString> splitMessage(QByteArray in) {
	QVector<QString> result;

	// assert is OK here, since "in" is supposed to come from QIODevice::readLine()
	assert(in.size() >= 1);
	assert(in.at(in.size()-1) == '\n');
	in.chop(1); //< Remove the \n at the end

	const char* begin = in.data();
	const char* end = in.data() + in.size();

	for (const char* field_begin = begin, *field_end; ; field_begin = field_end + 1) {
		field_end = std::find_first_of(field_begin, end, LWS, LWS + sizeof(LWS));

		// MPD messages are always in UTF8
		result.push_back(QString::fromUtf8(field_begin, field_end - field_begin));

		if (field_end == end) break;
	}

	return result;
}

