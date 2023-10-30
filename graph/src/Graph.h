#pragma once

#include <ImPlot/implot.h>
#include <string>
#include <vector>
#include <cmath>

struct Plot
{
	std::string title;
	std::vector<double> data;

	bool is_shaded;
	double base_line;
	double max_value;
	double min_value;

	Plot(const std::string& title, const std::vector<double>& data, double max_value, double min_value, bool is_shaded = false, double base_line = 0) :
		title(title), data(data), is_shaded(is_shaded), base_line(base_line), max_value(max_value), min_value(min_value) {

	}
};

class Graph
{
public:
	Graph(const std::string& graphTitle, const std::vector<double>& height);

	void addPlot(const std::string title, const std::vector<double> data, double max_value, double min_value, bool is_shaded = false, double base_line = 0);
	void render(const char* vAxisName);

private:
	std::string title;
	std::vector<double> heightData;
	std::vector<Plot> plots;
};

