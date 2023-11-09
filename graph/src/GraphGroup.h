#pragma once

#include <ImPlot/implot.h>
#include <string>
#include <map>
#include "Graph.h"
#include "HeatmapGraph.h"
#include <vector>
#include <memory>

class GraphGroup
{
public:
	GraphGroup(const std::string& title, const std::string& heightTitle, const std::vector<double>& height);
	~GraphGroup();

	void addColumn(const std::string& name, const std::vector<double>& data);
	void addGraph(const std::string& name);
	void addHeatMapGraph(const std::string& graphTitle, const std::vector<double> data, float scale_min = INFINITY, float scale_max = INFINITY);
	void addPlot(const std::string& graphName, const std::string& name, const std::vector<double>& data, double top_limit = INFINITY, double bottom_limit = INFINITY, bool reseved = false, bool is_shaded = false, double base_line = 0, ImVec4 color = ImVec4(0, 0, 0, 1));
	inline void addGraph(const std::string& graphName, const std::string& dataName, const std::vector<double>& data, double top_limit, double bottom_limit);
	void render();

private:
	std::string title;
	std::vector<Graph*> graphs;
	std::vector<HeatmapGraph> heatmapGraphs;
	std::map<std::string, Graph*> nameMap;
	std::vector<double> heightData;
	std::string heightTitle;
	std::map<std::string, bool> display;
	std::vector<float> ratio;
	std::vector<DataColumn> columns;
	std::map<int, bool> isShown;

	void setRatio();
};

