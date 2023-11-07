#include "UtilFunc.h"
#include <cmath>
#include <string>

int UtilFunc::BinarySearch(const double* arr, int left, int right, double value, double gap) {
	if (right >= left) {
		int mid = left + (right - left) / 2;
		if (abs(arr[mid] - value) < gap)
			return mid;
		if (arr[mid] > value)
			return BinarySearch(arr, left, mid - 1, value, gap);
		return BinarySearch(arr, mid + 1, right, value, gap);
	}
	return -1;
}

double UtilFunc::RandDouble(int min_value, int max_value) {
	if (min_value >= max_value) return 1.0;
	return double(min_value + (std::rand() % (int)(max_value - min_value + 1)));
}

std::string UtilFunc::CustomRound(double value, double precision) {
	std::string new_value = std::to_string(std::round(value / precision) * precision);
	return new_value.substr(0, new_value.length() - 4);
}