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
	print("Make project : "..projectName)
end

local function setExampleProject(projectName)
	setCppProject(projectName)
	kind "ConsoleApp"
	
	links
	{
		"Core",
	}
	
	files
	{
		"Source/Examples/"..projectName.."/*.*",
	}
	
	includedirs
	{
		"Source/Core/",
		"Source/Core/Geometry/",
		"Source/Core/Material/",
		"Source/Core/Math/",
		"Source/External/",
	}
end

-- Core
setCppProject("Core")
	kind "StaticLib"
	openmp "On"
	
	files
	{
		"Source/External/**.*",
		"Source/Core/**.*",
	}
	
	includedirs
	{
		"Source/Core/",
		"Source/Core/Geometry/",
		"Source/Core/Material/",
		"Source/Core/Math/",
		"Source/External/",
	}

-- Examples
local allExamples = os.matchdirs("Source/Examples/Example*")
for _, v in ipairs(allExamples) do
	local projectName = path.getname(v)
	setExampleProject(projectName)
end