#include <QtTest/QtTest>
#include <QVector>
#include <QString>
#include <QByteArray>
#include "../mpd_utils.hpp"

class TestMPDUtils: public QObject {
	Q_OBJECT

private slots:
	void splitting();
};

void TestMPDUtils::splitting() {
	QByteArray coded("hello joe\n");
	QVector<QString> decoded;
	decoded << "hello" << "joe";

	QCOMPARE(splitMessage(coded), decoded);
}

QTEST_MAIN(TestMPDUtils)
#include "test_mpd_utils.moc"
