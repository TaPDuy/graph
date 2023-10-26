#include "geo_graph.h"
#include <string>
#include "ImPlot/implot.h"
#include "ImPlot/implot_internal.h"
#include <iostream>

GeoGraph::GeoGraph(size_t length, int subPlotNumber, double* depth) {
	this->length = length;
	this->subPlotNumber = subPlotNumber;
	this->depth = depth;
}

GeoGraph::GeoGrapghAttribute::GeoGrapghAttribute(
	std::string name,
	double* value,
	int subplotIndex,
	bool filled,
	double shade_line
	/*double minAxisLimit,
	double maxAxisLimit,
	double minZoom,
	double maxZoom*/
) {
	this->name = name;
	this->value = value;
	this->subplotIndex = subplotIndex;
	this->filled = filled;
	this->shade_line = shade_line;
	/*this->minAxisLimit = minAxisLimit;
	this->maxAxisLimit = maxAxisLimit;
	this->minZoom = minZoom;
	this->maxZoom = maxZoom;*/
}

void GeoGraph::addAttribute(
	std::string name,
	double* value,
	int subplotIndex,
	bool filled,
	double shade_line
	/*double minAxisLimit,
	double maxAxisLimit,
	double minZoom,
	double maxZoom*/
) {
	GeoGrapghAttribute newAttr = GeoGrapghAttribute(name, value, subplotIndex, filled, shade_line);
	this->attributes.push_back(newAttr);
}

void GeoGraph::drawSubGraph(int index) {
	const std::string graphId = "MultiAxis##" + std::to_string(index);
	if (ImPlot::BeginPlot(graphId.c_str(), ImVec2(-1, 0), ImPlotFlags_NoTitle)) {

		ImPlotAxisFlags flags = (index != 0 ? ImPlotAxisFlags_NoLabel : ImPlotAxisFlags_None);
		ImPlot::SetupAxis(ImAxis_Y1, "Depth", ImPlotAxisFlags_Invert | flags);
		ImPlot::SetupAxisLimits(ImAxis_Y1, this->depth[0], this->depth[length - 1]);
		ImPlot::SetupAxisFormat(ImAxis_Y1, "%g m");
		ImPlot::SetupAxisLimitsConstraints(ImAxis_Y1, this->depth[0], this->depth[length - 1]);
		ImPlot::SetupAxisZoomConstraints(ImAxis_Y1, 20, 3000);

		ImPlot::SetupAxis(ImAxis_X1, "", ImPlotAxisFlags_NoLabel | ImPlotAxisFlags_Opposite);
		ImPlot::SetupAxisLimits(ImAxis_X1, 0, 1);
		ImPlot::SetupAxisLimitsConstraints(ImAxis_X1, 0, 1);
		ImPlot::SetupAxisZoomConstraints(ImAxis_X1, 0.1, 1);

		for (int i = 0; i < this->attributes.size(); ++i) {
			if (this->attributes[i].subplotIndex == index) {
				ImPlot::PlotLine(attributes[i].name.c_str(), attributes[i].value, this->depth, this->length);
				
				/*if (attributes[i].filled) {
					float fill_ref = 0;
					ImPlot::PushStyleVar(ImPlotStyleVar_FillAlpha, 0.1f);
					ImPlot::PlotBars(attributes[i].name.c_str(), attributes[i].value, this->depth, this->length, 1, ImPlotBarsFlags_Horizontal);
					ImPlot::PopStyleVar();
				}*/
			}
		}
		ImPlot::EndPlot();
	}
}

void GeoGraph::draw() {
	if (ImPlot::BeginSubplots("Test Subplots", 1, this->subPlotNumber, ImVec2(-1, 800), ImPlotSubplotFlags_LinkAllY)) {
		for (int i = 0; i < this->subPlotNumber; ++i) {
			GeoGraph::drawSubGraph(i);
		}
		ImPlot::EndSubplots();
	}
}
