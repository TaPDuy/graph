#pragma once

#include <ImPlot/implot.h>
#include <string>
#include <vector>
#include <cmath>

class HeatmapGraph {
private:
	std::string graphTitle;
	std::vector<double> depth;
	std::vector<double> data;
	float scaleMin;
	float scaleMax;
	float minValue;
	float maxValue;
	ImPlotColormap map;
	void heatmapCandlestick(const double* y_value, int count, std::vector<double>& data, std::string graph_title);

public:
	HeatmapGraph(const std::string& graphTitle, const std::vector<double>& depth, const std::vector<double> data, float scale_min = INFINITY, float scale_max = INFINITY);
	void render(const char* vAxisName);
	std::string getTitle();
};