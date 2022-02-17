#pragma once
#include <string>
#include <iostream>
#include <fstream>
#include <string>
#include <vector>
#include <sstream>
#include <filesystem>
#include "encryption.h"

class Profile
{
private:
	std::string name;
	std::string uuid;
	std::string private_key;
	std::string public_key; // save only in ram
public:
	Profile();
	bool isExist();
};

