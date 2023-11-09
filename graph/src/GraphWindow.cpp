#include "GraphWindow.h"

GraphWindow::GraphWindow(const std::string& title) : 
	title(title),
	csv("assets/data.csv"),
	group("Data", "Depth", csv.getColumn("DEPT"))
{ }

void GraphWindow::init() {
	const std::vector<std::string>& headers = csv.getHeaders();
	for (auto it = headers.begin() + 1; it < headers.end(); ++it) {
		group.addColumn(*it, csv.getColumn(*it));
	}

	/*group.addGraph("gamma-ray");
	group.addPlot("gamma-ray", "GR", csv.getColumn("GR"), INFINITY, 0, false, true, 0);
	group.addPlot("gamma-ray", "CAL", csv.getColumn("CAL"), 16, 0);

	group.addGraph("resistivity");
	group.addPlot("resistivity", "RESDEEP", csv.getColumn("RESDEEP"), 2000, 2);
	group.addPlot("resistivity", "RESMED", csv.getColumn("RESMED"), 2000, 2);

	group.addGraph("neutron density");
	group.addPlot("neutron density", "PE", csv.getColumn("PE"), 10, 0);
	group.addPlot("neutron density", "RHOB", csv.getColumn("RHOB"), 2.95, 1.95);
	group.addPlot("neutron density", "NPHI", csv.getColumn("NPHI"), 0.45, -0.15);

	group.addGraph("toc");
	group.addPlot("toc", "TOC", csv.getColumn("TOC"), 0.1, 0, true, true);
	
	group.addGraph("mineralogoy");
	group.addPlot("mineralogoy", "VCLAY", csv.getColumn("VCLAY"), 1, 0, true, true);
	group.addPlot("mineralogoy", "VOTZ", csv.getColumn("VOTZ"), 1, 0, true, true);
	group.addPlot("mineralogoy", "VDOL", csv.getColumn("VDOL"), 1, 0, true, true);
	group.addPlot("mineralogoy", "VCLC", csv.getColumn("VCLC"), 1, 0, true, true);
	group.addPlot("mineralogoy", "VPYR", csv.getColumn("VPYR"), 1, 0, true, true);
	group.addPlot("mineralogoy", "VOM", csv.getColumn("VOM"), 1, 0, true, true);

	group.addGraph("porosity");
	group.addPlot("porosity", "BVH", csv.getColumn("BVH"), 1, 0, true, true);
	group.addPlot("porosity", "BVWF", csv.getColumn("BVWF"), 1, 0, true, true);
	group.addPlot("porosity", "BVWI", csv.getColumn("BVWI"), 1, 0, true, true);

	group.addGraph("saturation");
	group.addPlot("saturation", "Sw", csv.getColumn("Sw"), 1, 0, true, true);

	group.addGraph("oil in place");
	group.addPlot("oil in place", "OIP", csv.getColumn("OIP"), 0.25, 0, false, true);
	group.addPlot("oil in place", "SUM OIP", csv.getColumn("SUM OIP"), 50, 0, true, true);

	group.addHeatMapGraph("Electrofacies", csv.getColumn("Electrofacies"));*/
}

void GraphWindow::render(GLFWwindow* window) {
	int width, height;
	glfwGetWindowSize(window, &width, &height);
	ImGui::SetNextWindowSize(ImVec2(width, height));
	ImGui::SetNextWindowPos(ImVec2(0, 0));

	ImGui::Begin(title.c_str(), NULL, ImGuiWindowFlags_NoCollapse | ImGuiWindowFlags_NoResize);
	group.render();
	ImGui::End();

	/*ImGui::ShowDemoWindow();
	ImPlot::ShowDemoWindow();*/
}
