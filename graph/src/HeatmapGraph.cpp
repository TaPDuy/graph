#include "HeatmapGraph.h"

#include <vector>
#include <cmath>
#include <sstream>

#include "ImPlot/implot.h"
#include "ImPlot/implot_internal.h"
#include "utils/UtilFunc.h"

HeatmapGraph::HeatmapGraph(const std::string& graphTitle, const std::vector<double>& depth, const std::vector<double> data, float scale_min, float scale_max):
	graphTitle(graphTitle), depth(depth) {
	map = ImPlotColormap_Viridis;
	minValue = data[0];
	maxValue = data[0];
	for (int i = 0; i < data.size(); ++i) {
		if (minValue > data[i]) minValue = data[i];
		if (maxValue < data[i]) maxValue = data[i];
	}
	if (scale_min == INFINITY) scale_min = minValue;
	if (scale_max == INFINITY) scale_max = maxValue;
	this->scaleMin = scale_min;
	this->scaleMax = scale_max;
	int n = data.size();
	for (int i = n - 1; i >= 0; --i) {
		this->data.push_back(data[i]);
	}
}

std::string HeatmapGraph::getTitle() {
	return graphTitle;
}

// create tooltip table when hover over graph
void HeatmapGraph::heatmapCandlestick(const double* y_value, int count, std::vector<double> data, std::string graph_title) {
	if (ImPlot::IsPlotHovered()) {
		ImPlotPoint mouse = ImPlot::GetPlotMousePos();
		double gap = 0.5;
		if (count > 1) {
			gap = abs(y_value[0] - y_value[1]) / 2;
		}
		int idx = UtilFunc::BinarySearch(y_value, 0, count - 1, mouse.y, gap);
		if (idx != -1) {
			ImGui::BeginTooltip();
			ImGui::Text("Depth: %.2f m", y_value[idx]);
			ImGui::Text((graph_title + ": %.2f").c_str(), data[data.size() - 1 - idx]);
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

		ImPlot::PlotHeatmap(graphTitle.c_str(), data.data(), depth.size(), 1, scaleMin, scaleMax, "", ImPlotPoint(0, depth.front() - depth_gap/2), ImPlotPoint(1, depth.back() + depth_gap/2));

		if (ImPlot::BeginLegendPopup(graphTitle.c_str())) {
			ImGui::Text(graphTitle.c_str());
			ImGui::Separator();
			if (ImPlot::ColormapButton(ImPlot::GetColormapName(map), ImVec2(225, 0), map)) {
				map = (map + 1) % ImPlot::GetColormapCount();
			}
			ImGui::SameLine();
			ImGui::LabelText(("##Colormap Index" + graphTitle).c_str(), "%s", "Change Colormap");
			ImGui::SetNextItemWidth(225);
			ImGui::DragFloatRange2(("Scale##" + graphTitle).c_str(), &scaleMin, &scaleMax, 0.01f, minValue - 1, maxValue + 1, "Min: %.3f", "Max: %.3f");
			ImPlot::ColormapScale(("##HeatScale" + graphTitle).c_str(), scaleMin, scaleMax, ImVec2(60, 200), "%g");
			ImPlot::EndLegendPopup();
		}
		heatmapCandlestick(depth.data(), depth.size(), data, graphTitle);
		ImPlot::EndPlot();
	}
	ImPlot::PopColormap();
}