workspace "Pressia"
	architecture "x64"

	configurations {
		"Debug",
		"Release",
		"Dist",
	}
	
	startproject "Sandbox"
	outputdir = "%{cfg.buildcfg}-%{cfg.system}-%{cfg.architecture}"

	-- Include Directories
	IncludeDir = {}
	IncludeDir["GLFW"] = "Pressia/vendor/GLFW/include"
	IncludeDir["GLAD"] = "Pressia/vendor/GLAD/include"
	IncludeDir["imgui"] = "Pressia/vendor/imgui"
	IncludeDir["glm"] = "Pressia/vendor/glm"
	IncludeDir["stb_image"] = "Pressia/vendor/stb_image"

	group "Dependencies"
		include "Pressia/vendor/GLFW"
		include "Pressia/vendor/GLAD"
		include "Pressia/vendor/imgui"

	group ""


project "Pressia"
	location "Pressia"
	kind "StaticLib"
	language "C++"
	cppdialect "C++17"
	staticruntime "On"

	targetdir ("bin/" .. outputdir .. "/%{prj.name}")
	objdir ("bin-int/" .. outputdir .. "/%{prj.name}")

	pchheader "pspch.h"
	pchsource "Pressia/src/pspch.cpp"

	files {
		"%{prj.name}/src/**.h",
		"%{prj.name}/src/**.cpp",
		"%{prj.name}/vendor/stb_image/stb_image.h",
		"%{prj.name}/vendor/stb_image/stb_image.cpp",
		"%{prj.name}/vendor/glm/glm.hpp",
		"%{prj.name}/vendor/glm/glm.inl"
	}

	includedirs {
		"%{prj.name}/src",
		"%{prj.name}/vendor/spdlog/include",
		"%{IncludeDir.GLFW}",
		"%{IncludeDir.GLAD}",
		"%{IncludeDir.imgui}",
		"%{IncludeDir.glm}",
		"%{IncludeDir.stb_image}"
	}

	links {
		"GLFW",
		"GLAD",
		"ImGui",
		"opengl32.lib",
		"dwmapi.lib"
	}

	filter "system:windows"
		systemversion "latest"

		defines {
			"PS_PLATFORM_WINDOWS",
			"PS_BUILD_DLL",
			"GLFW_INCLUDE_NONE"
		}

	filter "configurations:Debug"
		defines {
			"PS_DEBUG",
			"PS_ENABLE_ASSERTS"
		}
		symbols "On"
		runtime "Debug"

	filter "configurations:Release"
		defines "PS_RELEASE"
		symbols "On"
		runtime "Release"
		optimize "On"

	filter "configurations:Dist"
		defines "PS_DIST"
		symbols "On"
		runtime "Release"
		optimize "On"


project "Sandbox"
	location "Sandbox"
	kind "ConsoleApp"
	language "C++"
	cppdialect "C++17"
	staticruntime "On"
	
	targetdir ("bin/" .. outputdir .. "/%{prj.name}")
	objdir ("bin-int/" .. outputdir .. "/%{prj.name}")

	files {
		"%{prj.name}/src/**.h",
		"%{prj.name}/src/**.cpp"
	}

	includedirs {
		"Pressia/vendor/spdlog/include",
		"Pressia/src",
		"Pressia/vendor",
		"%{IncludeDir.glm}"
	}

	links {
		"Pressia"
	}

	filter "system:windows"
		systemversion "latest"

		defines {
			"PS_PLATFORM_WINDOWS"
		}

	filter "configurations:Debug"
		defines "PS_DEBUG"
		symbols "On"
		runtime "Debug"

	filter "configurations:Release"
		defines "PS_RELEASE"
		symbols "On"
		runtime "Release"

	filter "configurations:Dist"
		defines "PS_DIST"
		symbols "On"
		runtime "Release"