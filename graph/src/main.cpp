#include <iostream>
#include <GL/glew.h>
#include <GLFW/glfw3.h>

#include <ImGui/imgui.h>
#include <ImGui/imgui_impl_glfw.h>
#include <ImGui/imgui_impl_opengl3.h>
#include "utils/CSV.h"
#include <ImPlot/implot.h>
#include <ImPlot/implot_internal.h>

#include "geo_graph.h"

double randDouble(double minValue, double maxValue) {
	return minValue + (std::rand() % (int)(maxValue - minValue + 1));
}

int main() {

	CSV csv("assets/data.csv");

	double* cali = (double*)csv.getRow("CALI");
	double* gr = (double*)csv.getRow("GR");

	double* depth = (double*)csv.getRow("DEPT");
	const size_t length = csv.getSize();

	std::cout << length << " " << depth[0] << " " << depth[length-1] << "\n";

	GeoGraph graph = GeoGraph(length, 1, depth);
	
	for (int i = 0; i < length; ++i) {
		cali[i] = cali[i] / 16;
		gr[i] = gr[i] / 150;
	}

	graph.addAttribute("CAL", cali, 0);
	graph.addAttribute("GR", gr, 0, true);

	glfwInit();
	const char* glsl_version = "#version 330 core";
	glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
	glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);

	glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

	GLFWwindow* window = glfwCreateWindow(600, 600, "Imgui", nullptr, nullptr);
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

	while (!glfwWindowShouldClose(window)) {
		glfwPollEvents();

		ImGui_ImplOpenGL3_NewFrame();
		ImGui_ImplGlfw_NewFrame();
		ImGui::NewFrame();

		ImGui::ShowDemoWindow();

		ImPlot::ShowDemoWindow();

		{
			ImGui::Begin("Graph");
			if (ImGui::CollapsingHeader("Testing")) {
				if (ImPlot::BeginPlot("Testing Plot", ImVec2(-1, 500))) {
					ImPlot::SetupAxes("Cali", "Depth", ImPlotAxisFlags_NoTickLabels);
					ImPlot::SetupAxesLimits(0, 30, depth[0], depth[0] + 1000);

					ImPlot::SetupAxisLimitsConstraints(ImAxis_X1, 0, 30);
					ImPlot::SetupAxisZoomConstraints(ImAxis_X1, 5, 30);
					ImPlot::SetupAxisLimitsConstraints(ImAxis_Y1, depth[0], depth[length-1]);
					ImPlot::SetupAxisZoomConstraints(ImAxis_Y1, 50, 1000);

					ImPlot::PlotLine("First", cali, depth, length);

					ImPlot::EndPlot();
				}
			}

			if(ImGui::CollapsingHeader("Graph")) {
				graph.draw();
			}

			ImGui::End();
		}

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
