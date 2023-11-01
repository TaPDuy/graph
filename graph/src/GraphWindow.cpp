#include "GraphWindow.h"

GraphWindow::GraphWindow(const std::string& title) : 
	title(title),
	csv("assets/data.csv"),
	group("Data", "Depth", csv.getColumn("DEPT"))
{

}

void GraphWindow::init() {
	group.addGraph("##gamma-ray");
	group.addPlot("##gamma-ray", "GR", csv.getColumn("GR"), INFINITY, 0, false, true, 0);
	group.addPlot("##gamma-ray", "CAL", csv.getColumn("CAL"), 16, 0, true);

	group.addGraph("##resistivity");
	group.addPlot("##resistivity", "RESDEEP", csv.getColumn("RESDEEP"), 2000, 2);
	group.addPlot("##resistivity", "RESMED", csv.getColumn("RESMED"), 2000, 2);

	group.addGraph("##density");
	group.addPlot("##density", "PE", csv.getColumn("PE"), 10, 0);
	group.addPlot("##density", "RHOB", csv.getColumn("RHOB"), 2.95, 1.95);
	group.addPlot("##density", "NPHI", csv.getColumn("NPHI"), 0.45, -0.15);
}

void GraphWindow::render(GLFWwindow* window) {
	int width, height;
	glfwGetWindowSize(window, &width, &height);
	ImGui::SetNextWindowSize(ImVec2(width, height));
	ImGui::SetNextWindowPos(ImVec2(0, 0));

	ImGui::Begin(title.c_str(), NULL, ImGuiWindowFlags_NoCollapse | ImGuiWindowFlags_NoResize);
	group.render();
	ImGui::End();
}
