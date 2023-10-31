#include <iostream>
#include <GL/glew.h>
#include <GLFW/glfw3.h>

#include <ImGui/imgui.h>
#include <ImGui/imgui_impl_glfw.h>
#include <ImGui/imgui_impl_opengl3.h>

#include <ImPlot/implot.h>
#include "utils/CSV.h"
#include "Graph.h"
#include "GraphGroup.h"
#include <cmath>

int main() {

	CSV csv("assets/data.csv");

	glfwInit();
	const char* glsl_version = "#version 330 core";
	glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
	glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);

	glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
	glfwWindowHint(GLFW_MAXIMIZED, GL_TRUE);

	GLFWwindow* window = glfwCreateWindow(1280, 720, "Imgui", nullptr, nullptr);
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

	GraphGroup group("Data", "Depth", csv.getColumn("DEPT"));
	group.addGraph("##gamma-ray");
	group.addPlot("##gamma-ray", "GR", csv.getColumn("GR"), INFINITY, 0, true, 0);
	group.addPlot("##gamma-ray", "CAL", csv.getColumn("CALI"), 16, 0, false, 0, ImVec4(0.5, 0.5, 0, 1));

	group.addGraph("##resistivity");
	group.addPlot("##resistivity", "RESDEEP", csv.getColumn("ILD"), 2000, 2);
	group.addPlot("##resistivity", "RESMED", csv.getColumn("ILM"), 2000, 2);

	group.addGraph("##density");
	group.addPlot("##density", "PE", csv.getColumn("PE"), 10, 0);
	group.addPlot("##density", "RHOB", csv.getColumn("RHOB"), 2.95, 1.95);
	group.addPlot("##density", "NPHI", csv.getColumn("NPHI"), 0.45, -0.15);

	while (!glfwWindowShouldClose(window)) {

		glfwPollEvents();

		ImGui_ImplOpenGL3_NewFrame();
		ImGui_ImplGlfw_NewFrame();
		ImGui::NewFrame();

		/*ImGui::ShowDemoWindow();*/
		ImPlot::ShowDemoWindow();

		ImGui::Begin("Visualizer");
		group.render();
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
