workspace "graph"
	architecture "x64"
	
	configurations 
	{
		"Debug",
		"Release"
	}
	
outputdir = "%{cfg.buildcfg}-%{cfg.system}-%{cfg.architecture}"
	
project "graph"
	location "graph"
	kind "ConsoleApp"
	language "C++"
	
	targetdir ("%{wks.location}/bin/" .. outputdir .. "/%{prj.name}")
	objdir ("%{wks.location}/bin-int/" .. outputdir .. "/%{prj.name}")
	
	files
	{
		"%{prj.name}/src/**.h",
		"%{prj.name}/src/**.cpp",
		"%{prj.name}/vendor/**.cpp",
	}
	
	includedirs
	{
		"vendor/include",
		"vendor/include/ImGui",
		"vendor/include/ImPlot"
	}
	
	libdirs {
		"vendor/lib/GLFW",
		"vendor/lib/GL",
	}
	
	links {
		"glew32",
		"glfw3",
		"opengl32"
	}
	
	postbuildcommands {
		("{COPYFILE} ../vendor/bin/*.dll ../bin/" .. outputdir .. "/%{prj.name}")
	}
	
	filter "system:windows"
		cppdialect "C++20"
		systemversion "latest"
		
	filter "configurations:Debug"
		symbols "On"
		
	filter "configurations:Release"
		optimize "On"
