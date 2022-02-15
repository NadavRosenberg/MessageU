#include "port.h"

Port::Port() {
	port = 0;

	std::string content;
	std::ifstream portfile;
	portfile.open(PORT_INFO_FILENAME);
	if (!portfile.fail()) {
		getline(portfile, content);
		portfile.close();

		if (is_valid_port(content)) {
			port = stoi(content);
		}
	}
	else {
		std::cerr << "Failed to open " << PORT_INFO_FILENAME << '\n';
		exit(0);
	}
}

int Port::getPort() {
	return port;
}
