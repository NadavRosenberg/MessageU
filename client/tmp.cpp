#include <WinSock2.h>
#include <Windows.h>
#pragma comment(lib, "ws2_32.lib")

#include <iostream>
#include <fstream>
#include <string>
#include <thread>
#include <vector>
#include "response.h"
#include "request.h"

#define MAX_REQUEST_SIZE 1024
#define MAX_RESPONSE_SIZE 1024
#define VERSION '1'

union REQUEST {
	Request r;
	char buf[MAX_REQUEST_SIZE] = { 0 };
};

union RESPONSE {
	Response r;
	char buf[MAX_RESPONSE_SIZE] = { 0 };
};



std::string gen_random(const int len) {
	static const char alphanum[] =
		"0123456789"
		"ABCDEFGHIJKLMNOPQRSTUVWXYZ"
		"abcdefghijklmnopqrstuvwxyz";
	std::string tmp_s;
	tmp_s.reserve(len);

	for (int i = 0; i < len; ++i) {
		tmp_s += alphanum[rand() % (sizeof(alphanum) - 1)];
	}

	return tmp_s;
}

void send210(SOCKET clientsocket, REQUEST req, std::string payload) {
	RESPONSE res;
	res.r.h.version = VERSION;
	res.r.h.status = 210;
	res.r.h.name_len = req.r.h.name_len;
	memcpy(&res.r.h.filename, &req.r.h.filename, req.r.h.name_len);
	auto sizeOffset = sizeof(char) + 2 * sizeof(uint16_t) + req.r.h.name_len;
	res.buf[sizeOffset] = payload.length();
	memcpy(&res.buf[sizeOffset + sizeof(uint32_t)], payload.c_str(), payload.length());

	send(clientsocket, res.buf, sizeof(res.buf), 0);
}

void send211(SOCKET clientsocket, REQUEST req, std::string filename, std::string payload) {
	RESPONSE res;
	res.r.h.version = VERSION;
	res.r.h.status = 211;
	res.r.h.name_len = filename.size();
	memcpy(&res.r.h.filename, filename.c_str(), filename.size());
	auto sizeOffset = sizeof(char) + 2 * sizeof(uint16_t) + filename.size();
	res.buf[sizeOffset] = payload.length();
	memcpy(&res.buf[sizeOffset + sizeof(uint32_t)], payload.c_str(), payload.length());

	send(clientsocket, res.buf, sizeof(res.buf), 0);
}

void send212(SOCKET clientsocket, REQUEST req) {
	RESPONSE res;
	res.r.h.version = VERSION;
	res.r.h.status = 212;
	res.r.h.name_len = req.r.h.name_len;
	memcpy(&res.r.h.filename, &req.r.h.filename, req.r.h.name_len);

	send(clientsocket, res.buf, sizeof(res.buf), 0);
}

void send1001(SOCKET clientsocket, REQUEST req) {
	RESPONSE res;
	res.r.h.version = VERSION;
	res.r.h.status = 1001;
	res.r.h.name_len = req.r.h.name_len;
	memcpy(&res.r.h.filename, &req.r.h.filename, req.r.h.name_len);

	send(clientsocket, res.buf, sizeof(res.buf), 0);
}

void send1002(SOCKET clientsocket) {
	RESPONSE res;
	res.r.h.version = VERSION;
	res.r.h.status = 1002;

	send(clientsocket, res.buf, sizeof(res.buf), 0);
}

void send1003(SOCKET clientsocket) {
	RESPONSE res;
	res.r.h.version = VERSION;
	res.r.h.status = 1003;

	send(clientsocket, res.buf, sizeof(res.buf), 0);
}

void handle100(SOCKET clientsocket, REQUEST req) {
	char* filename = new char[req.r.h.name_len + 1];
	memcpy(filename, &req.r.h.filename, req.r.h.name_len);
	filename[req.r.h.name_len] = '\0';

	std::string outputfolder = std::to_string(req.r.h.user_id);

	if (CreateDirectory(outputfolder.c_str(), NULL) ||
		ERROR_ALREADY_EXISTS == GetLastError()) {
		int size = 0;
		memcpy(&size, &req.r.h.filename + req.r.h.name_len / 4, 4);

		if (size > MAX_REQUEST_SIZE) {
			send1003(clientsocket);
			std::cerr << "Request is too big!" << std::endl;
			return;
		}

		std::ofstream outfile(outputfolder + "/" + filename);
		if (outfile.fail()) {
			send1003(clientsocket);
			std::cerr << "Failed open" << filename << "." << std::endl;
			return;
		}

		char* payload = new char[size + 1];
		memcpy(payload, &req.r.h.filename + req.r.h.name_len / 4 + 1, size);
		payload[size] = '\0';

		outfile.write(payload, size);
		outfile.close();

		send212(clientsocket, req);
		std::cout << "File " << filename << " uploaded successfully!" << std::endl;
	}
	else {
		send1003(clientsocket);
		std::cerr << "Failed to create directory." << std::endl;
		return;
	}
}

void handle200(SOCKET clientsocket, REQUEST req) {
	char* filename = new char[req.r.h.name_len + 1];
	memcpy(filename, &req.r.h.filename, req.r.h.name_len);
	filename[req.r.h.name_len] = '\0';

	std::string userfolder = std::to_string(req.r.h.user_id);

	int size = 0;
	memcpy(&size, &req.r.h.filename + req.r.h.name_len / 4, 4);

	std::ifstream requestedfile(userfolder + "/" + filename);
	if (requestedfile.fail()) {
		send1001(clientsocket, req);
		std::cerr << "Failed open file '" << filename << "'." << std::endl;
		return;
	}

	std::string payload((std::istreambuf_iterator<char>(requestedfile)),
		std::istreambuf_iterator<char>());

	send210(clientsocket, req, payload);
	std::cout << "File " << filename << " retrieved successfully!" << std::endl;
}

void handle201(SOCKET clientsocket, REQUEST req) {
	// get the fullpath of the file
	char* filename = new char[req.r.h.name_len + 1];
	memcpy(filename, &req.r.h.filename, req.r.h.name_len);
	filename[req.r.h.name_len] = '\0';

	std::string userfolder = std::to_string(req.r.h.user_id);

	auto fullpath = std::to_string(req.r.h.user_id) + "/" + filename;

	// delete that file
	if (remove(fullpath.c_str()) != 0) {
		send1001(clientsocket, req);
		std::cerr << "Failed deleteing " << filename << "." << std::endl;
	}
	else {
		send212(clientsocket, req);
		std::cerr << "File " << filename << " deleted successfully!" << std::endl;
	}
}

void handle202(SOCKET clientsocket, REQUEST req) {
	std::string userfolder = std::to_string(req.r.h.user_id);

	// get all files' names inside the user folder 
	std::vector<std::string> names;
	std::string search_path = userfolder + "/*.*";
	WIN32_FIND_DATA fd;
	HANDLE hFind = ::FindFirstFile(search_path.c_str(), &fd);
	if (hFind != INVALID_HANDLE_VALUE) {
		do {
			if (!(fd.dwFileAttributes & FILE_ATTRIBUTE_DIRECTORY)) {
				names.push_back(fd.cFileName);
			}
		} while (::FindNextFile(hFind, &fd));
		::FindClose(hFind);
	}

	// pretifier the results
	std::string payload = "";
	for (auto i = names.begin(); i != names.end(); ++i) {
		payload.append(*i);
		payload.append("\n");
	}

	if (names.size() == 0) {
		send1002(clientsocket);
		std::cout << "User " << req.r.h.user_id << " doesn't have any file!" << std::endl;
		return;
	}


	// create the auto-generated file
	std::string filename = gen_random(32);
	std::string outputfolder = std::to_string(req.r.h.user_id);

	std::ofstream outfile(outputfolder + "/" + filename);
	if (outfile.fail()) {
		send1003(clientsocket);
		std::cerr << "Failed open" << filename << "." << std::endl;
		return;
	}

	// insert files' names to the file
	outfile.write(payload.c_str(), payload.size());
	outfile.close();

	send211(clientsocket, req, filename, payload);
	std::cout << "File list of user " << req.r.h.user_id << " retrieved successfully!" << std::endl;
}

void printreq(REQUEST req) {
	std::cout << "###### Request ######" << std::endl;
	std::cout << "id:       " << req.r.h.user_id << std::endl;
	std::cout << "version:  " << req.r.h.version << std::endl;
	uint32_t op = int((unsigned char)(req.r.h.op));
	std::cout << "op:       " << op << std::endl;
	std::cout << "name_len: " << req.r.h.name_len << std::endl;
	char* filename = new char[req.r.h.name_len + 1];
	memcpy(filename, &req.r.h.filename, req.r.h.name_len);
	filename[req.r.h.name_len] = '\0';
	std::cout << "filename: " << filename << std::endl;
	int size = 0;
	memcpy(&size, &req.r.h.filename + req.r.h.name_len / 4, 4);
	std::cout << "size:     " << size << std::endl;
	if (size <= MAX_REQUEST_SIZE) {
		char* payload = new char[size + 1];
		memcpy(payload, &req.r.h.filename + req.r.h.name_len / 4 + 1, size);
		payload[size] = '\0';
		std::cout << "payload:  " << payload << std::endl;
	}
	std::cout << "#####################" << std::endl;
}

void handlerequest(SOCKET clientsocket) {
	std::cout << "\nClient connected!" << std::endl;

	REQUEST req;
	int bytesReceived = recv(clientsocket, req.buf, sizeof(req.buf), 0);
	if (bytesReceived == SOCKET_ERROR) {
		std::cerr << "Error in recv()." << std::endl;
		closesocket(clientsocket);
		return;
	}

	if (bytesReceived == 0) {
		std::cout << "Client disconnected." << std::endl;
		closesocket(clientsocket);
		return;
	}

	printreq(req);

	uint32_t op = int((unsigned char)(req.r.h.op));
	switch (op) {
	case 100:
		handle100(clientsocket, req);
		break;
	case 200:
		handle200(clientsocket, req);
		break;
	case 201:
		handle201(clientsocket, req);
		break;
	case 202:
		handle202(clientsocket, req);
		break;
	default:
		send1003(clientsocket);
		std::cerr << "Illegal op!" << std::endl;
	}

	closesocket(clientsocket);
}

void handleclient(SOCKET s) {
	SOCKET clientsocket = accept(s, NULL, NULL);
	std::thread ct(handlerequest, clientsocket);
	ct.detach();
}
