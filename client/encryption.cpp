#include "encryption.h"
#include "protocol.h"

rsa_keys encryption::getRSAKeys()
{
	/////////////////////////////////////////
	//// Generate Parameters
	//CryptoPP::AutoSeededRandomPool rng;

	//CryptoPP::InvertibleRSAFunction params;
	//params.GenerateRandomWithKeySize(rng, 3072);

	/////////////////////////////////////////
	//// Create Keys
	//CryptoPP::RSA::PrivateKey privateKey(params);
	//CryptoPP::RSA::PublicKey publicKey(params);

	std::string publicKey = "MIGeMA0GCSqGSIb3DQEBAQUAA4GMADCBiAKBgH859vc0hABAkjPpa70pcu80ktZJ\
Sm8xsDBSvLAoBhH5mrFjfhpJpnItK8Gqpz+T47wA+Ddebl6ylg5GJGOA82hUPvm0\
UBMx57n/yIwiPz4tf2oQU5xSpdPbo6Y2XeH87m1q05vEe9bGaYCBY8755emckj7N\
6Iiy37NTr042GTUHAgMBAAE=";

	std::string privateKey = "MIICWwIBAAKBgH859vc0hABAkjPpa70pcu80ktZJSm8xsDBSvLAoBhH5mrFjfhpJ\
pnItK8Gqpz+T47wA+Ddebl6ylg5GJGOA82hUPvm0UBMx57n/yIwiPz4tf2oQU5xS\
pdPbo6Y2XeH87m1q05vEe9bGaYCBY8755emckj7N6Iiy37NTr042GTUHAgMBAAEC\
gYB9e69hLPPQx98rwlsYTIxm8ASvw8PdnjV3w/MFD8TE79oqgcX7qY64aGMS9f1V\
kMj6Qj6PZd0YG64yz+3OEn5/kR9Ec0bYM00nLJyfZyWX6lXKhXI43WzIA99UfK14\
PHvHi6IJ09ir5L4DHFqEuPhKAfNhatm+OIiyeh/+tuFJmQJBALfpcUNs1g75uBKF\
jzCc4IG+sjI1j2ZSsFmFxDrMni9zeBZsXT4a6sUbshjy3i64j1UlGAmpBd0/BrrJ\
xdMMcAUCQQCxGHHSFYLO6D9EbZ1Jse0hfpE5em1KVulJdgVz173VG6trAuBiozB1\
v+piGRXhpA02Jjnh/5fMz93x/DgJo3qbAkBrc1abgDEomrnzCFl6m42IAHR4ZVac\
97MzxyxBe+95YbaAwIPUQatltrdPLiXL3l+06dRrZEhjLtP0qdkATZJAkBO4s2o\
wE5BbnDoQU+kCS7REbkM+nz+dVJ7l6f8Hyj/adDk3SYoqfqnKPL1+aiml1YPPd3g\
zuLGp/18klY/7ONrAkEAmkD8btz3VMRlmdIendlmvgv7c+tdUSuYWKaI85ae/WC8\
B38TyxuRj0Y8CLs9BRWaLnFJHMU6qVhpnMRpbs2ktg==";

	rsa_keys keys = rsa_keys();
	keys.public_key = publicKey;
	keys.private_key = privateKey;

	return keys;
}

std::string encryption::getEncryptedSymmetricKey() {
	return "ASFGVSS";
}