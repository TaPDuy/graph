#pragma once

#include <vector>
#include <map>
#include <string>

enum GraphFile {
	GammaRay, Resistivity, NeutronDensity, Toc, Mineralogoy, Porosity, Saturation, OilInPlace, Electrofacies
};

class CSV
{
public:
	CSV(const char* path);

	inline const double* getRow(const std::string& header) const {
		return data.at(header).data();
	}

	inline std::vector<double> getColumn(const std::string& header) const {
		return data.at(header);
	}

	inline const std::vector<std::string>& getHeaders() const {
		return headers;
	}

	inline size_t getSize() const {
		return length;
	}

	void loadData();

	//std::vector<double> normalize(const std::string& header, double min, double max);
private:
	std::string path;
	size_t length;
	std::vector<std::string> headers;
	std::map<std::string, std::vector<double>> data;
	// std::map<std::string, std::vector<double>> normalizedData;
};

