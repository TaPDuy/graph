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

// generate random double
double RandDouble(int min_value, int max_value) {
	if (min_value >= max_value) return 1.0;
	return double(min_value + (std::rand() % (int)(max_value - min_value + 1)));
}

// custom round function
std::string CustomRound(double value, double precision = 0.01) {
	std::string new_value = std::to_string(std::round(value / precision) * precision);
	return new_value.substr(0, new_value.length() - 4);
}

// find index of array base on value of depth
int BinarySearch(const double* arr, int left, int right, double value, double gap) {
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

// create tooltip table when hover over graph
void PlotCandlestick(const double* y_value, std::vector<Plot> plots, int count) {
	if (ImPlot::IsPlotHovered()) {
		ImPlotPoint mouse = ImPlot::GetPlotMousePos();
		double gap = 0.5;
		if (count > 1) {
			gap = abs(y_value[0] - y_value[1]) / 2;
		}
		int idx = BinarySearch(y_value, 0, count - 1, mouse.y, gap);
		if (idx != -1) {
			ImGui::BeginTooltip();
			ImGui::Text("Depth: %.2f m", y_value[idx]);
			for (int i = 0; i < plots.size(); ++i) {
				if (!plots[i].display) continue;
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
		if (num_arr[i] < num_arr[i + 1]) {
			measure = (num_arr[i + 1] - num_arr[i]) * (1 - value) + num_arr[i];
		}
		else {
			measure = (num_arr[i] - num_arr[i + 1]) * value + num_arr[i + 1];
		}
		result += CustomRound(measure);
	}

	return snprintf(buff, size, result.c_str());
}

void Graph::addPlot(const std::string title, const std::vector<double> data, double max_value, double min_value, bool reversed, bool is_shaded, double base_line, ImVec4 color) {
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

	double x_color = color.x, y_color = color.y, z_color = color.z;
	while ((x_color + y_color + z_color) == 0) {
		x_color = RandDouble(0, 100) / 100;
		y_color = RandDouble(0, 100) / 100;
		z_color = RandDouble(0, 100) / 100;
	}
	ImVec4 new_color = ImVec4(x_color, y_color, z_color, 1);

	plots.push_back(Plot(title, normalized, maxV, minV, reversed, is_shaded, base, new_color));
}

void Graph::render(const char* vAxisName) {

	// custom tick at the start and the end of X-axis
	std::string start_tick = "", end_tick = "", name = "";
	for (Plot plot : plots) {
		if (!plot.display) continue;
		if (plot.reversed) {
			start_tick += CustomRound(plot.max_value) + "\n";
			end_tick += CustomRound(plot.min_value) + "\n";
		}
		else {
			start_tick += CustomRound(plot.min_value) + "\n";
			end_tick += CustomRound(plot.max_value) + "\n";
		}
		name += plot.title + "\n";
	}
	const char* labels[] = { start_tick.c_str(), name.c_str(), end_tick.c_str() };
	static double ticks[] = { 0.1, 0.5, 0.9 };

	// get max, min values of plots to use for metric formater
	std::string user_data = "";
	for (int i = 0; i < plots.size(); ++i) {
		if (!plots[i].display) continue;
		if (plots[i].reversed) {
			user_data.append(std::to_string(plots[i].min_value) + DELIMITER);
			user_data.append(std::to_string(plots[i].max_value) + DELIMITER);
		}
		else {
			user_data.append(std::to_string(plots[i].max_value) + DELIMITER);
			user_data.append(std::to_string(plots[i].min_value) + DELIMITER);
		}
	}

	// draw plot
	if (ImGui::BeginTabItem(title.c_str())) {
		{
			ImGui::BeginChild("Selectables Area", ImVec2(100, 400));
			ImGui::Spacing();
			if (ImGui::Button(("Reset legend##" + title).c_str())) {
				for (int i = 0; i < plots.size(); ++i) {
					plots[i].display = false;
				}
			}
			ImGui::Spacing(); ImGui::Spacing();
			for (int i = 0; i < plots.size(); ++i) {
				if (!plots[i].display) {
					ImPlot::ItemIcon(plots[i].color);
					ImGui::SameLine();
					ImGui::Selectable(plots[i].title.c_str(), false, 0, ImVec2(100, 0));
					if (ImGui::BeginDragDropSource()) {
						ImGui::SetDragDropPayload(("PLOT_PAYLOAD" + title).c_str(), &i, sizeof(int));
						ImPlot::ItemIcon(plots[i].color);
						ImGui::SameLine();
						ImGui::Text(plots[i].title.c_str());
						ImGui::EndDragDropSource();
					}
				}
			}
			ImGui::EndChild();
			if (ImGui::BeginDragDropTarget()) {
				if (const ImGuiPayload* payload = ImGui::AcceptDragDropPayload(("PLOT_PAYLOAD" + title).c_str())) {
					int index = *(int*)payload->Data;
					plots[index].display = false;
				}
				ImGui::EndDragDropTarget();
			}
		}

		ImGui::SameLine();

		{
			ImGui::BeginChild("Plot Area");
			if (ImPlot::BeginPlot(title.c_str(), ImVec2(-1, -1), ImPlotFlags_NoMenus)) {
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

				for (int i = 0; i < plots.size(); ++i) {
					if (!plots[i].display) continue;
					ImPlot::SetNextLineStyle(plots[i].color, plots[i].thickness);
					ImPlot::PlotLine(plots[i].title.c_str(), plots[i].getData().data(), heightData.data(), heightData.size());
					if (plots[i].is_shaded == true) {
						ImPlot::SetNextFillStyle(plots[i].color, plots[i].trans);
						ImPlot::PlotShaded(plots[i].title.c_str(), plots[i].getData().data(), heightData.data(), heightData.size(), plots[i].base_line, ImPlotShadedFlags_Horizontal);
					}

					if (ImPlot::BeginDragDropSourceItem(plots[i].title.c_str())) {
						ImGui::SetDragDropPayload(("PLOT_PAYLOAD" + title).c_str(), &i, sizeof(int));
						ImPlot::ItemIcon(plots[i].color);
						ImGui::SameLine();
						ImGui::Text(plots[i].title.c_str());
						ImPlot::EndDragDropSource();
					}

					if (ImPlot::BeginLegendPopup(plots[i].title.c_str())) {
						ImGui::Text(plots[i].title.c_str());
						ImGui::Separator();
						ImGui::Checkbox(("Reversed##" + plots[i].title).c_str(), &plots[i].reversed);
						ImGui::Separator();
						ImGui::ColorEdit3(("Color##" + plots[i].title).c_str(), &plots[i].color.x);
						ImGui::SliderFloat(("Thicknes##" + plots[i].title).c_str(), &plots[i].thickness, 1, 5, "%.2f");
						ImGui::Separator();
						ImGui::Checkbox(("Shaded##" + plots[i].title).c_str(), &plots[i].is_shaded);
						if (plots[i].is_shaded) {
							ImGui::SliderFloat(("Transparency##" + plots[i].title).c_str(), &plots[i].trans, 0, 1, "%.2f");
							ImGui::SliderFloat(("Baseline##" + plots[i].title).c_str(), &plots[i].base_line, 0, 1, "%.2f");
						}
						ImPlot::EndLegendPopup();
					}
				}

				if (ImPlot::BeginDragDropTargetPlot()) {
					if (const ImGuiPayload* payload = ImGui::AcceptDragDropPayload(("PLOT_PAYLOAD" + title).c_str())) {
						int index = *(int*)payload->Data;
						plots[index].display = true;
					}
					ImPlot::EndDragDropTarget();
				}

				// create a tooltip table
				PlotCandlestick(heightData.data(), plots, heightData.size());

				ImPlot::EndPlot();
			}
			ImGui::EndChild();
		}
		ImGui::EndTabItem();
	}
	
}