#include "Plot.h"

#include <string>
#include <vector>
#include <iostream>
#include "utils/UtilFunc.h"

Plot::Plot(const std::string& title, const std::vector<double>& data, double top_limit, double bottom_limit, bool reversed, bool is_shaded, float base_line, ImVec4 color) :
	title(title), data(data), isShaded(is_shaded), reversed(reversed) {
	
	// take max and min value of array as max and min limits if no value is provided
	if (bottom_limit >= top_limit) {
		bottom_limit = INFINITY;
		top_limit = INFINITY;
	}
	double top = ((top_limit != (INFINITY)) ? top_limit : *std::max_element(data.begin(), data.end()));
	double bottom = ((bottom_limit != (INFINITY)) ? bottom_limit : *std::min_element(data.begin(), data.end()));
	this->topLimit = top;
	this->bottomLimit = bottom;

	// base line
	float base = 0;
	if ((top >= base_line) && (base_line >= bottom)) {
		base = float((base_line - bottom) / (top - bottom));
	}
	this->baseLine = base;

	// set min max values, transparency, line weight
	trans = 0.2;
	thickness = 1;
	double min = data[0], max = data[0];
	for (int i = 1; i < data.size(); ++i) {
		if (min > data[i]) min = data[i];
		if (max < data[i]) max = data[i];
	}
	oldTopLimit = topLimit;
	oldBottomLimit = bottomLimit;
	wasReversed = reversed;

	// create random color
	double x_color = color.x, y_color = color.y, z_color = color.z;
	while ((x_color + y_color + z_color) == 0) {
		x_color = UtilFunc::RandDouble(0, 100) / 100;
		y_color = UtilFunc::RandDouble(0, 100) / 100;
		z_color = UtilFunc::RandDouble(0, 100) / 100;
	}
	this->color = ImVec4(x_color, y_color, z_color, color.w);

	for (int i = 0; i < data.size(); ++i) {
		double cell_value = 0;
		if (this->topLimit > this->bottomLimit) {
			cell_value = (data[i] - bottomLimit) / (topLimit - bottomLimit);
			if (reversed) {
				cell_value = 1 - cell_value;
			}
		}
		normalizedData.push_back(cell_value);
	}
}

std::vector<double> Plot::getData() {
	if (bottomLimit >= topLimit) {
		baseLine = 0;
		for (int i = 0; i < data.size(); ++i) {
			normalizedData[i] = 0;
		}
		return normalizedData;
	}

	if ((topLimit == oldTopLimit) && (bottomLimit == oldBottomLimit)) {
		if (wasReversed != reversed) {
			for (int i = 0; i < normalizedData.size(); ++i) {
				normalizedData[i] = 1 - normalizedData[i];
			}
			wasReversed = reversed;
		}
		return normalizedData;
	}

	for (int i = 0; i < data.size(); ++i) {
		double cell_value = (data[i] - bottomLimit) / (topLimit - bottomLimit);
		if (reversed) {
			cell_value = 1 - cell_value;
		}
		normalizedData[i] = cell_value;
	}
	oldTopLimit = topLimit;
	oldBottomLimit = bottomLimit;
	wasReversed = reversed;
	return normalizedData;
}