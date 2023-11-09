#pragma once

#include "Plot.h"

#include <ImPlot/implot.h>
#include <string>
#include <vector>

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
	void plotCandlestick(const double* y_value, std::vector<Plot>& plots, int count);
	std::string getMetricData();
};