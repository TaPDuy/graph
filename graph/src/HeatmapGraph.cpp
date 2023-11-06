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
	min_value = 0.0;
	max_value = 1.0;
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

void HeatmapGraph::render(const char* vAxisName) {
	ImPlot::PushColormap(map);

	if (ImPlot::BeginPlot((graphTitle + "##heatmap").c_str(), ImVec2(0, -1), ImPlotFlags_NoMenus)) {
		ImPlot::SetupAxes(
			("##" + graphTitle).c_str(), vAxisName,
			ImPlotAxisFlags_Lock | ImPlotAxisFlags_NoDecorations,
			((vAxisName == nullptr) * ImPlotAxisFlags_NoLabel) | ((vAxisName == nullptr) * ImPlotAxisFlags_NoTickLabels) | ImPlotAxisFlags_Invert | ImPlotAxisFlags_NoSideSwitch
		);

		ImPlot::SetupAxisLimits(ImAxis_X1, 0, 1);
		ImPlot::SetupAxisLimits(ImAxis_Y1, depth.front(), depth.back());
		ImPlot::SetupAxisLimitsConstraints(ImAxis_Y1, depth.front(), depth.back());
		ImPlot::SetupAxisFormat(ImAxis_Y1, "%g m");
		ImPlot::SetupLegend(ImPlotLocation_North, ImPlotLegendFlags_Outside | ImPlotLegendFlags_Horizontal | ImPlotLegendFlags_Sort);

		ImPlot::PlotHeatmap(graphTitle.c_str(), data.data(), depth.size(), 1, scale_min, scale_max, "%.1f", ImPlotPoint(0, depth.front() - 0.5), ImPlotPoint(1, depth.back() + 0.5));

		if (ImPlot::BeginLegendPopup(graphTitle.c_str())) {
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
		ImPlot::EndPlot();
	}
	ImPlot::PopColormap();
}