#include "Graph.h"

#include <vector>
#include <iostream>
#include <cmath>
#include <sstream>

#include "ImPlot/implot.h"
#include "ImPlot/implot_internal.h"

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
int BinarySearch(const double* arr, int l, int r, double y) {
	if (r >= l) {
		int mid = l + (r - l) / 2;
		if (int(arr[mid]) == int(y))
			return mid;
		if (arr[mid] > y)
			return BinarySearch(arr, l, mid - 1, y);
		return BinarySearch(arr, mid + 1, r, y);
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

//int CustomMetricFormatter(double value, char* buff, int size, void* user_data) {
//	double* data = (double*)user_data;
//	std::string metric = "";
//	int length = data[0];
//	std::cout << "Length: " << length << "\n";
//	for (int i = 1; i <=length; i=i+2){
//		value = value * (data[i] - data[i + 1]) + data[i + 1];
//		metric += std::to_string(value);
//		if ((i + 2) != length) {
//			metric += ", ";
//		}
//	}
//	std::cout << "Metric: " << metric << "\n";
//	return snprintf(buff, size, metric.c_str());
//}

void Graph::addPlot(const std::string title, const std::vector<double> data, double max_value, double min_value, bool is_shaded, double base_line) {
	std::vector<double> normalized;

	// take max and min value of array as max and min limits if no value is provided
	double maxV = ((max_value != (INFINITY)) ? max_value : *std::max_element(data.begin(), data.end()));
	double minV = ((min_value != (INFINITY)) ? min_value : *std::min_element(data.begin(), data.end()));
	for (int i = 0; i < data.size(); ++i) {
		normalized.push_back((data[i] - minV) / (maxV - minV));
	}

	// base line if shade graph, default is 0
	double base = 0;
	if (maxV > minV) {
		base = (base_line - minV) / (maxV - minV);
	}

	plots.push_back(Plot(title, normalized, maxV, minV, is_shaded, base));
}

void Graph::render(const char* vAxisName) {

	// custom tick at the start and the end of X-axis
	std::string start_tick = "", end_tick = "", name = "";
	for (Plot plot : plots) {
		start_tick += CustomRound(plot.min_value) + "\n";
		end_tick += CustomRound(plot.max_value) + "\n";
		name += plot.title + "\n";
	}
	double ticks[] = { 0.05, 0.5, 0.95 };
	const char* labels[] = { start_tick.c_str(), name.c_str(), end_tick.c_str() };

	std::vector<double> user_data;
	user_data.push_back(plots.size());
	for (int i = 0; i < plots.size(); ++i) {
		user_data.push_back(plots[i].max_value);
		user_data.push_back(plots[i].min_value);
	}

	// draw plot
	if (ImPlot::BeginPlot(title.c_str(), ImVec2(0, -1))) {
		ImPlot::SetupAxes(
			title.c_str(), vAxisName,
			ImPlotAxisFlags_Opposite | ImPlotAxisFlags_Lock,
			((vAxisName == nullptr) * ImPlotAxisFlags_NoDecorations) | ImPlotAxisFlags_Invert | ImPlotAxisFlags_NoSideSwitch
		);
		ImPlot::SetupAxisLimits(ImAxis_X1, 0, 1);
		ImPlot::SetupAxisLimits(ImAxis_Y1, heightData.front(), heightData.back());
		ImPlot::SetupAxisLimitsConstraints(ImAxis_Y1, heightData.front(), heightData.back());
		if (vAxisName != nullptr) {
			ImPlot::SetupAxisFormat(ImAxis_Y1, "%g m");
		}

		/*ImPlot::SetupAxisFormat(ImAxis_X1, CustomMetricFormatter, (void*)user_data.data());*/

		ImPlot::SetupAxisTicks(ImAxis_X1, ticks, 3, labels, false);

		ImPlot::PushStyleVar(ImPlotStyleVar_FillAlpha, 0.2f);
		for (Plot plot : plots) {
			if (plot.is_shaded == true) {
				ImPlot::PlotShaded(plot.title.c_str(), plot.data.data(), heightData.data(), heightData.size(), plot.base_line, ImPlotShadedFlags_Horizontal);
			}
			ImPlot::PlotLine(plot.title.c_str(), plot.data.data(), heightData.data(), heightData.size());
		}
		ImPlot::PopStyleVar();

		// create a tooltip table
		PlotCandlestick(heightData.data(), plots, heightData.size());

		ImPlot::EndPlot();
	}
}