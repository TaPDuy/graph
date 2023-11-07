#include "UtilFunc.h"
#include <cmath>
#include <string>
#include "ImGui/imgui.h"
#include "ImPlot/implot.h"

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

void UtilFunc::CustomPlotSyle() {
	ImGuiStyle& imguiStyle = ImGui::GetStyle();
	ImVec4* imguiColors = imguiStyle.Colors;
	imguiColors[ImGuiCol_Text] = ImVec4(0.0f, 0.0f, 0.0f, 1.0f);
	imguiColors[ImGuiCol_PopupBg] = ImVec4(1.0f, 1.0f, 1.0f, 0.9f);
	imguiColors[ImGuiCol_FrameBg] = ImVec4(0.9f, 0.9f, 0.9f, 1.0f);
	imguiColors[ImGuiCol_FrameBgHovered] = ImVec4(0.8f, 0.8f, 0.8f, 1.0f);

	ImPlotStyle& style = ImPlot::GetStyle();
	ImVec4* colors = style.Colors;
	colors[ImPlotCol_FrameBg] = ImVec4(1.0f, 1.0f, 1.0f, 1.0f);
	colors[ImPlotCol_PlotBg] = ImVec4(0.98f, 0.98f, 0.98f, 1.0f);
	colors[ImPlotCol_PlotBorder] = ImVec4(0.0f, 0.0f, 0.0f, 1.0f);
	colors[ImPlotCol_LegendBg] = ImVec4(0.9f, 0.9f, 0.9f, 1.0f);
	colors[ImPlotCol_LegendBorder] = ImVec4(0.0f, 0.0f, 0.0f, 1.0f);
	colors[ImPlotCol_LegendText] = ImVec4(0.0f, 0.0f, 0.0f, 1.0f);
	colors[ImPlotCol_TitleText] = ImVec4(0.0f, 0.0f, 0.0f, 1.0f);
	colors[ImPlotCol_AxisText] = ImVec4(0.0f, 0.0f, 0.0f, 1.0f);
	colors[ImPlotCol_AxisGrid] = ImVec4(0.2f, 0.2f, 0.2f, 1.0f);
	colors[ImPlotCol_AxisBgHovered] = ImVec4(0.9f, 0.9f, 0.9f, 1.0f);
	colors[ImPlotCol_AxisBgActive] = ImVec4(0.9f, 0.9f, 0.9f, 0.75f);
	colors[ImPlotCol_InlayText] = ImVec4(0.0f, 0.0f, 0.0f, 1.0f);
}

void UtilFunc::CustomCollapStyle() {
	ImGuiStyle& style = ImGui::GetStyle();
	ImVec4* colors = style.Colors;
	colors[ImGuiCol_Border] = ImVec4(0.0f, 0.0f, 0.0f, 1.0f);
	colors[ImGuiCol_FrameBg] = ImVec4(0.9f, 0.9f, 0.9f, 1.0f);
	colors[ImGuiCol_FrameBgHovered] = ImVec4(0.8f, 0.8f, 0.8f, 1.0f);
}