#include "./server/Server.hpp"

int main(int argc, char *argv[]) {
	if (argc != 3) {
		cerr << "Error: Invalid number of arguments" << endl;
		return 1;
	}
	
	for (size_t i = 0; i < std::strlen(argv[1]); ++i) {
		if (!isdigit(argv[1][i])) {
			cerr << "Error: Invalid port number" << endl;
			return 1;
		}
	}

	long port = atol(argv[1]);

	if (!(port >= 1024 && port <= 49151)) {
		cerr << "Error: Invalid port number range" << endl;
		return 1; 
	} 

	Server server(atoi(argv[1]), argv[2]);
	server.run();
	return 0;
}
