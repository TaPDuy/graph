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

void GraphGroup::addPlot(const std::string& graphName, const std::string& name, const std::vector<double>& data, double max_value, double min_value, bool reversed, bool is_shaded, double base_line, ImVec4 color) {
	nameMap[graphName]->addPlot(name, data, max_value, min_value, reversed, is_shaded, base_line, color);
}

inline void GraphGroup::addGraph(const std::string& graphName, const std::string& dataName, const std::vector<double>& data, double max_value, double min_value) {
	addGraph(graphName);
	addPlot(graphName, dataName, data, max_value, min_value);
}

std::vector<std::string> GraphGroup::getAllGraphTitles() {
	std::vector<std::string> titles;
	for (int i = 0; i < graphs.size(); ++i) {
		titles.push_back(graphs[i]->getTitle());
	}
	return titles;
}

void GraphGroup::render() {
	if (ImGui::BeginTable("##graph_select", 4, ImGuiTableFlags_BordersOuter)) {
		for (int i = 0; i < graphs.size(); ++i) {
			ImGui::TableNextColumn();
			ImGui::Checkbox(graphs[i]->getTitle().c_str(), &display[graphs[i]->getTitle()]);
		}
		ImGui::EndTable();
	}
	ImGui::Spacing(); ImGui::Spacing();
	ImGui::Separator();
	ImGui::Spacing(); ImGui::Spacing();
	bool display_y_axis = true;

	if (ImPlot::BeginSubplots(title.c_str(), 1, graphs.size(), ImVec2(-1, -1), ImPlotSubplotFlags_LinkAllY)) {
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
		ImPlot::EndSubplots();
	}
}

GraphGroup::~GraphGroup() {
	for (Graph* graph : graphs) {
		delete graph;
	}
}