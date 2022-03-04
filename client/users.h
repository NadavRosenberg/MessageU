#pragma once
#include <iostream>
#include <vector>

struct user
{
	std::string uuid;
	std::string name;
	std::string public_key;
	std::string symmetric_key;
};

class users
{
	std::vector<user> _users;
	bool is_exist(std::string);
public:
	void add_user(std::string, std::string);
	std::string get_user_id(std::string);
	std::string get_username(std::string);
	std::string get_user_public_key(std::string);
	std::string get_user_symm_key(std::string);
	void set_user_symm_key(std::string, std::string);
	void set_user_public_key(std::string, std::string);
	void print();
};

