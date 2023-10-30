#pragma once

#include <ImPlot/implot.h>
#include <string>
#include <vector>

struct Plot
{
	std::string title;
	std::vector<double> data;
};

class Graph
{
public:
	Graph(const std::string& graphTitle, const std::vector<double>& height);

	void addPlot(const std::string& title, const std::vector<double>& data);
	void render(const char* vAxisName);

private:
	std::string title;
	std::vector<double> heightData;
	std::vector<Plot> plots;
};

