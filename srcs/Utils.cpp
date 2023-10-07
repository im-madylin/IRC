#include "Utils.hpp"

vector<string> split(string str, string delim)
{
	vector<string> result;
	
	size_t startPos = 0;
	size_t endPos = str.find(delim);

	while (endPos != string::npos) {
		result.push_back(str.substr(startPos, endPos - startPos));
		startPos = endPos + delim.length();
		endPos = str.find(delim, startPos);
	}
	if (str.length() > startPos) {
		result.push_back(str.substr(startPos));
	}
	return result;
}

string toString(size_t num)
{
	std::stringstream ss;
    ss << num;
    return ss.str();
}
