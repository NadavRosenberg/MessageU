#pragma once
#include <cryptopp/osrng.h>
#include <cryptopp/rsa.h>
#include <iostream>

struct rsa_keys {
	std::string public_key;
	std::string private_key;
};

class encryption
{
public:
	static rsa_keys getRSAKeys();
	static std::string encryption::getEncryptedSymmetricKey();
};