#include "users.h"
#include "global.h"

void users::add_user(std::string uuid, std::string name, std::string public_key, std::string symmetric_key)
{
	user u;
	u.uuid = uuid;
	u.name = name;
	u.symmetric_key = symmetric_key;
	u.public_key = public_key;
	_users.push_back(u);
}

void users::add_user(std::string uuid, std::string name, std::string public_key)
{
	user u;
	u.uuid = uuid;
	u.name = name;
	u.public_key = public_key;
	_users.push_back(u);
}

void users::add_user(std::string uuid, std::string name)
{
	if (!is_exist(uuid)) // is not exist
	{
		user u;
		u.uuid = uuid;
		u.name = name;
		u.public_key = "";
		_users.push_back(u);
	}
}

void users::add_users(user clients[], int amount)
{
	for (int i = 0; i < amount; i++)
	{
		_users.push_back(clients[i]);
	}
}

std::string users::get_user_id(std::string name)
{
	std::string padded_name = name + std::string(NAME_LENGTH - std::min(NAME_LENGTH, (int)name.length()), '\0');

	for (int i = 0; i < _users.size(); i++)
		if (padded_name.compare(_users[i].name) == 0)
		{
			return _users[i].uuid;
		}
	return "";
}

std::string users::get_user_public_key(std::string uuid)
{
	for (int i = 0; i < _users.size(); i++)
		if (uuid.compare(_users[i].uuid) == 0)
		{
			return _users[i].public_key;
		}
	return "";
}

std::string users::get_user_symm_key(std::string uuid)
{
	for (int i = 0; i < _users.size(); i++)
		if (uuid.compare(_users[i].uuid) == 0)
		{
			return _users[i].symmetric_key;
		}
	return "";
}

void users::set_user_symm_key(std::string uuid, std::string symm_key)
{
	//std::string padded_name = name + std::string(NAME_LENGTH - std::min(NAME_LENGTH, (int)name.length()), '\0');

	for (int i = 0; i < _users.size(); i++)
		if (uuid.compare(_users[i].uuid) == 0)
		{
			_users[i].symmetric_key = symm_key;
			return;
		}
}

void users::set_user_public_key(std::string uuid, std::string public_key)
{
	for (int i = 0; i < _users.size(); i++)
		if (uuid.compare(_users[i].uuid) == 0)
		{
			_users[i].public_key = public_key;
			return;
		}
}

bool users::is_exist(std::string uuid)
{
	for (int i = 0; i < _users.size(); i++)
		if (uuid.compare(_users[i].uuid) == 0)
		{
			return true;
		}
	return false;
}

void users::print()
{
	std::cout << "Users:" << std::endl;
	for (int i=0; i<_users.size(); i++)
		printf("# %s (%s)\n", _users[i].name.c_str(), _users[i].uuid.c_str());
}
