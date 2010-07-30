#include <cassert>
#include <QIODevice>
#include "iodevicesink.hpp"

IODeviceSink::IODeviceSink(QIODevice* sink_) :
	sink(sink_)
{
	assert(sink);
}

std::streamsize IODeviceSink::write(const char* buffer, std::streamsize n) {
	return sink->write(buffer, n);
}
