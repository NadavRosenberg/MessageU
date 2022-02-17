#pragma once
#include <cryptopp/rsa.h>
#include <iostream>

struct rsa_keys {
	std::string public_key;
	std::string private_key;
};

class Encryption
{
public:
	static rsa_keys getRSAKeys();
};