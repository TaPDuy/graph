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
	float base_line;
	double max_value;
	double min_value;
	bool default_color;
	ImVec4 color;
	float trans;
	float thickness;
	bool reversed;

	Plot(const std::string& title, const std::vector<double>& data, double max_value, double min_value, bool reversed = false, bool is_shaded = false, float base_line = 0, ImVec4 color = ImVec4(0, 0, 0, 1)) :
		title(title), data(data), is_shaded(is_shaded), base_line(base_line), max_value(max_value), min_value(min_value), color(color), reversed(reversed) {
		if (((color.x + color.y + color.z) > 0) && (color.w == 1)) {
			default_color = false;
		}
		else {
			default_color = true;
		}
		if (reversed && base_line == 0) {
			base_line = 1;
		}
		trans = 0.2;
		thickness = 1;
	}
	
	std::vector<double> getData() {
		if (reversed) {
			std::vector<double> new_data;
			for (int i = 0; i < data.size(); ++i) {
				new_data.push_back(1.0 - data[i]);
			}
			return new_data;
		}
		return data;
	}
};

class Graph
{
public:
	Graph(const std::string& graphTitle, const std::vector<double>& height);
	void addPlot(const std::string title, const std::vector<double> data, double max_value, double min_value, bool reversed = false, bool is_shaded = false, double base_line = 0, ImVec4 color = ImVec4(0, 0, 0, 1));
	void render(const char* vAxisName);

private:
	std::string title;
	std::vector<double> heightData;
	std::vector<Plot> plots;
};

