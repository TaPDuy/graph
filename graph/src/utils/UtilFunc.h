#pragma once

#include <string>

#ifndef UTIL_FUNC
#define UTIL_FUNC

namespace UtilFunc {
	int BinarySearch(const double* arr, int left, int right, double value, double gap);
	// generate random double
	double RandDouble(int min_value, int max_value);
	// custom round function
	std::string CustomRound(double value, double precision = 0.01);
}

#endif // !UTIL_FUNC
