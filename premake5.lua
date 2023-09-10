workspace "Pressia"
	architecture "x64"

	configurations {
		"Debug",
		"Release",
		"Dist",
	}

outputdir = "%{cfg.buildcfg}-%{cfg.system}-%{cfg.architecture}"

project "Pressia"
	location "Pressia"
	kind "SharedLib"
	language "C++"

	targetdir ("bin/" .. outputdir .. "/%{prj.name}")
	objdir ("bin-int/" .. outputdir .. "/%{prj.name}")

	pchheader "pspch.h"
	pchsource "Pressia/src/pspch.cpp"

	files {
		"%{prj.name}/src/**.h",
		"%{prj.name}/src/**.cpp",
	}

	includedirs {
		"%{prj.name}/src",
		"%{prj.name}/vendor/spdlog/include"
	}

	filter "system:windows"
		cppdialect "C++17"
		staticruntime "On"
		systemversion "latest"

		defines {
			"PS_PLATFORM_WINDOWS",
			"PS_BUILD_DLL"
		}

		postbuildcommands {
			("{COPY} %{cfg.buildtarget.relpath} ../bin/" .. outputdir .. "/Sandbox")
		}

	filter "configurations:Debug"
		defines "PS_DEBUG"
		symbols "On"

	filter "configurations:Release"
		defines "PS_RELEASE"
		symbols "On"

	filter "configurations:Dist"
		defines "PS_DIST"
		symbols "On"


project "Sandbox"
	location "Sandbox"
	kind "ConsoleApp"
	language "C++"
	
	targetdir ("bin/" .. outputdir .. "/%{prj.name}")
	objdir ("bin-int/" .. outputdir .. "/%{prj.name}")

	files {
		"%{prj.name}/src/**.h",
		"%{prj.name}/src/**.cpp"
	}

	includedirs {
		"Pressia/vendor/spdlog/include",
		"Pressia/src"
	}

	links {
		"Pressia"
	}

	filter "system:windows"
		cppdialect "C++17"
		staticruntime "On"
		systemversion "latest"

		defines {
			"PS_PLATFORM_WINDOWS"
		}

	filter "configurations:Debug"
		defines "PS_DEBUG"
		symbols "On"

	filter "configurations:Release"
		defines "PS_RELEASE"
		symbols "On"

	filter "configurations:Dist"
		defines "PS_DIST"
		symbols "On"