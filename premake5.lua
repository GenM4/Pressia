include "Dependencies.lua"

workspace "Pressia"
	architecture "x86_64"
	startproject "Pressia-Chamber"

	configurations {
		"Debug",
		"Release",
		"Dist",
	}

	flags {
		"MultiProcessorCompile"
	}
	
	outputdir = "%{cfg.buildcfg}-%{cfg.system}-%{cfg.architecture}"

	group "Dependencies"
		include "Pressia/vendor/GLFW"
		include "Pressia/vendor/GLAD"
		include "Pressia/vendor/imgui"
		include "Pressia/vendor/yamlcpp"
		include "Pressia/vendor/imguizmo"

	group ""

	include "Pressia"
	include "Sandbox"
	include "Pressia-Chamber"