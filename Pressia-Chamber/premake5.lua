project "Pressia-Chamber"
	kind "ConsoleApp"
	language "C++"
	cppdialect "C++17"
	staticruntime "off"
	
	targetdir ("%{wks.location}/bin/" .. outputdir .. "/%{prj.name}")
	objdir ("%{wks.location}/bin-int/" .. outputdir .. "/%{prj.name}")

	files {
		"src/**.h",
		"src/**.cpp"
	}

	includedirs {
		"%{wks.location}/Pressia/vendor/spdlog/include",
		"%{wks.location}/Pressia/src",
		"%{wks.location}/Pressia/vendor",
		"%{IncludeDir.glm}",
		"%{IncludeDir.entt}"
	}

	links {
		"Pressia"
	}

	filter "system:windows"
		systemversion "latest"

	filter "configurations:Debug"
		defines {	"PS_DEBUG",
					"PS_ENABLE_ASSERTS" }
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