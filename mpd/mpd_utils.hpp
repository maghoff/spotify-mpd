#ifndef MPD_UTILS_HPP
#define MPD_UTILS_HPP

class QString;
class QByteArray;
template <typename T> class QVector;

QVector<QString> splitMessage(QByteArray in);

#endif // MPD_UTILS_HPP
