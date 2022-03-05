#include "utils.h"

bool is_number(const std::string& s) {
	return !s.empty() && std::find_if(s.begin(),
		s.end(), [](unsigned char c) { return !isdigit(c); }) == s.end();
}

bool is_in_range(int num, int min, int max) {
	return num > min && num < max;
}

void exit_program() {
	system("pause");
	exit(0);
}