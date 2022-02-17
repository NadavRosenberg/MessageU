#include "Profile.h"

#define FILENAME "my.info"

Profile::Profile() {
	if (std::filesystem::exists(FILENAME)) {
		std::ifstream file(FILENAME);
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
				private_key = segment;
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
	else {
		std::ofstream file(FILENAME);

		rsa_keys keys = Encryption::getRSAKeys();

		file << "\n\n" << keys.private_key << std::endl;

		file.close();
	}
}

bool Profile::isExist() {
	return std::filesystem::exists(FILENAME);
}