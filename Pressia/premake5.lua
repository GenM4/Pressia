project "Pressia"
	kind "StaticLib"
	language "C++"
	cppdialect "C++17"
	staticruntime "off"

	targetdir ("%{wks.location}/bin/" .. outputdir .. "/%{prj.name}")
	objdir ("%{wks.location}/bin-int/" .. outputdir .. "/%{prj.name}")

	pchheader "pspch.h"
	pchsource "src/pspch.cpp"

	files {
		"src/**.h",
		"src/**.cpp",
		
		"vendor/stb_image/stb_image**.h",
		"vendor/stb_image/stb_image**.cpp",
		
		"vendor/glm/glm**.hpp",
		"vendor/glm/glm**.inl"
	}

	includedirs {
		"src",
		"vendor/spdlog/include",
		"%{IncludeDir.GLFW}",
		"%{IncludeDir.GLAD}",
		"%{IncludeDir.imgui}",
		"%{IncludeDir.glm}",
		"%{IncludeDir.stb_image}",
		"%{IncludeDir.entt}",
		"%{IncludeDir.yamlcpp}",
		"%{IncludeDir.imguizmo}",
		"%{IncludeDir.VulkanSDK}"
	}

	links {
		"GLFW",
		"GLAD",
		"ImGui",
		"ImGuizmo",
		"yamlcpp",
		"opengl32.lib"
	}

	defines {
			"_CRT_SECURE_NO_WARNINGS",
			"GLFW_INCLUDE_NONE"
	}

	filter "system:windows"
		systemversion "latest"

	filter "configurations:Debug"
		defines {
			"PS_DEBUG",
			"PS_ENABLE_ASSERTS"
		}
		symbols "On"
		runtime "Debug"

		links {
			"%{Library.ShaderC_Debug}",
			"%{Library.SPIRV_Cross_Debug}",
			"%{Library.SPIRV_Cross_GLSL_Debug}"
		}

	filter "configurations:Release"
		defines "PS_RELEASE"
		symbols "On"
		runtime "Release"
		optimize "On"

		links {
			"%{Library.ShaderC_Release}",
			"%{Library.SPIRV_Cross_Release}",
			"%{Library.SPIRV_Cross_GLSL_Release}"
		}

	filter "configurations:Dist"
		defines "PS_DIST"
		symbols "On"
		runtime "Release"
		optimize "On"

		links {
			"%{Library.ShaderC_Release}",
			"%{Library.SPIRV_Cross_Release}",
			"%{Library.SPIRV_Cross_GLSL_Release}"
		}
