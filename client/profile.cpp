#include "Profile.h"

#define MY_INFO "my.info"

Profile::Profile() {
	std::ifstream file(MY_INFO);
	std::string file_content;

	if (file.is_open()) {
		file >> file_content; // pipe file's content into stream

		try {
			std::vector<std::string> seglist;

			std::stringstream file_content_s(file_content);
			std::string segment;

			std::getline(file_content_s, segment, '\n');
			name = segment;

			std::getline(file_content_s, segment, '\n');
			uuid = segment;

			std::getline(file_content_s, segment, '\n');
			uuid = private_key;
		}
		catch (int num) {
			std::cerr << "Failed fetching user data!" << std::endl;
			exit(0);
		}
	}
	else {
		std::cerr << "Something happend while opening the file!" << std::endl;
	}
}