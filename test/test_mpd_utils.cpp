#include <QtTest/QtTest>
#include <QVector>
#include <QString>
#include <QByteArray>
#include "../mpd_utils.hpp"

Q_DECLARE_METATYPE(QVector<QString>);

class TestMPDUtils: public QObject {
	Q_OBJECT

private slots:
	void splitting_data();
	void splitting();
};

void TestMPDUtils::splitting_data() {
	QTest::addColumn<QByteArray>("coded");
	QTest::addColumn<QVector<QString> >("decoded");

	QVector<QString> tmp;

	tmp.clear(); tmp << "hello";
	QTest::newRow("basic") << QByteArray("hello\n") << tmp;

	tmp.clear(); tmp << "Uncle" << "Leo!" << "Hello!";
	QTest::newRow("multiple") << QByteArray("Uncle Leo! Hello!\n") << tmp;
}

void TestMPDUtils::splitting() {
	QFETCH(QByteArray, coded);
	QFETCH(QVector<QString>, decoded);

	QCOMPARE(splitMessage(coded), decoded);
}

QTEST_MAIN(TestMPDUtils)
#include "test_mpd_utils.moc"
