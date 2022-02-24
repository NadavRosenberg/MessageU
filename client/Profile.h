#pragma once
#include <string>
#include <iostream>
#include <fstream>
#include <string>
#include <vector>
#include <sstream>
#include <filesystem>
#include "encryption.h"
#include "utils.h"
#include "global.h"

#define VERSION 1
#define PROFILE_FILENAME "my.info"

class profile
{
private:
	std::string name;
	char uuid[16];
	std::string private_key;
public:
	void fetchData();
	void profile::setData(std::string, char*, std::string);
	bool isExist();
	std::string getName();
	char* getUuid();
	std::string getPrivateKey();
	static int getVersion();
};

