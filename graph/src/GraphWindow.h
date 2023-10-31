#pragma once

#include "GLFW/glfw3.h"

#include "utils/CSV.h"
#include "GraphGroup.h"

class GraphWindow
{
public:
	GraphWindow(const std::string& title);
	void init();
	void render(GLFWwindow* window);

private:
	CSV csv;
	GraphGroup group;
	std::string title;
};
