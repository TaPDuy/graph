#include "GraphGroup.h"
#include <iostream>

GraphGroup::GraphGroup(const std::string& title, const std::string& heightTitle, const std::vector<double>& height)
	: title(title), heightTitle(heightTitle), heightData(height) {}

void GraphGroup::addGraph(const std::string& name) {
	Graph* graph = new Graph(name, heightData);
	graphs.push_back(graph);
	nameMap[name] = graph;
	display[name] = true;
	ratio.push_back(1.0f);
}

void GraphGroup::addPlot(const std::string& graphName, const std::string& name, const std::vector<double>& data, double top_limit, double bottom_limit, bool reversed, bool is_shaded, double base_line, ImVec4 color) {
	nameMap[graphName]->addPlot(name, data, top_limit, bottom_limit, reversed, is_shaded, base_line, color);
}

void GraphGroup::addHeatMapGraph(const std::string& graphTitle, const std::vector<double> data, float scale_min, float scale_max) {
	heatmapGraphs.push_back(HeatmapGraph(graphTitle, heightData, data, scale_min, scale_max));
	display[graphTitle] = true;
	ratio.push_back(1.0f);
}

inline void GraphGroup::addGraph(const std::string& graphName, const std::string& dataName, const std::vector<double>& data, double max_value, double min_value) {
	addGraph(graphName);
	addPlot(graphName, dataName, data, max_value, min_value);
}

void GraphGroup::setRatio() {
	int count = 0;
	for (std::map<std::string, bool>::iterator it = display.begin(); it != display.end(); ++it) {
		if (it->second) count++;
	}
	for (int i = 1; i < ratio.size(); ++i) {
		if (i >= count) {
			ratio[i] = 0.0f;
		}
		else {
			if (ratio[i] == 0) ratio[i] = 1.0f;
		}
	}
};

void CustomPlotSyle() {
	ImGuiStyle& imguiStyle = ImGui::GetStyle();
	ImVec4* imguiColors = imguiStyle.Colors;
	imguiColors[ImGuiCol_Text] = ImVec4(0.0f, 0.0f, 0.0f, 1.0f);
	imguiColors[ImGuiCol_PopupBg] = ImVec4(1.0f, 1.0f, 1.0f, 0.9f);
	imguiColors[ImGuiCol_FrameBg] = ImVec4(0.9f, 0.9f, 0.9f, 1.0f);
	imguiColors[ImGuiCol_FrameBgHovered] = ImVec4(0.8f, 0.8f, 0.8f, 1.0f);

	ImPlotStyle& style = ImPlot::GetStyle();
	ImVec4* colors = style.Colors;
	colors[ImPlotCol_FrameBg] = ImVec4(1.0f, 1.0f, 1.0f, 1.0f);
	colors[ImPlotCol_PlotBg] = ImVec4(0.95f, 0.95f, 0.95f, 1.0f);
	colors[ImPlotCol_PlotBorder] = ImVec4(0.0f, 0.0f, 0.0f, 1.0f);
	colors[ImPlotCol_LegendBg] = ImVec4(0.9f, 0.9f, 0.9f, 1.0f);
	colors[ImPlotCol_LegendBorder] = ImVec4(0.0f, 0.0f, 0.0f, 1.0f);
	colors[ImPlotCol_LegendText] = ImVec4(0.0f, 0.0f, 0.0f, 1.0f);
	colors[ImPlotCol_TitleText] = ImVec4(0.0f, 0.0f, 0.0f, 1.0f);
	colors[ImPlotCol_AxisText] = ImVec4(0.0f, 0.0f, 0.0f, 1.0f);
	colors[ImPlotCol_AxisGrid] = ImVec4(0.2f, 0.2f, 0.2f, 1.0f);
	colors[ImPlotCol_AxisBgHovered] = ImVec4(0.9f, 0.9f, 0.9f, 1.0f);
	colors[ImPlotCol_AxisBgActive] = ImVec4(0.9f, 0.9f, 0.9f, 0.75f);
	colors[ImPlotCol_InlayText] = ImVec4(0.0f, 0.0f, 0.0f, 1.0f);
}

void CustomCollapStyle() {
	ImGuiStyle& style = ImGui::GetStyle();
	ImVec4* colors = style.Colors;
	colors[ImGuiCol_Border] = ImVec4(0.0f, 0.0f, 0.0f, 1.0f);
	colors[ImGuiCol_FrameBg] = ImVec4(0.9f, 0.9f, 0.9f, 1.0f);
	colors[ImGuiCol_FrameBgHovered] = ImVec4(0.8f, 0.8f, 0.8f, 1.0f);
}

void GraphGroup::render() {
	setRatio();

	ImGuiStyle styleBackup = ImGui::GetStyle();
	CustomCollapStyle();
	if (ImGui::CollapsingHeader("Display graph")) {
		if (ImGui::BeginTable("##graph_select", 4, ImGuiTableFlags_BordersOuter)) {
			for (int i = 0; i < graphs.size(); ++i) {
				ImGui::TableNextColumn();
				ImGui::Checkbox(graphs[i]->getTitle().c_str(), &display[graphs[i]->getTitle()]);
			}
			for (int i = 0; i < heatmapGraphs.size(); ++i) {
				ImGui::TableNextColumn();
				ImGui::Checkbox(heatmapGraphs[i].getTitle().c_str(), &display[heatmapGraphs[i].getTitle()]);
			}
			ImGui::EndTable();
		}
	}

	ImGui::Spacing(); ImGui::Spacing();
	ImGui::Separator();
	ImGui::Spacing(); ImGui::Spacing();
	ImPlot::PushColormap(ImPlotColormap_Deep);
	ImPlotStyle backup = ImPlot::GetStyle();
	CustomPlotSyle();
	bool display_y_axis = true;
	if (ImPlot::BeginSubplots(title.c_str(), 1, (graphs.size() + heatmapGraphs.size()), ImVec2(-1, -1), ImPlotSubplotFlags_LinkAllY, nullptr, ratio.data())) {
		for (auto it = graphs.begin(); it < graphs.end(); ++it) {
			if (!display[(*it)->getTitle()]) continue;
			if (display_y_axis) {
				(*it)->render(heightTitle.c_str());
				display_y_axis = false;
			}
			else {
				(*it)->render(nullptr);
			}
		}
		for (int i = 0; i < heatmapGraphs.size(); ++i) {
			if (!display[heatmapGraphs[i].getTitle()]) continue;
			if (display_y_axis) {
				heatmapGraphs[i].render(heightTitle.c_str());
			}
			else {
				heatmapGraphs[i].render(nullptr);
			}
		}
		ImPlot::EndSubplots();
	}
	ImPlot::GetStyle() = backup;
	ImPlot::PopColormap();
	ImGui::GetStyle() = styleBackup;
}

GraphGroup::~GraphGroup() {
	for (Graph* graph : graphs) {
		delete graph;
	}
}