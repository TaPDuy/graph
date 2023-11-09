#include <iostream>
#include <GL/glew.h>
#include <GLFW/glfw3.h>

#include <ImGui/imgui.h>
#include <ImGui/imgui_impl_glfw.h>
#include <ImGui/imgui_impl_opengl3.h>

#include <ImPlot/implot.h>
#include <cmath>
#include "GraphWindow.h"

int main() {

	glfwInit();
	const char* glsl_version = "#version 330 core";
	glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
	glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);

	glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

	GLFWwindow* window = glfwCreateWindow(800, 800, "Imgui", nullptr, nullptr);
	if (window == nullptr) {
		glfwTerminate();
		return -1;
	}
	glfwMakeContextCurrent(window);

	if (glewInit() != GLEW_OK) {
		glfwTerminate();
		return -1;
	}

	IMGUI_CHECKVERSION();
	ImGui::CreateContext();
	ImPlot::CreateContext();
	ImGuiIO& io = ImGui::GetIO(); (void)io;

	ImGui_ImplGlfw_InitForOpenGL(window, true);
	ImGui_ImplOpenGL3_Init(glsl_version);

	GraphWindow graph("Graph");
	static std::map<GraphFile, std::string> options = {
		{GraphFile::GammaRay, "Gramma ray"},
		{GraphFile::Mineralogoy, "Mineralogoy"},
		{GraphFile::NeutronDensity, "NeutronDensity"},
		{GraphFile::OilInPlace, "OilInPlace"},
		{GraphFile::Porosity, "Porosity"},
		{GraphFile::Resistivity, "Resistivity"},
		{GraphFile::Saturation, "Saturation"},
		{GraphFile::Toc, "Toc"},
		{GraphFile::Electrofacies, "Electrofacies"}
	};

	while (!glfwWindowShouldClose(window)) {

		glfwPollEvents();

		ImGui_ImplOpenGL3_NewFrame();
		ImGui_ImplGlfw_NewFrame();
		ImGui::NewFrame();

		int width, height;
		glfwGetWindowSize(window, &width, &height);
		ImGui::SetNextWindowSize(ImVec2(width, height));
		ImGui::SetNextWindowPos(ImVec2(0, 0));

		ImGui::Begin(graph.getTitle().c_str(), NULL, ImGuiWindowFlags_NoCollapse | ImGuiWindowFlags_NoResize);
		{
			ImGui::BeginChild("Select area", ImVec2(150, -1));
			if (ImGui::CollapsingHeader("Graph##choosegrap")) {
				for (std::map<GraphFile, std::string>::iterator it = options.begin(); it != options.end(); ++it) {
					if (graph.isGraphIncluded(it->first)) continue;
					ImGui::Selectable(it->second.c_str(), false, 0, ImVec2(150, 0));
					if (ImGui::BeginDragDropSource()) {
						ImGui::SetDragDropPayload("GRAPH_PAYLOAD", &(it->first), sizeof(GraphFile));
						ImGui::Text(it->second.c_str());
						ImGui::EndDragDropSource();
					}
				}
			}
			ImGui::Spacing();
			ImGui::Separator();
			ImGui::Spacing();
			if (ImGui::Button("Reset", ImVec2(-1, 0))) {
				graph.reset();
			}
			ImGui::Spacing();
			if (ImGui::Button("Refresh", ImVec2(-1, 0))) {
				graph.refresh();
			}
			ImGui::EndChild();

			if (ImGui::BeginDragDropTarget()) {
				if (const ImGuiPayload* payload = ImGui::AcceptDragDropPayload("SEC_PAYLOAD")) {
					std::string name = *(std::string*)payload->Data;
					graph.removeGraph(name);
				}
				ImGui::EndDragDropTarget();
			}
		}

		ImGui::SameLine();

		{
			ImGui::BeginChild("Graph area", ImVec2(-1, -1));
			graph.render(window);
			if (ImGui::BeginDragDropTarget()) {
				if (const ImGuiPayload* payload = ImGui::AcceptDragDropPayload("GRAPH_PAYLOAD")) {
					GraphFile idx = *(GraphFile*)payload->Data;
					graph.loadFile(idx);
				}
				ImGui::EndDragDropTarget();
			}
			ImGui::EndChild();
		}
		ImGui::End();

		ImGui::Render();
		glClear(GL_COLOR_BUFFER_BIT);
		ImGui_ImplOpenGL3_RenderDrawData(ImGui::GetDrawData());
		glfwSwapBuffers(window);
	}

	ImGui_ImplOpenGL3_Shutdown();
	ImGui_ImplGlfw_Shutdown();
	ImPlot::DestroyContext();
	ImGui::DestroyContext();

	glfwDestroyWindow(window);
	glfwTerminate();

	return 0;
}
