#ifndef UTILS_HPP
# define UTILS_HPP

# include <string>
# include <vector>
# include <sstream>
# include <iostream>

using namespace std;

vector<string>	split(string str, string delim);
string			toString(size_t num);
void			errorExit(string message);

#endif
