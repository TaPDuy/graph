#include "GraphGroup.h"
#include <iostream>
#include "utils/UtilFunc.h"

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

void GraphGroup::addColumn(const std::string& name, const std::vector<double>& data) {
	DataColumn col(name, data);
	columns.push_back(col);
	isShown[col.idx] = false;
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

void GraphGroup::render() {
	setRatio();

	ImGui::BeginChild("Plot list", ImVec2(100, -1));
	for (const DataColumn& col : columns) {
		if (isShown[col.idx])
			continue;

		ImPlot::ItemIcon(col.color); ImGui::SameLine();
		ImGui::Selectable(col.label, false, 0, ImVec2(100, 0));
		if (ImGui::BeginDragDropSource(ImGuiDragDropFlags_None)) {
			ImGui::SetDragDropPayload("Plot data", &col, sizeof(DataColumn));
			ImPlot::ItemIcon(col.color); ImGui::SameLine();
			ImGui::TextUnformatted(col.label);
			ImGui::EndDragDropSource();
		}
	}
	ImGui::EndChild();
	ImGui::SameLine();
	ImGui::BeginChild("Plots", ImVec2(-1, -1));

	ImGuiStyle styleBackup = ImGui::GetStyle();
	UtilFunc::CustomCollapStyle();
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
	UtilFunc::CustomPlotSyle();
	bool display_y_axis = true;

	bool dragging = ImGui::IsMouseDragging(ImGuiMouseButton_Left);
	int col = (graphs.size() + heatmapGraphs.size());
	if (ImPlot::BeginSubplots(title.c_str(), 1, col > 0 ? col : 1, ImVec2(-1, -1), ImPlotSubplotFlags_LinkAllY, nullptr, ratio.data())) {
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

	if (dragging) {
		ImGui::SameLine();
		ImGui::BeginChild("Drop zone", ImVec2(100, -1));
		ImGui::Text("Drop here");
		ImGui::EndChild();
	}

	if (ImGui::BeginDragDropTarget()) {
		if (const ImGuiPayload* payload = ImGui::AcceptDragDropPayload("Plot data")) {
			DataColumn* col = (DataColumn*)payload->Data;
			std::string label = col->label;
			std::vector<double> data(col->data, col->data + col->size);
			this->addGraph(label);
			this->addPlot(label, label, data, 0, 16, false, false, 0.0f, col->color);
			isShown[col->idx] = true;
		}

		ImGui::EndDragDropTarget();
	}

	ImPlot::GetStyle() = backup;
	ImPlot::PopColormap();
	ImGui::GetStyle() = styleBackup;

	ImGui::EndChild();
}

GraphGroup::~GraphGroup() {
	for (Graph* graph : graphs) {
		delete graph;
	}
}