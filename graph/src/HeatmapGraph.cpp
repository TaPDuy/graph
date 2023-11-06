#include "HeatmapGraph.h"

#include <vector>
#include <iostream>
#include <cmath>
#include <sstream>

#include "ImPlot/implot.h"
#include "ImPlot/implot_internal.h"

HeatmapGraph::HeatmapGraph(const std::string& graphTitle, const std::vector<double>& depth, const std::vector<double> data, float scale_min, float scale_max):
	graphTitle(graphTitle), depth(depth), data(data) {
	map = ImPlotColormap_Viridis;
	min_value = data[0];
	max_value = data[0];
	for (int i = 0; i < data.size(); ++i) {
		if (min_value > data[i]) min_value = data[i];
		if (max_value < data[i]) max_value = data[i];
	}
	if (scale_min == INFINITY) this->scale_min = min_value;
	if (scale_max == INFINITY) this->scale_max = max_value;
}

std::string HeatmapGraph::getTitle() {
	return graphTitle;
}

int HeatBinarySearch(const double* arr, int left, int right, double value, double gap) {
	if (right >= left) {
		int mid = left + (right - left) / 2;
		if (abs(arr[mid] - value) < gap)
			return mid;
		if (arr[mid] > value)
			return HeatBinarySearch(arr, left, mid - 1, value, gap);
		return HeatBinarySearch(arr, mid + 1, right, value, gap);
	}
	return -1;
}

// create tooltip table when hover over graph
void HeatmapPlotCandlestick(const double* y_value, int count, std::vector<double> data, std::string graph_title) {
	if (ImPlot::IsPlotHovered()) {
		ImPlotPoint mouse = ImPlot::GetPlotMousePos();
		double gap = 0.5;
		if (count > 1) {
			gap = abs(y_value[0] - y_value[1]) / 2;
		}
		int idx = HeatBinarySearch(y_value, 0, count - 1, mouse.y, gap);
		if (idx != -1) {
			ImGui::BeginTooltip();
			ImGui::Text("Depth: %.2f m", y_value[idx]);
			ImGui::Text((graph_title + ": %.2f").c_str(), data[idx]);
			ImGui::EndTooltip();
		}
	}
}

void HeatmapGraph::render(const char* vAxisName) {
	ImPlot::PushColormap(map);
	static const double depth_gap = depth[1] - depth[0];

	if (ImPlot::BeginPlot((graphTitle + "##heatmap").c_str(), ImVec2(0, -1), ImPlotFlags_NoMenus)) {
		ImPlot::SetupAxes(
			("##" + graphTitle).c_str(), vAxisName,
			ImPlotAxisFlags_Lock | ImPlotAxisFlags_NoDecorations,
			((vAxisName == nullptr) * ImPlotAxisFlags_NoLabel) | ((vAxisName == nullptr) * ImPlotAxisFlags_NoTickLabels) | ImPlotAxisFlags_Invert | ImPlotAxisFlags_NoSideSwitch
		);
		ImPlot::SetupAxisFormat(ImAxis_X1, "");
		ImPlot::SetupAxisLimits(ImAxis_X1, 0, 1);
		ImPlot::SetupAxisLimits(ImAxis_Y1, depth.front(), depth.back());
		ImPlot::SetupAxisLimitsConstraints(ImAxis_Y1, depth.front(), depth.back());
		ImPlot::SetupAxisFormat(ImAxis_Y1, "%g m");
		ImPlot::SetupLegend(ImPlotLocation_North, ImPlotLegendFlags_Outside | ImPlotLegendFlags_Horizontal | ImPlotLegendFlags_Sort);

		ImPlot::PlotHeatmap(graphTitle.c_str(), data.data(), depth.size(), 1, scale_min, scale_max, "%.1f", ImPlotPoint(0, depth.front() - depth_gap/2), ImPlotPoint(1, depth.back() + depth_gap/2));

		if (ImPlot::BeginLegendPopup(graphTitle.c_str())) {
			ImGui::Text(graphTitle.c_str());
			ImGui::Separator();
			if (ImPlot::ColormapButton(ImPlot::GetColormapName(map), ImVec2(225, 0), map)) {
				map = (map + 1) % ImPlot::GetColormapCount();
			}
			ImGui::SameLine();
			ImGui::LabelText(("##Colormap Index" + graphTitle).c_str(), "%s", "Change Colormap");
			ImGui::SetNextItemWidth(225);
			ImGui::DragFloatRange2(("Scale##" + graphTitle).c_str(), &scale_min, &scale_max, 0.01f, min_value - 1, max_value + 1, "Min: %.3f", "Max: %.3f");
			ImPlot::ColormapScale(("##HeatScale" + graphTitle).c_str(), scale_min, scale_max, ImVec2(60, 200), "%g");
			ImPlot::EndLegendPopup();
		}
		HeatmapPlotCandlestick(depth.data(), depth.size(), data, graphTitle);
		ImPlot::EndPlot();
	}
	ImPlot::PopColormap();
}