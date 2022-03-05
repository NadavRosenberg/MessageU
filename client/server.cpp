#include "server.h"

server::server() {

	std::ifstream file(SERVER_FILENAME);
	std::string content;

	if (file.is_open()) {
		file >> content; // pipe file's content into stream
		file.close();

		std::stringstream file_content_s(content);
		std::string segment;

		std::getline(file_content_s, segment, ':');
		if (is_valid_host(segment)) {
			host = segment;
		}

		std::getline(file_content_s, segment, ':');
		if (is_valid_port(segment)) {
			port = stoi(segment);
		}

		if (host.size() == 0 && !port) {
			std::cerr << "Failed fetching host and port!" << std::endl;
			exit(0);
		}
	}
	else {
		std::cerr << "Failed to open " << SERVER_FILENAME << '\n';
		exit(0);
	}
}

int server::getPort() {
	return port;
}

std::string server::getHost() {
	return host;
}

bool server::is_valid_host(std::string s) {
	const char* p = s.c_str();

	while (*p != '\0')
	{
		if (isxdigit(*p) == 0)
		{
			if (ispunct(*p) == 0)
			{
				return 0;
			}
		}
		p++;
	}
	return 1;
}

bool server::is_valid_port(std::string s) {
	if (is_number(s) && is_in_range(stoi(s), MIN_PORT, MAX_PORT)) {
		return true;
	}
	return false;
}