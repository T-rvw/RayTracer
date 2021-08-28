local cpuArch = "x64"
local workSpaceName = "RayTracingRender"

-- Workspace
workspace(workSpaceName)
	architecture(cpuArch)
	configurations { "Debug", "Release", "RelWithDbgInfo" }
	location("build")
	startproject(workSpaceName)
	
	-- Compile types
	filter "configurations:Debug"
		defines { "DEBUG" }
		optimize "Off"
		symbols "On"
		
	filter "configurations:Release"
		defines { "NDEBUG" }
		optimize "On"
		symbols "Off"

	filter "configurations:RelWithDbgInfo"
		defines { "NDEBUG" }
		optimize "On"
		symbols "On"

-- Projects
project(workSpaceName)
	cppdialect("C++20")
	editAndContinue("Off")
	floatingpoint("Fast")
	language("C++")
	location("build/"..workSpaceName)
	targetdir("build/bin/%{cfg.buildcfg}"..cpuArch)
	warnings("Extra")
	
	kind "ConsoleApp"
	
	-- Project files
	files { 
		"source/**.h", "source/**.cpp",
	}