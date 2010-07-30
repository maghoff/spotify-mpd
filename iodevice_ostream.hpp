#ifndef IODEVICE_OSTREAM_HPP
#define IODEVICE_OSTREAM_HPP

#include <boost/iostreams/stream.hpp>
#include "iodevicesink.hpp"

typedef boost::iostreams::stream<IODeviceSink> IODevice_ostream;

#endif // IODEVICE_OSTREAM_HPP
