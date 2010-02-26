#include <iostream>
#include <sstream>
#include <stdexcept>
#include <QtTest/QtTest>
#include <QVector>
#include <QString>
#include <QByteArray>
#include "../mpd_utils.hpp"

Q_DECLARE_METATYPE(QVector<QString>);

namespace QTest {
	template<>
	char *toString(const QVector<QString> &msg) {
		std::ostringstream ss;
		ss << "[";
		for (QVector<QString>::const_iterator i = msg.begin(); i != msg.end(); ++i) {
			if (i != msg.begin()) ss << ", ";
			ss << '"' << (*i).toUtf8().data() << '"';
		}
		ss << ']';
		return qstrdup(ss.str().c_str());
	}
}


class TestMPDUtils: public QObject {
	Q_OBJECT

private slots:
	void splitting_data();
	void splitting();

	void splittingFail_data();
	void splittingFail();
};

void TestMPDUtils::splitting_data() {
	QTest::addColumn<QByteArray>("coded");
	QTest::addColumn<QVector<QString> >("decoded");

	QVector<QString> tmp;

	tmp.clear(); tmp << "hello";
	QTest::newRow("basic") << QByteArray("hello\n") << tmp;

	tmp.clear(); tmp << "Uncle" << "Leo!" << "Hello!";
	QTest::newRow("multiple") << QByteArray("Uncle Leo! Hello!\n") << tmp;

	tmp.clear(); tmp << "play" << "music file.ogg";
	QTest::newRow("simple quoted") << QByteArray("play \"music file.ogg\"\n") << tmp;

	tmp.clear(); tmp << "play" << "music\\file\".ogg";
	QTest::newRow("complex quoted") << QByteArray("play \"music\\\\file\\\".ogg\"\n") << tmp;

	// It is unclear to me how this protocol handles fields with newline characters.
	// From rudimentary testing it seems that they are allowed directly inside quoted
	// fields. Which is a shame, because it ruins our line buffering.
}

void TestMPDUtils::splitting() {
	QFETCH(QByteArray, coded);
	QFETCH(QVector<QString>, decoded);

	QCOMPARE(splitMessage(coded), decoded);
}

void TestMPDUtils::splittingFail_data() {
	QTest::addColumn<QByteArray>("coded");
	QTest::newRow("open quoted") << QByteArray("play \"music\n");
	QTest::newRow("open escape") << QByteArray("play \"music\\\n");
}

void TestMPDUtils::splittingFail() {
	QFETCH(QByteArray, coded);
	try {
		QVector<QString> decoded = splitMessage(coded);
		QFAIL("Expected exception was not thrown");
	}
	catch (const std::runtime_error&) {
		// pass
	}
}

QTEST_MAIN(TestMPDUtils)
#include "test_mpd_utils.moc"
