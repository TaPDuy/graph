#include "GraphGroup.h"

GraphGroup::GraphGroup(const std::string& title, const std::string& heightTitle, const std::vector<double>& height)
	: title(title), heightTitle(heightTitle), heightData(height)
{
}

void GraphGroup::addGraph(const std::string& name) {
	Graph* graph = new Graph(name, heightData);
	graphs.push_back(graph);
	nameMap[name] = graph;
}

void GraphGroup::addPlot(const std::string& graphName, const std::string& name, const std::vector<double>& data) {
	nameMap[graphName]->addPlot(name, data);
}

inline void GraphGroup::addGraph(const std::string& graphName, const std::string& dataName, const std::vector<double>& data) {
	addGraph(graphName);
	addPlot(graphName, dataName, data);
}

void GraphGroup::render() {
	if (ImPlot::BeginSubplots(title.c_str(), 1, graphs.size(), ImVec2(-1, -1), ImPlotSubplotFlags_LinkAllY)) {

		for (auto it = graphs.begin(); it < graphs.end(); ++it) {
			if (it != graphs.begin())
				(*it)->render(nullptr);
			else
				(*it)->render(heightTitle.c_str());
		}
		ImPlot::EndSubplots();
	}
}

GraphGroup::~GraphGroup() {
	for (Graph* graph : graphs) {
		delete graph;
	}
}