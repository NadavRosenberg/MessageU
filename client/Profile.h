#pragma once
#include <string>
#include <iostream>
#include <fstream>
#include <vector>
#include <sstream>
#include <filesystem>
#include "utils.h"
#include "global.h"

#define VERSION 1
#define PROFILE_FILENAME "my.info"

class profile
{
private:
	std::string name;
	char uuid[UUID_SIZE + 1];
	std::string private_key;
	std::string public_key;
	void saveToFile();
public:
	void fetchData();
	void setData(std::string, char*, std::string);
	void setKeys(std::string, std::string);
	std::string getName();
	char* getUuid();
	std::string getPublicKey();
	std::string getPrivateKey();
	static int getVersion();
	bool isUserRegistered();
};

