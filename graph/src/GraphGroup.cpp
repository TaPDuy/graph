#include "GraphGroup.h"
#include <iostream>
#include "utils/UtilFunc.h"

GraphGroup::GraphGroup(const std::string& title, const std::string& heightTitle, const std::vector<double>& height, const int& maxGraph)
	: title(title), heightTitle(heightTitle), heightData(height), maxGraph(maxGraph) {
	for (int i = 0; i < maxGraph; ++i) {
		ratio.push_back(1.0f);
	}
}

void GraphGroup::addGraph(const std::string& name) {
	Graph* graph = new Graph(name, heightData);
	graphs.push_back(graph);
	nameMap[name] = graph;
	display[name] = true;
}

void GraphGroup::addPlot(const std::string& graphName, const std::string& name, const std::vector<double>& data, double top_limit, double bottom_limit, bool reversed, bool is_shaded, double base_line, ImVec4 color) {
	nameMap[graphName]->addPlot(name, data, top_limit, bottom_limit, reversed, is_shaded, base_line, color);
}

void GraphGroup::addHeatMapGraph(const std::string& graphTitle, const std::vector<double> data, float scale_min, float scale_max) {
	heatmapGraphs.push_back(HeatmapGraph(graphTitle, heightData, data, scale_min, scale_max));
	display[graphTitle] = true;
}

inline void GraphGroup::addGraph(const std::string& graphName, const std::string& dataName, const std::vector<double>& data, double max_value, double min_value) {
	addGraph(graphName);
	addPlot(graphName, dataName, data, max_value, min_value);
}

void GraphGroup::clear() {
	this->nameMap.clear();
	for (int i = 0; i < graphs.size(); ++i) {
		delete graphs[i];
		graphs[i] = nullptr;
	}
	graphs.clear();
	heatmapGraphs.clear();
	for (int i = 0; i < ratio.size(); ++i) {
		ratio[i] = 1.0f;
	}
}

void GraphGroup::setRatio() {
	int len = graphs.size() + heatmapGraphs.size();
	for (int i = 1; i < ratio.size(); ++i) {
		if (i >= len) {
			ratio[i] = 0;
		}
		else {
			if(ratio[i] == 0) ratio[i] = 1.0;
		}
	}
};

void GraphGroup::render() {
	setRatio();
	ImGuiStyle styleBackup = ImGui::GetStyle();
	ImPlot::PushColormap(ImPlotColormap_Deep);
	ImPlotStyle backup = ImPlot::GetStyle();
	UtilFunc::CustomPlotSyle();
	bool display_y_axis = true;

	if (ImPlot::BeginSubplots(title.c_str(), 1, (maxGraph > 0 ? maxGraph : 1), ImVec2(-1, -1), ImPlotSubplotFlags_LinkAllY, nullptr, ratio.data())) {
		for (auto it = graphs.begin(); it < graphs.end(); ++it) {
			if (display_y_axis) {
				(*it)->render(heightTitle.c_str());
				display_y_axis = false;
			}
			else {
				(*it)->render(nullptr);
			}
		}
		for (int i = 0; i < heatmapGraphs.size(); ++i) {
			if (display_y_axis) {
				heatmapGraphs[i].render(heightTitle.c_str());
			}
			else {
				heatmapGraphs[i].render(nullptr);
			}
		}
		ImPlot::EndSubplots();
	}

	ImPlot::GetStyle() = backup;
	ImPlot::PopColormap();
	ImGui::GetStyle() = styleBackup;
}

GraphGroup::~GraphGroup() {
	for (Graph* graph : graphs) {
		delete graph;
	}
}