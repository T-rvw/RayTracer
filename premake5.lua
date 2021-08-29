local cpuArch = "x64"
local workSpaceName = "RayTracingRender"

-- Workspace
workspace(workSpaceName)
	architecture(cpuArch)
	configurations { "Debug", "Release", "RelWithDbgInfo" }
	location("build")
	startproject("example1_output_image")
	
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

local function setCppProject(projectName)
project(projectName)
	cppdialect("C++20")
	editAndContinue("Off")
	floatingpoint("Fast")
	language("C++")
	location("build/"..workSpaceName.."/"..projectName)
	targetdir("build/bin/%{cfg.buildcfg}"..cpuArch)
	warnings("Extra")
	
	files {
		"source/"..projectName.."/**.h",
		"source/"..projectName.."/**.cpp",
	}
end

local function setExampleProject(projectName)
	setCppProject(projectName)
	kind "ConsoleApp"
	
	includedirs { "source/core/"}
	links { "core" }
end

-- Projects
setCppProject("core")
	kind "StaticLib"

setExampleProject("example1_output_image")

setExampleProject("example2_blue_to_white")