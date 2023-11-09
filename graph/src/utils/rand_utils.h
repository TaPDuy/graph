#pragma once

#include <ImGui/imgui.h>

template <typename T>
inline T randomRange(T min, T max) {
	T scale = rand() / (T)RAND_MAX;
	return min + scale * (max - min);
}

inline ImVec4 randColor() {
	ImVec4 col;
	col.x = randomRange(0.0f, 1.0f);
	col.y = randomRange(0.0f, 1.0f);
	col.z = randomRange(0.0f, 1.0f);
	col.w = 1.0f;
	return col;
}
