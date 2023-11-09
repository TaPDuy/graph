#pragma once

#include "Plot.h"
#include "utils/rand_utils.h"

#include <ImPlot/implot.h>
#include <string>
#include <vector>

struct DataColumn {
	int idx;
	const char* label;
	ImVec4 color;
	const double* data;
	size_t size;

	DataColumn(const std::string& label, const std::vector<double>& data) {
		static int uniqueId = 0;
		this->idx = uniqueId++;
		this->label = label.c_str();
		this->data = data.data();
		this->size = data.size();
		this->color = randColor();
	}
};

class Graph
{
public:
	Graph(const std::string& graphTitle, const std::vector<double>& height);
	void addPlot(const std::string title, const std::vector<double> data, double top_limit, double bottom_limit, bool reversed = false, bool is_shaded = false, double base_line = 0, ImVec4 color = ImVec4(0, 0, 0, 1));
	void render(const char* vAxisName);
	std::string getTitle();

private:
	std::string title;
	std::vector<double> heightData;
	std::vector<Plot> plots;
	void plotCandlestick(const double* y_value, std::vector<Plot> plots, int count);
	std::string getMetricData();
};