local cpuArch = "x64"
local workSpaceName = "RayTracingRender"

-- Workspace
workspace(workSpaceName)
	architecture(cpuArch)
	configurations { "Debug", "Release", "RelWithDbgInfo" }
	location("Build")
	startproject("Example1_output_image")
	
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
	floatingpoint("Strict")
	language("C++")
	location("Build/"..projectName)
	targetdir("Build/Bin/"..projectName.."/%{cfg.buildcfg}"..cpuArch)
	warnings("Extra")
	
	files {
		"Source/"..projectName.."/**.h",
		"Source/"..projectName.."/**.cpp",
	}

	includedirs {
		"Source/Core/",
		"Source/Core/Geometry/",
		"Source/Core/Material/",
		"Source/Core/Math/",
	}
end

local function setExampleProject(projectName)
	setCppProject(projectName)
	print(projectName)
	kind "ConsoleApp"
	
	links { "Core" }
end

-- Projects
setCppProject("Core")
	kind "StaticLib"

-- Examples
local allExamples = os.matchdirs("Source/Example*")
for k, v in ipairs(allExamples) do
	print("Make exmaple project : "..path.getname(v))
	setExampleProject(path.getname(v))
end