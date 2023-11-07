#include "Plot.h"

#include <string>
#include <vector>

Plot::Plot(const std::string& title, const std::vector<double>& data, 
	double top_limit, double bottom_limit, bool reversed, 
	bool is_shaded, float base_line, ImVec4 color) :
	title(title), data(data), isShaded(is_shaded), baseLine(base_line), 
	topLimit((float)top_limit), bottomLimit((float)bottom_limit), 
	color(color), reversed(reversed) {
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
	minValue = ((min > 0) ? 0 : min);
	maxValue = max;
}

std::vector<double> Plot::getData() {
	if (bottomLimit >= topLimit) {
		baseLine = 0;
		std::vector<double> result;
		for (int i = 0; i < data.size(); ++i) {
			result.push_back(0);
		}
		return result;
	}
	std::vector<double> normalized;
	for (int i = 0; i < data.size(); ++i) {
		double cell_value = (data[i] - bottomLimit) / (topLimit - bottomLimit);
		if (reversed) {
			cell_value = 1 - cell_value;
		}
		normalized.push_back(cell_value);
	}
	return normalized;
}