#pragma once

#include <ImGui/imgui.h>
#include <math.h>
#include <stdlib.h>

template <typename T>
inline T randomRange(T min, T max) {
	T scale = rand() / (T)RAND_MAX;
	return min + scale * (max - min);
}

inline ImVec4 randColor() {
	ImVec4 col;

	const float L = randomRange(0.0f, 0.8f);
	const float S = randomRange(0.0f, 1.0f);
	const float H = randomRange(0.0f, 360.0f);

	const float a = S * fmin(L, 1 - L);

	const float kR = fmod(H / 30.0f, 12.0f);
	const float kG = fmod(8 + H / 30.0f, 12.0f);
	const float kB = fmod(4 + H / 30.0f, 12.0f);

	col.x = L - a * fmax(-1.0f, fmin(kR - 3, fmin(9 - kR, 1)));
	col.y = L - a * fmax(-1.0f, fmin(kG - 3, fmin(9 - kG, 1)));
	col.z = L - a * fmax(-1.0f, fmin(kB - 3, fmin(9 - kB, 1)));

	col.w = 1.0f;
	return col;
}
