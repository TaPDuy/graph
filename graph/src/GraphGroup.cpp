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
}

void GraphGroup::addPlot(const std::string& graphName, const std::string& name, const std::vector<double>& data, double max_value, double min_value, bool reversed, bool is_shaded, double base_line, ImVec4 color) {
	nameMap[graphName]->addPlot(name, data, max_value, min_value, reversed, is_shaded, base_line, color);
}

inline void GraphGroup::addGraph(const std::string& graphName, const std::string& dataName, const std::vector<double>& data, double max_value, double min_value) {
	addGraph(graphName);
	addPlot(graphName, dataName, data, max_value, min_value);
}

void GraphGroup::render() {
	if (ImGui::BeginTabBar(title.c_str())) {
		for (auto it = graphs.begin(); it < graphs.end(); ++it) {
			(*it)->render(heightTitle.c_str());
		}
		ImGui::EndTabBar();
	}
}

GraphGroup::~GraphGroup() {
	for (Graph* graph : graphs) {
		delete graph;
	}
}