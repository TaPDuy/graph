#include "CSV.h"

#include <fstream>
#include <sstream>
#include <iostream>
#include <algorithm>

CSV::CSV(const char* path) {
	this->path = path;
	loadData();

	/*std::cout << "reading file: " << path << std::endl;
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

	this->length = data[headers[0]].size();*/
}

void CSV::loadData() {
	data.clear();
	headers.clear();

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

//std::vector<double> CSV::normalize(const std::string& header, double min, double max) {
//
//	std::vector<double> normalized;
//	std::transform(
//		data[header].begin(), data[header].end(),
//		std::back_inserter(normalized), 
//		[min, max](double d) { 
//			return (d - min) / (max - min);
//		}
//	);
//
//	return normalized;
//}
