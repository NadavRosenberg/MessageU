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
public:
	void add_user(std::string, std::string, std::string, std::string);
	void add_user(std::string, std::string, std::string);
	void add_user(std::string, std::string);
	void add_users(user[], int);
	std::string get_user_id(std::string);
	void print();
};

