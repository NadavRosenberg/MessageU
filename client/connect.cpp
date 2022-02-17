//#include <boost/asio.hpp>
//#include <iostream>
//
//using boost::asio::ip::tcp;
//
//int main2() {
//	char address[] = "127.0.0.1";
//	char port[] = "1234";
//	boost::asio::io_context io_context;
//	tcp::socket s(io_context);
//	tcp::resolver resolver(io_context);
//	boost::asio::connect(s, resolver.resolve(address, port));
//	int max_Length = 1042;
//	char request[max_length];
//	std::cout << "Enter message: ";
//	std::cin.getline(request, max_length);
//	boost::asio::write(s, boost::asio::buffer(request, max_Length));
//	return 0;
//}