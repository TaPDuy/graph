#pragma once

#include "GLFW/glfw3.h"

#include "utils/CSV.h"
#include "GraphGroup.h"
#include <vector>
#include <string>

class GraphWindow
{
public:
	GraphWindow(const std::string& title);
	void render(GLFWwindow* window);
	std::string getTitle();
	void reset();
	bool isGraphIncluded(GraphFile name);
	void loadFile(GraphFile name);
	void removeGraph(std::string title);

private:
	CSV csv;
	GraphGroup group;
	std::string title;
	std::map<GraphFile, bool> displayed;
	std::map<std::string, GraphFile> graphMap;
};
