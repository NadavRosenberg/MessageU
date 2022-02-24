#include "profile.h"
#include "protocol.h"

void profile::fetchData() {
	if (std::filesystem::exists(PROFILE_FILENAME)) {
		std::ifstream file(PROFILE_FILENAME);
		std::string content;

		if (file.is_open()) {
			file >> content; // pipe file's content into stream

			try {
				std::stringstream content_s(content);
				std::string segment;

				std::getline(content_s, segment, '\n');
				name = segment;

				std::getline(content_s, segment, '\n');
				memcpy(&uuid, &segment, UUID_SIZE);

				std::getline(content_s, segment, '\n');
				private_key = segment;
			}
			catch (int num) {
				std::cerr << "Failed fetching user data!" << std::endl;
				exit_program();
			}
		}
		else {
			std::cerr << "Something happend while opening the file!" << std::endl;
			exit_program();
		}
	}
	else {
		std::ofstream file(PROFILE_FILENAME);

		rsa_keys keys = encryption::getRSAKeys();

		file << "\n\n" << keys.public_key << std::endl;

		file.close();
	}
}

void profile::setData(std::string _name, char* _uuid, std::string _private_key) {
	name = _name;
	memcpy(uuid, _uuid, 16);
	private_key = _private_key;

	std::ofstream file(PROFILE_FILENAME);
	std::string content;

	if (file.is_open()) {
		file << name << '\n';
		file << uuid << '\n';
		file << private_key;
	}
	else {
		std::cerr << "Something happend while opening the file!" << std::endl;
		exit_program();
	}
}

bool profile::isExist() {
	return std::filesystem::exists(PROFILE_FILENAME);
}

std::string profile::getName() {
	return name;
}

char* profile::getUuid() {
	return uuid;
}

std::string profile::getPrivateKey() {
	return private_key;
}

int profile::getVersion() {
	return VERSION;
}