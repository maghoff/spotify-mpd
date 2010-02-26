#include <iostream>
#include "application.hpp"

application::application() {
	server.listen(QHostAddress::Any, 6601);
	connect(&server, SIGNAL(newConnection()), this, SLOT(newConnection()));
}

application::~application() {
}

void application::newConnection() {
	std::cout << "New connection" << std::endl;
}

