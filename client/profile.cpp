#include "profile.h"
#include "protocol.h"
#include "Base64Wrapper.h"

void profile::fetchData() {
	if (std::filesystem::exists(PROFILE_FILENAME)) {
		std::ifstream file(PROFILE_FILENAME);
		std::string content;

		if (file.is_open()) {
			std::stringstream content_s;
			content_s << file.rdbuf();

			try {
				std::string segment;

				std::getline(content_s, segment, '\n');
				if (!segment.empty())
					name = segment;

				std::getline(content_s, segment, '\n');
				if (!segment.empty())
					memcpy(uuid, segment.c_str(), UUID_SIZE);

				std::getline(content_s, segment, '\xff'); // until EOF
				if (!segment.empty())
					private_key = Base64Wrapper::decode(segment);

				file.close();
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
		file.close();
	}
}

void profile::setData(std::string _name, char* _uuid, std::string _private_key) {
	name = _name;
	memcpy(uuid, _uuid, UUID_SIZE + 1);
	private_key = _private_key;

	saveToFile();
}

std::string profile::getName() {
	return name;
}

char* profile::getUuid() {
	return uuid;
}

int profile::getVersion() {
	return VERSION;
}

void profile::setKeys(std::string _public_key, std::string _private_key)
{
	public_key = _public_key;
	private_key = _private_key;
	saveToFile();
}

std::string profile::getPublicKey()
{
	return public_key;
}

std::string profile::getPrivateKey()
{
	return private_key;
}

void profile::saveToFile()
{
	std::ofstream file(PROFILE_FILENAME);
	std::string content;

	if (file.is_open()) {
		std::string pkey = Base64Wrapper::encode(private_key);

		file << name << '\n';
		file << uuid << '\n';
		file << pkey;

		file.close();
	}
	else {
		std::cerr << "Something happend while opening the file!" << std::endl;
		exit_program();
	}
}

bool profile::isUserRegistered() {
	return name.length() > 0;
}