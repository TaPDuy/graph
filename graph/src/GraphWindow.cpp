#include "GraphWindow.h"
#include <string>
#include <vector>
#include "utils/CSV.h"

GraphWindow::GraphWindow(const std::string& title) : 
	title(title),
	csv("assets/data.csv"),
	group("Data", "Depth", csv.getColumn("DEPT"), 9) {
	displayed[GraphFile::GammaRay] = false;
	displayed[GraphFile::Electrofacies] = false;
	displayed[GraphFile::Mineralogoy] = false;
	displayed[GraphFile::NeutronDensity] = false;
	displayed[GraphFile::OilInPlace] = false;
	displayed[GraphFile::Porosity] = false;
	displayed[GraphFile::Resistivity] = false;
	displayed[GraphFile::Saturation] = false;
	displayed[GraphFile::Toc] = false;
}

void GraphWindow::loadFile(GraphFile name) {
	// if (displayed[name]) return;
	switch (name) {
	case GammaRay:
		group.addGraph("gamma-ray");
		group.addPlot("gamma-ray", "GR", csv.getColumn("GR"), INFINITY, 0, false, true, 0);
		group.addPlot("gamma-ray", "CAL", csv.getColumn("CAL"), 16, 0);
		graphMap["gamma - ray"] = GraphFile::GammaRay;
		break;
	case Resistivity:
		group.addGraph("resistivity");
		group.addPlot("resistivity", "RESDEEP", csv.getColumn("RESDEEP"), 2000, 2);
		group.addPlot("resistivity", "RESMED", csv.getColumn("RESMED"), 2000, 2);
		graphMap["resistivity"] = GraphFile::Resistivity;
		break;
	case NeutronDensity:
		group.addGraph("neutron density");
		group.addPlot("neutron density", "PE", csv.getColumn("PE"), 10, 0);
		group.addPlot("neutron density", "RHOB", csv.getColumn("RHOB"), 2.95, 1.95);
		group.addPlot("neutron density", "NPHI", csv.getColumn("NPHI"), 0.45, -0.15);
		graphMap["neutron density"] = GraphFile::NeutronDensity;
		break;
	case Toc:
		group.addGraph("toc");
		group.addPlot("toc", "TOC", csv.getColumn("TOC"), 0.1, 0, true, true);
		graphMap["toc"] = GraphFile::Toc;
		break;
	case Mineralogoy:
		group.addGraph("mineralogoy");
		group.addPlot("mineralogoy", "VCLAY", csv.getColumn("VCLAY"), 1, 0, true, true);
		group.addPlot("mineralogoy", "VOTZ", csv.getColumn("VOTZ"), 1, 0, true, true);
		group.addPlot("mineralogoy", "VDOL", csv.getColumn("VDOL"), 1, 0, true, true);
		group.addPlot("mineralogoy", "VCLC", csv.getColumn("VCLC"), 1, 0, true, true);
		group.addPlot("mineralogoy", "VPYR", csv.getColumn("VPYR"), 1, 0, true, true);
		group.addPlot("mineralogoy", "VOM", csv.getColumn("VOM"), 1, 0, true, true);
		graphMap["mineralogoy"] = GraphFile::Mineralogoy;
		break;
	case Porosity:
		group.addGraph("porosity");
		group.addPlot("porosity", "BVH", csv.getColumn("BVH"), 1, 0, true, true);
		group.addPlot("porosity", "BVWF", csv.getColumn("BVWF"), 1, 0, true, true);
		group.addPlot("porosity", "BVWI", csv.getColumn("BVWI"), 1, 0, true, true);
		graphMap["porosity"] = GraphFile::Porosity;
		break;
	case Saturation:
		group.addGraph("saturation");
		group.addPlot("saturation", "Sw", csv.getColumn("Sw"), 1, 0, true, true);
		graphMap["saturation"] = GraphFile::Saturation;
		break;
	case OilInPlace:
		group.addGraph("oil in place");
		group.addPlot("oil in place", "OIP", csv.getColumn("OIP"), 0.25, 0, false, true);
		group.addPlot("oil in place", "SUM OIP", csv.getColumn("SUM OIP"), 50, 0, true, true);
		graphMap["oil in place"] = GraphFile::OilInPlace;
		break;
	case Electrofacies:
		group.addHeatMapGraph("Electrofacies", csv.getColumn("Electrofacies"));
		graphMap["Electrofacies"] = GraphFile::Electrofacies;
		break;
	default:
		return;
	}
	displayed[name] = true;
}

bool GraphWindow::isGraphIncluded(GraphFile name) {
	return displayed[name];
}

void GraphWindow::reset() {
	displayed[GraphFile::GammaRay] = false;
	displayed[GraphFile::Electrofacies] = false;
	displayed[GraphFile::Mineralogoy] = false;
	displayed[GraphFile::NeutronDensity] = false;
	displayed[GraphFile::OilInPlace] = false;
	displayed[GraphFile::Porosity] = false;
	displayed[GraphFile::Resistivity] = false;
	displayed[GraphFile::Saturation] = false;
	displayed[GraphFile::Toc] = false;
	group.clear();
}

void GraphWindow::removeGraph(std::string title) {
	GraphFile id = graphMap[title];
	displayed[id] = false;
	std::vector<GraphFile> isBeingDisplay;
	group.clear();
	for (std::map<GraphFile, bool>::iterator it = displayed.begin(); it != displayed.end(); ++it) {
		if (displayed[it->first]) {
			loadFile(it->first);
		}
	}
}

std::string GraphWindow::getTitle() {
	return title;
}

void GraphWindow::render(GLFWwindow* window) {
	group.render();
}
