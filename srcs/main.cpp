
#include <iostream>
#include "./server/Server.hpp"

using namespace std;

int main(int argc, char *argv[]) {
	if (argc != 3) {
		cout << "Error: Invalid number of arguments" << endl;
		return 1;
	}
	
	for (size_t i = 0; i < std::strlen(argv[1]); ++i) {
        if (!isdigit(argv[1][i])) {
            cout << "Error: Invalid port number" << endl;
			return 1;
        }
    }

	long port = atol(argv[1]);

    if (!(port >= 0 && port <= 65535))
	{
		cout << "Error: Invalid port number range" << endl;
		return 1; 
	} 

	Server server(argv[1], argv[2]);
	server.run();
}
