
#include <iostream>
#include "./server/Server.hpp"

using namespace std;

int main(int argc, char *argv[]) {
	if (argc != 3) {
		cout << "Error: Invalid number of arguments" << endl;
		return 1;
	}
	Server server(argv[1], argv[2]);
	server.run();
}
