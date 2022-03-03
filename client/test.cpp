#include <iostream>
#include "AESWrapper.h"
#include "RSAWrapper.h"
#include "test.h"
#include "global.h"
#include "message.h"

int sendMessage()
{
	// create symmetric key
	unsigned char* buffer = new unsigned char[SYMMETRIC_KEY_LENGTH + 1]{ 0 };
	AESWrapper::GenerateKey(buffer, (unsigned int)SYMMETRIC_KEY_LENGTH);
	buffer[SYMMETRIC_KEY_LENGTH] = '\0';
	std::string symm_key(buffer, buffer + SYMMETRIC_KEY_LENGTH);

	// create the message
	std::string msgContent = "hello world!";
	message msg("AAAABBBBCCCCDDDD", '\3', msgContent.length(), msgContent);

	// encrypt the message using symmetric key
	AESWrapper aes(reinterpret_cast<const unsigned char*>(symm_key.c_str()), symm_key.length());
	std::string ciper_symmetric = aes.encrypt(msg.getContent().c_str(), msg.getContentSize());

	// decrypt the message using symmetric key
	AESWrapper aes2(reinterpret_cast<const unsigned char*>(symm_key.c_str()), symm_key.length());
	std::string plain_symmetric = aes.decrypt(ciper_symmetric.c_str(), ciper_symmetric.length());

	printf("sendMessage:\n");
	printf("plain:           %s\n", msg.getContent().c_str());
	printf("ciper:           %s\n", ciper_symmetric.c_str());
	printf("plain recovered: %s\n", plain_symmetric.c_str());

	return 0;
}

int shareSymmetric()
{
	// 1 sending to 2
	RSAPrivateWrapper pw1 = RSAPrivateWrapper();
	std::string public_key = pw1.getPublicKey();
	std::string private_key = pw1.getPrivateKey();

	RSAPrivateWrapper pw2;
	std::string public_key2 = pw2.getPublicKey();
	std::string private_key2 = pw2.getPrivateKey();

	// create symmetric key
	unsigned char* buffer = new unsigned char[SYMMETRIC_KEY_LENGTH + 1]{ 0 };
	AESWrapper::GenerateKey(buffer, (unsigned int)SYMMETRIC_KEY_LENGTH);
	buffer[SYMMETRIC_KEY_LENGTH] = '\0';
	std::string symm_key(reinterpret_cast<char*>(buffer));

	// create the message
	message msg("AAAABBBBCCCCDDDD", '\2', SYMMETRIC_KEY_LENGTH, symm_key);

	// source encrypt symmetric key using destination's public key
	RSAPublicWrapper public_w(public_key2);
	std::string plain = msg.getContent();
	std::string ciper = public_w.encrypt(plain);

	// destination decrypt the message using his private key
	RSAPrivateWrapper pw(private_key2);
	std::string plain_recovered = pw.decrypt(ciper);

	printf("shareSymmetric:\n");
	printf("plain:         %s\n", plain.c_str());
	printf("ciper:         %s\n", ciper.c_str());
	printf("plain covered: %s\n", plain_recovered.c_str());

	return 0;
}
