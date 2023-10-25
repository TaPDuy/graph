#include "CSV.h"

#include <fstream>
#include <sstream>
#include <iostream>

CSV::CSV(const char* path) {
	std::cout << "reading file: " << path << std::endl;
	std::ifstream file(path);

	std::string tmp;
	std::getline(file, tmp);
	std::stringstream ss(tmp);
	while (std::getline(ss, tmp, ',')) {
		headers.push_back(tmp);
		data[tmp] = std::vector<double>();
	}

	std::string line;
	while (std::getline(file, line)) {
		size_t start = 0, sz = 0;
		for (std::string h : headers) {
			data[h].push_back(std::stod(line.substr(start), &sz));
			start += sz + 1;
		}
	}

	file.close();

	this->length = data[headers[0]].size();
}
