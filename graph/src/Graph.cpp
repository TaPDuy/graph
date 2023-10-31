#include "Graph.h"

#include <vector>
#include <iostream>
#include <cmath>
#include <sstream>

#include "ImPlot/implot.h"
#include "ImPlot/implot_internal.h"

#define DELIMITER "|"

Graph::Graph(const std::string& graphTitle, const std::vector<double>& height) :
	title(graphTitle), heightData(height)
{

}

// custom round function
std::string CustomRound(double value, double precision = 0.01) {
	std::string new_value = std::to_string(std::round(value / precision) * precision);
	return new_value.substr(0, new_value.length() - 4);
}

// find index of array base on value of depth
int BinarySearch(const double* arr, int left, int right, double value) {
	if (right >= left) {
		int mid = left + (right - left) / 2;
		if (int(arr[mid]) == int(value))
			return mid;
		if (arr[mid] > value)
			return BinarySearch(arr, left, mid - 1, value);
		return BinarySearch(arr, mid + 1, right, value);
	}
	return -1;
}

// create tooltip table when hover over graph
void PlotCandlestick(const double* y_value, std::vector<Plot> plots, int count) {
	if (ImPlot::IsPlotHovered()) {
		ImPlotPoint mouse = ImPlot::GetPlotMousePos();
		int idx = BinarySearch(y_value, 0, count - 1, mouse.y);
		if (idx != -1) {
			ImGui::BeginTooltip();
			ImGui::Text("Depth: %.2f m", y_value[idx]);
			for (int i = 0; i < plots.size(); ++i) {
				double value = plots[i].data[idx] * (plots[i].max_value - plots[i].min_value) + (plots[i].min_value);
				ImGui::Text((plots[i].title + ": %.2f").c_str(), value);
			}
			ImGui::EndTooltip();
		}
	}
}

int CustomMetricFormatter(double value, char* buff, int size, void* user_data) {
	const std::string data = (const char*)user_data;

	size_t last = 0, next = 0;
	std::string result = "";
	std::vector<double> num_arr;
	while ((next = data.find(DELIMITER, last)) != std::string::npos) {
		num_arr.push_back(std::stod(data.substr(last, next - last)));
		last = next + IM_ARRAYSIZE(DELIMITER) - 1;
	}
	double measure;
	for (int i = 0; i < num_arr.size(); i = i + 2) {
		if (i != 0) {
			result += ", ";
		}
		measure = (num_arr[i] - num_arr[i + 1]) * value + num_arr[i + 1];
		result += CustomRound(measure);
	}

	return snprintf(buff, size, result.c_str());
}

void Graph::addPlot(const std::string title, const std::vector<double> data, double max_value, double min_value, bool is_shaded, double base_line, ImVec4 color) {
	std::vector<double> normalized;

	// take max and min value of array as max and min limits if no value is provided
	double maxV = ((max_value != (INFINITY)) ? max_value : *std::max_element(data.begin(), data.end()));
	double minV = ((min_value != (INFINITY)) ? min_value : *std::min_element(data.begin(), data.end()));
	for (int i = 0; i < data.size(); ++i) {
		normalized.push_back((data[i] - minV) / (maxV - minV));
	}

	// base line if shade graph, default is 0
	float base = 0;
	if (maxV > minV) {
		base = float((base_line - minV) / (maxV - minV));
	}

	plots.push_back(Plot(title, normalized, maxV, minV, is_shaded, base, color));
}

void Graph::render(const char* vAxisName) {

	// custom tick at the start and the end of X-axis
	std::string start_tick = "", end_tick = "", name = "";
	for (Plot plot : plots) {
		start_tick += CustomRound(plot.min_value) + "\n";
		end_tick += CustomRound(plot.max_value) + "\n";
		name += plot.title + "\n";
	}
	const char* labels[] = { start_tick.c_str(), name.c_str(), end_tick.c_str() };
	static double ticks[] = { 0.1, 0.5, 0.9 };

	// get max, min values of plots to use for metric formater
	std::string user_data = "";
	for (int i = 0; i < plots.size(); ++i) {
		user_data.append(std::to_string(plots[i].max_value) + DELIMITER);
		user_data.append(std::to_string(plots[i].min_value) + DELIMITER);
	}

	// draw plot
	if (ImPlot::BeginPlot(title.c_str(), ImVec2(0, -1), ImPlotFlags_NoMenus)) {
		ImPlot::SetupAxes(
			title.c_str(), vAxisName,
			ImPlotAxisFlags_Lock | ImPlotAxisFlags_NoGridLines | ImPlotAxisFlags_NoTickMarks,
			((vAxisName == nullptr) * ImPlotAxisFlags_NoLabel) | ((vAxisName == nullptr) * ImPlotAxisFlags_NoTickLabels) | ImPlotAxisFlags_Invert | ImPlotAxisFlags_NoSideSwitch
		);
		ImPlot::SetupAxisLimits(ImAxis_X1, 0, 1);
		ImPlot::SetupAxisLimits(ImAxis_Y1, heightData.front(), heightData.back());
		ImPlot::SetupAxisLimitsConstraints(ImAxis_Y1, heightData.front(), heightData.back());
		ImPlot::SetupAxisFormat(ImAxis_Y1, "%g m");
		ImPlot::SetupAxisFormat(ImAxis_X1, CustomMetricFormatter, (void*)user_data.c_str());
		ImPlot::SetupAxisTicks(ImAxis_X1, ticks, 3, labels, false);
		ImPlot::SetupLegend(ImPlotLocation_North, ImPlotLegendFlags_Outside | ImPlotLegendFlags_Horizontal | ImPlotLegendFlags_Sort);
		
		for (Plot& plot : plots) {
			if (plot.default_color && ((plot.color.x > 0) || (plot.color.y > 0) || (plot.color.z > 0))) {
				plot.default_color = false;
			}

			ImPlot::PushStyleVar(ImPlotStyleVar_FillAlpha, plot.trans);
			if (plot.default_color) {
				ImPlot::SetNextLineStyle(ImVec4(0, 0, 0, -1), plot.thickness);
			}
			else {
				ImPlot::SetNextLineStyle(plot.color, plot.thickness);
				ImPlot::SetNextFillStyle(plot.color, plot.trans);
			}
			ImPlot::PlotLine(plot.title.c_str(), plot.data.data(), heightData.data(), heightData.size());
			if (plot.is_shaded == true) {
				ImPlot::PlotShaded(plot.title.c_str(), plot.data.data(), heightData.data(), heightData.size(), plot.base_line, ImPlotShadedFlags_Horizontal);
			}
			ImPlot::PopStyleVar();

			if (ImPlot::BeginLegendPopup(plot.title.c_str())) {
				ImGui::Text(plot.title.c_str());
				ImGui::ColorEdit3(("Color##" + plot.title).c_str(), &plot.color.x);
				ImGui::SliderFloat(("Thicknes##" + plot.title).c_str(), &plot.thickness, 1, 5, "%.2f");
				ImGui::Separator();
				ImGui::Checkbox(("Shaded##" + plot.title).c_str(), &plot.is_shaded);
				if (plot.is_shaded) {
					ImGui::SliderFloat(("Transparency##" + plot.title).c_str(), &plot.trans, 0, 1, "%.2f");
					ImGui::SliderFloat(("Baseline##" + plot.title).c_str(), &plot.base_line, 0, 1, "%.2f");
				}
				ImPlot::EndLegendPopup();
			}
		}

		// create a tooltip table
		PlotCandlestick(heightData.data(), plots, heightData.size());

		ImPlot::EndPlot();
	}
}