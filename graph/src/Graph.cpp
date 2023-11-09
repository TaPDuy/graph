#include "Graph.h"

#include <vector>
#include <cmath>
#include <sstream>

#include "ImPlot/implot.h"
#include "ImPlot/implot_internal.h"
#include "utils/UtilFunc.h"

#define DELIMITER "|"

Graph::Graph(const std::string& graphTitle, const std::vector<double>& height) :
	title(graphTitle), heightData(height) {}

// create tooltip table when hover over graph
void Graph::plotCandlestick(const double* y_value, std::vector<Plot> plots, int count) {
	if (ImPlot::IsPlotHovered()) {
		ImPlotPoint mouse = ImPlot::GetPlotMousePos();
		double gap = y_value[1] - y_value[0];
		if (count > 1) gap = abs(y_value[0] - y_value[1]) / 2;
		int idx = UtilFunc::BinarySearch(y_value, 0, count - 1, mouse.y, gap);
		if (idx != -1) {
			ImGui::BeginTooltip();
			ImGui::Text("Depth: %.2f m", y_value[idx]);
			for (int i = 0; i < plots.size(); ++i) {
				double value = plots[i].data[idx];
				ImGui::Text((plots[i].title + ": %.2f").c_str(), value);
			}
			ImGui::EndTooltip();
		}
	}
}

int PlotMetricFormatter(double value, char* buff, int size, void* user_data) {
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
		if (i != 0) result += ", ";
		if (num_arr[i] < num_arr[i + 1]) {
			measure = (num_arr[i + 1] - num_arr[i]) * (1 - value) + num_arr[i];
		}
		else {
			measure = (num_arr[i] - num_arr[i + 1]) * value + num_arr[i + 1];
		}
		result += UtilFunc::CustomRound(measure);
	}

	return snprintf(buff, size, result.c_str());
}

void Graph::addPlot(const std::string title, const std::vector<double> data, double top_limit, double bottom_limit, bool reversed, bool is_shaded, double base_line, ImVec4 color) {
	plots.push_back(Plot(title, data, top_limit, bottom_limit, reversed, is_shaded, base_line, color));
}

std::string Graph::getMetricData() {
	std::string user_data = "";
	for (int i = 0; i < plots.size(); ++i) {
		if (plots[i].reversed) {
			user_data.append(std::to_string(plots[i].bottomLimit) + DELIMITER);
			user_data.append(std::to_string(plots[i].topLimit) + DELIMITER);
		}
		else {
			user_data.append(std::to_string(plots[i].topLimit) + DELIMITER);
			user_data.append(std::to_string(plots[i].bottomLimit) + DELIMITER);
		}
	}
	return user_data;
}

void Graph::render(const char* vAxisName) {
	// custom tick at the start and the end of X-axis
	std::string start_tick = "", end_tick = "", name = "";
	for (Plot plot : plots) {
		if (plot.reversed) {
			start_tick += UtilFunc::CustomRound(plot.topLimit) + "\n";
			end_tick += UtilFunc::CustomRound(plot.bottomLimit) + "\n";
		}
		else {
			start_tick += UtilFunc::CustomRound(plot.bottomLimit) + "\n";
			end_tick += UtilFunc::CustomRound(plot.topLimit) + "\n";
		}
		name += plot.title + "\n";
	}
	const char* labels[] = { start_tick.c_str(), name.c_str(), end_tick.c_str() };
	static double ticks[] = { 0.1, 0.5, 0.9 };

	// get top, bottom limits of plots to use for metric formater
	std::string user_data = getMetricData();

	// draw plot
	if (ImPlot::BeginPlot(title.c_str(), ImVec2(0, -1), ImPlotFlags_NoMenus)) {
		ImPlot::SetupAxes(
			title.c_str(), vAxisName,
			ImPlotAxisFlags_Lock | ImPlotAxisFlags_NoGridLines | ImPlotAxisFlags_NoTickMarks | ImPlotAxisFlags_NoSideSwitch,
			((vAxisName == nullptr) * ImPlotAxisFlags_NoLabel) | ((vAxisName == nullptr) * ImPlotAxisFlags_NoTickLabels) | ImPlotAxisFlags_Invert | ImPlotAxisFlags_NoSideSwitch
		);
		ImPlot::SetupAxisLimits(ImAxis_X1, 0, 1);
		ImPlot::SetupAxisLimits(ImAxis_Y1, heightData.front(), heightData.back());
		ImPlot::SetupAxisLimitsConstraints(ImAxis_Y1, heightData.front(), heightData.back());
		ImPlot::SetupAxisFormat(ImAxis_Y1, "%g m");
		ImPlot::SetupAxisFormat(ImAxis_X1, PlotMetricFormatter, (void*)user_data.c_str());
		ImPlot::SetupAxisTicks(ImAxis_X1, ticks, 3, labels, false);
		ImPlot::SetupLegend(ImPlotLocation_North, ImPlotLegendFlags_Outside | ImPlotLegendFlags_Horizontal | ImPlotLegendFlags_Sort);
		
		for (int i = 0; i < plots.size(); ++i) {
			ImPlot::SetNextLineStyle(plots[i].color, plots[i].thickness);
			ImPlot::PlotLine(plots[i].title.c_str(), plots[i].getData().data(), heightData.data(), heightData.size());
			if (plots[i].isShaded == true) {
				ImPlot::SetNextFillStyle(plots[i].color, plots[i].trans);
				if (!plots[i].reversed) {
					ImPlot::PlotShaded(plots[i].title.c_str(), plots[i].getData().data(), heightData.data(), heightData.size(), plots[i].baseLine, ImPlotShadedFlags_Horizontal);
				}
				else {
					ImPlot::PlotShaded(plots[i].title.c_str(), plots[i].getData().data(), heightData.data(), heightData.size(), 1 - plots[i].baseLine, ImPlotShadedFlags_Horizontal);
				}
			}

			if (ImPlot::BeginLegendPopup(plots[i].title.c_str())) {
				ImGui::Text(plots[i].title.c_str());
				ImGui::Separator();
				ImGui::Checkbox(("Reversed##" + plots[i].title).c_str(), &plots[i].reversed);
				ImGui::Separator();
				ImGui::DragFloatRange2(("Limit##" + plots[i].title).c_str(), &plots[i].bottomLimit, &plots[i].topLimit, 0.25f, 0.0f, 0.0f, "Min %.2f", "Max %.2f", ImGuiSliderFlags_AlwaysClamp);
				ImGui::Separator();
				ImGui::ColorEdit3(("Color##" + plots[i].title).c_str(), &plots[i].color.x);
				ImGui::SliderFloat(("Thicknes##" + plots[i].title).c_str(), &plots[i].thickness, 1, 5, "%.2f");
				ImGui::Separator();
				ImGui::Checkbox(("Shaded##" + plots[i].title).c_str(), &plots[i].isShaded);
				if (plots[i].isShaded) {
					ImGui::SliderFloat(("Transparency##" + plots[i].title).c_str(), &plots[i].trans, 0, 1, "%.2f");
					if (plots[i].reversed) {
						ImGui::SliderFloat(("Baseline##" + plots[i].title).c_str(), &plots[i].baseLine, 1, 0, "%.2f");
					}
					else {
						ImGui::SliderFloat(("Baseline##" + plots[i].title).c_str(), &plots[i].baseLine, 0, 1, "%.2f");
					}
				}
				ImPlot::EndLegendPopup();
			}
		}

		if (ImPlot::BeginDragDropTargetPlot()) {
			if (const ImGuiPayload* payload = ImGui::AcceptDragDropPayload("Plot data")) {
				DataColumn* col = (DataColumn*)payload->Data;
				std::string label = col->label;
				std::vector<double> data(col->data, col->data + col->size);
				this->addPlot(label, data, 0, 16, false, false, 0.0f, col->color);
			}
			ImPlot::EndDragDropTarget();
		}

		// create a tooltip table
		plotCandlestick(heightData.data(), plots, heightData.size());
		ImPlot::EndPlot();
	}
}

std::string Graph::getTitle() {
	return title;
}