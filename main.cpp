#include <QtCore/QCoreApplication>
#include "application.hpp"

int main(int argc, char *argv[]) {
	QCoreApplication a(argc, argv);

	application app;

	return a.exec();
}
