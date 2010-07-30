#ifndef IODEVICESINK_HPP
#define IODEVICESINK_HPP

#include <ios>
#include <boost/iostreams/categories.hpp>

class QIODevice;

// You probably want to use this class through a IODevice_ostream
class IODeviceSink {
	QIODevice* sink;

public:
	typedef char char_type;
	typedef boost::iostreams::sink_tag category;

	IODeviceSink(QIODevice* sink);

	std::streamsize write(const char* buffer, std::streamsize);
};

#endif // IODEVICESINK_HPP
