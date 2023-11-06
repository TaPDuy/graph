#include "GraphGroup.h"
#include <iostream>

GraphGroup::GraphGroup(const std::string& title, const std::string& heightTitle, const std::vector<double>& height)
	: title(title), heightTitle(heightTitle), heightData(height)
{
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
	heatmap_graphs.push_back(HeatmapGraph(graphTitle, heightData, data, scale_min, scale_max));
	display[graphTitle] = true;
}

inline void GraphGroup::addGraph(const std::string& graphName, const std::string& dataName, const std::vector<double>& data, double max_value, double min_value) {
	addGraph(graphName);
	addPlot(graphName, dataName, data, max_value, min_value);
}

int GraphGroup::getColumns() {
	int count = 0;
	for (auto const& imap : display) {
		count += imap.second;
	}
	return count;
}

void GraphGroup::render() {
	if (ImGui::BeginTable("##graph_select", 4, ImGuiTableFlags_BordersOuter)) {
		for (int i = 0; i < graphs.size(); ++i) {
			ImGui::TableNextColumn();
			ImGui::Checkbox(graphs[i]->getTitle().c_str(), &display[graphs[i]->getTitle()]);
		}
		for (int i = 0; i < heatmap_graphs.size(); ++i) {
			ImGui::TableNextColumn();
			ImGui::Checkbox(heatmap_graphs[i].getTitle().c_str(), &display[heatmap_graphs[i].getTitle()]);
		}
		ImGui::EndTable();
	}
	ImGui::Spacing(); ImGui::Spacing();
	ImGui::Separator();
	ImGui::Spacing(); ImGui::Spacing();
	bool display_y_axis = true;

	if (ImPlot::BeginSubplots(title.c_str(), 1, getColumns(), ImVec2(-1, -1), ImPlotSubplotFlags_LinkAllY)) {
		for (auto it = graphs.begin(); it < graphs.end(); ++it) {
			if (!display[(*it)->getTitle()]) {
				continue;
			}
			if (display_y_axis) {
				(*it)->render(heightTitle.c_str());
				display_y_axis = false;
			}
			else {
				(*it)->render(nullptr);
			}
		}
		for (int i = 0; i < heatmap_graphs.size(); ++i) {
			if (!display[heatmap_graphs[i].getTitle()]) continue;
			if (display_y_axis) {
				heatmap_graphs[i].render(heightTitle.c_str());
			}
			else {
				heatmap_graphs[i].render(nullptr);
			}
		}
		ImPlot::EndSubplots();
	}
}

GraphGroup::~GraphGroup() {
	for (Graph* graph : graphs) {
		delete graph;
	}
}