#include "menu.h"

void Menu::show_menu() {
	std::cout << "MessageU client at your service." << std::endl << std::endl;
	std::cout << "110) Register" << std::endl;
	std::cout << "120) Request for clients list" << std::endl;
	std::cout << "130) Request for public key" << std::endl;
	std::cout << "140) Request for waiting messages" << std::endl;
	std::cout << "150) Send a text message" << std::endl;
	std::cout << "151) Send a request for symmetric key" << std::endl;
	std::cout << "151) Send your symmetric key" << std::endl;
	std::cout << "0) Exit client" << std::endl;
}