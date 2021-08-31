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
	location("Build/"..workSpaceName.."/"..projectName)
	targetdir("Build/bin/%{cfg.buildcfg}"..cpuArch)
	warnings("Extra")
	
	files {
		"Source/"..projectName.."/**.h",
		"Source/"..projectName.."/**.cpp",
	}

	includedirs {
		"Source/Core/",
		"Source/Core/Geometry/",
		"Source/Core/Math/"
	}
end

local function setExampleProject(projectName)
	setCppProject(projectName)
	kind "ConsoleApp"
	
	links { "Core" }
end

-- Projects
setCppProject("Core")
	kind "StaticLib"

setExampleProject("Example1_output_image")
setExampleProject("Example2_blue_to_white")
setExampleProject("Example3_red_sphere")
setExampleProject("Example4_surface_normal")
setExampleProject("Example5_multiple_spheres")
setExampleProject("example6_antialiasing")
setExampleProject("Example7_diffuse_material")