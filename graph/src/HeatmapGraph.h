#pragma once

#include <ImPlot/implot.h>
#include <string>
#include <vector>
#include <cmath>
#include <iostream>

class HeatmapGraph {
private:
	std::string graphTitle;
	std::vector<double> depth;
	std::vector<double> data;
	float scale_min;
	float scale_max;
	float min_value;
	float max_value;
	ImPlotColormap map;

public:
	HeatmapGraph(const std::string& graphTitle, const std::vector<double>& depth, const std::vector<double> data, float scale_min = INFINITY, float scale_max = INFINITY);
	void render(const char* vAxisName);
	std::string getTitle();
};