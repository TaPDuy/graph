#pragma once

#include <vector>
#include <map>
#include <string>

class CSV
{
public:
	CSV(const char* path);

	inline const double* getRow(std::string header) const {
		return data.at(header).data();
	}

	inline const std::vector<std::string>& getHeaders() const {
		return headers;
	}

	inline size_t getSize() const {
		return length;
	}

private:
	size_t length;
	std::vector<std::string> headers;
	std::map<std::string, std::vector<double>> data;
};

