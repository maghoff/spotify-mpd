#include <algorithm>
#include <cassert>
#include <stdexcept>
#include <QByteArray>
#include <QString>
#include <QVector>
#include "mpd_utils.hpp"

namespace {

QString parseUnquoted(char const * & pos, const char* end) {
	const char LWS[] = { ' ', '\t' };

	const char * field_begin = pos;
	const char * field_end = std::find_first_of(field_begin, end, LWS, LWS + sizeof(LWS));

	pos = field_end;

	// MPD messages are always in UTF8
	return QString::fromUtf8(field_begin, field_end - field_begin);
}

QString parseQuoted(char const * & pos, const char* end) {
	static const char special[] = { '"', '\\' };

	assert(*pos == '"');

	QByteArray buffer;

	for (const char *fragment_begin = pos + 1; ; ) {
		const char * fragment_end = std::find_first_of(fragment_begin, end, special, special + sizeof(special));

		if (fragment_end == end) {
			throw std::runtime_error("Missing terminating quotation mark in MPD message");
		}

		buffer.append(fragment_begin, fragment_end - fragment_begin);

		if (*fragment_end == '\\') {
			if (fragment_end + 1 == end) {
				throw std::runtime_error("Malformed escape sequence in MPD message");
			}

			buffer.append(*(fragment_end + 1));
			fragment_begin = fragment_end + 2;
		} else {
			assert(*fragment_end == '"');
			pos = fragment_end + 1;
			break;
		}
	}

	return QString::fromUtf8(buffer.data(), buffer.size());
}

}

QVector<QString> splitMessage(QByteArray in) {
	QVector<QString> result;

	// assert is OK here, since "in" is supposed to come from QIODevice::readLine()
	assert(in.size() >= 1);
	assert(in.at(in.size()-1) == '\n');
	in.chop(1); //< Remove the \n at the end

	const char* begin = in.data();
	const char* end = in.data() + in.size();

	for (const char* field_begin = begin, *field_end; ; field_begin = field_end + 1) {
		field_end = field_begin;

		QString field;
		if (*field_begin == '"') {
			field = parseQuoted(field_end, end);
		} else {
			field = parseUnquoted(field_end, end);
		}
		result.push_back(field);

		if (field_end == end) break;
	}

	return result;
}
