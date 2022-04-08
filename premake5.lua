local cpuArch = "x64"
local workSpaceName = "RayTracer"

-- Workspace
workspace(workSpaceName)
	architecture(cpuArch)
	configurations { "Release", "Debug" }
	location("Build")
	startproject("Example01_output_image")

	-- Compile types
	filter "configurations:Release"
		defines { "NDEBUG" }
		optimize "On"
		symbols "Off"

	filter "configurations:Debug"
		defines { "DEBUG" }
		optimize "Off"
		symbols "On"

local function setCppProject(projectName)
project(projectName)
	cppdialect("C++17")
	editAndContinue("Off")
	floatingpoint("Strict")
	language("C++")
	location("Build/"..projectName)
	targetdir("Build/Bin/"..projectName.."/%{cfg.buildcfg}"..cpuArch)
	warnings("Extra")
	flags
	{
		"FatalWarnings" -- treat warnings as errors
	}
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
	defines { "_CRT_SECURE_NO_WARNINGS", "WIN32_LEAN_AND_MEAN" }
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