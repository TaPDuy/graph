#include "Graph.h"

Graph::Graph(const std::string& graphTitle, const std::vector<double>& height) :
	title(graphTitle), heightData(height)
{

}

void Graph::addPlot(const std::string& title, const std::vector<double>& data) {
	plots.push_back(Plot(title, data));
}

void Graph::render(const char* vAxisName) {

	if (ImPlot::BeginPlot(title.c_str(), ImVec2(0, -1))) {
		ImPlot::SetupAxes(
			title.c_str(), vAxisName,
			ImPlotAxisFlags_Opposite | ImPlotAxisFlags_Lock | ImPlotAxisFlags_NoSideSwitch,
			((vAxisName == nullptr) * ImPlotAxisFlags_NoDecorations) | ImPlotAxisFlags_Invert | ImPlotAxisFlags_NoSideSwitch
		);
		ImPlot::SetupAxisLimits(ImAxis_X1, 0, 1);
		ImPlot::SetupAxisLimits(ImAxis_Y1, heightData.front(), heightData.back());
		ImPlot::SetupAxisLimitsConstraints(ImAxis_Y1, heightData.front(), heightData.back());

		ImPlot::PushStyleVar(ImPlotStyleVar_FillAlpha, 0.2f);
		for (Plot plot : plots) {
			ImPlot::PlotShaded(plot.title.c_str(), plot.data.data(), heightData.data(), heightData.size(), 0, ImPlotShadedFlags_Horizontal);
			ImPlot::PlotLine(plot.title.c_str(), plot.data.data(), heightData.data(), heightData.size());
		}
		ImPlot::PopStyleVar();

		ImPlot::EndPlot();
	}
}