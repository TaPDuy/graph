#include "Plot.h"

#include <string>
#include <vector>
#include <iostream>

Plot::Plot(const std::string& title, const std::vector<double>& data, double top_limit, double bottom_limit, bool reversed, bool is_shaded, float base_line, ImVec4 color) :
	title(title), data(data), isShaded(is_shaded), baseLine(base_line), topLimit((float)top_limit), bottomLimit((float)bottom_limit), color(color), reversed(reversed) {
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
	oldTopLimit = topLimit;
	oldBottomLimit = bottomLimit;
	wasReversed = reversed;

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