#pragma once

#include "ImPlot/implot.h"
#include <string>
#include <vector>

class Plot
{
private:
	std::vector<double> normalizedData;
	float oldTopLimit;
	float oldBottomLimit;
	bool wasReversed;

public:
	std::string title;
	std::vector<double> data;
	bool isShaded;
	float baseLine;
	float topLimit;
	float bottomLimit;
	ImVec4 color;
	float trans;
	float thickness;
	bool reversed;

	Plot(const std::string& title, const std::vector<double>& data, 
		double top_limit, double bottom_limit, bool reversed = false, 
		bool is_shaded = false, float base_line = 0, ImVec4 color = ImVec4(0, 0, 0, 1));
	std::vector<double> getData();
};
