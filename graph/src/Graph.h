#pragma once

#include <ImPlot/implot.h>
#include <string>
#include <vector>
#include <cmath>

#include <iostream>

struct Plot
{
	std::string title;
	std::vector<double> data;

	bool is_shaded;
	float base_line;
	float top_limit;
	float bottom_limit;
	ImVec4 color;
	float trans;
	float thickness;
	bool reversed;
	double min_value;
	double max_value;

	Plot(const std::string& title, const std::vector<double>& data, double top_limit, double bottom_limit, bool reversed = false, bool is_shaded = false, float base_line = 0, ImVec4 color = ImVec4(0, 0, 0, 1)) :
		title(title), data(data), is_shaded(is_shaded), base_line(base_line), top_limit((float)top_limit), bottom_limit((float)bottom_limit), color(color), reversed(reversed) {
		if (reversed && base_line == 0) {
			base_line = 1;
		}
		trans = 0.2;
		thickness = 1;
		double min = data[0], max = data[0];
		for (int i = 1; i < data.size(); ++i) {
			if (min > data[i]) min = data[i];
			if (max < data[i]) max = data[i];
		}
		min_value = ((min > 0) ? 0 : min);
		max_value = max;
	}

	std::vector<double> getData() {
		if (bottom_limit >= top_limit) {
			base_line = 0;
			std::vector<double> result;
			for (int i = 0; i < data.size(); ++i) {
				result.push_back(0);
			}
			return result;
		}
		std::vector<double> normalized;
		for (int i = 0; i < data.size(); ++i) {
			double cell_value = (data[i] - bottom_limit) / (top_limit - bottom_limit);
			if (reversed) {
				cell_value = 1 - cell_value;
			}
			normalized.push_back(cell_value);
		}
		return normalized;
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
};