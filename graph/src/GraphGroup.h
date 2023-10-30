#pragma once

#include <ImPlot/implot.h>
#include <string>
#include <map>
#include "Graph.h"

class GraphGroup
{
public:
	GraphGroup(const std::string& title, const std::string& heightTitle, const std::vector<double>& height);
	~GraphGroup();

	void addGraph(const std::string& name);
	void addPlot(const std::string& graphName, const std::string& name, const std::vector<double>& data);
	inline void addGraph(const std::string& graphName, const std::string& dataName, const std::vector<double>& data);
	void render();

private:
	std::string title;
	std::vector<Graph*> graphs;
	std::map<std::string, Graph*> nameMap;
	std::vector<double> heightData;
	std::string heightTitle;
};

