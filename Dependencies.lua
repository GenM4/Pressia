VULKAN_SDK = os.getenv("VULKAN_SDK")

--	Include Directories
IncludeDir = {}

IncludeDir["GLFW"] =					"%{wks.location}/Pressia/vendor/GLFW/include"

IncludeDir["GLAD"] =					"%{wks.location}/Pressia/vendor/GLAD/include"

IncludeDir["VulkanSDK"] =				"%{VULKAN_SDK}/Include"
IncludeDir["SPIRV_Cross"] =				"%{wks.location}/Pressia/vendor/SPIRV_Cross"
IncludeDir["shaderc"] =					"%{wks.location}/Pressia/vendor/shaderc/libshaderc/include"

IncludeDir["glm"] =						"%{wks.location}/Pressia/vendor/glm"
IncludeDir["entt"] =					"%{wks.location}/Pressia/vendor/entt/include"
IncludeDir["imgui"] =					"%{wks.location}/Pressia/vendor/imgui"
IncludeDir["imguizmo"] =				"%{wks.location}/Pressia/vendor/imguizmo"
IncludeDir["stb_image"] =				"%{wks.location}/Pressia/vendor/stb_image"
IncludeDir["yamlcpp"] =					"%{wks.location}/Pressia/vendor/yamlcpp/include"


--	Library Directories
LibraryDir = {}

LibraryDir["VulkanSDK"] =				"%{VULKAN_SDK}/Lib"
LibraryDir["VulkanSDK_Debug"] =			"%{wks.location}/Pressia/vendor/VulkanSDK/Lib"

Library = {}
Library["Vulkan"] =						"%{LibraryDir.VulkanSDK}/vulkan-1.lib"
Library["VulkanUtils"] =				"%{LibraryDir.VulkanSDK}/VkLayer_utils.lib"

Library["ShaderC_Debug"] =				"%{LibraryDir.VulkanSDK_Debug}/shaderc_sharedd.lib"
Library["SPIRV_Cross_Debug"] =			"%{LibraryDir.VulkanSDK_Debug}/spirv-cross-cored.lib"
Library["SPIRV_Cross_GLSL_Debug"] =		"%{LibraryDir.VulkanSDK_Debug}/spirv-cross-glsld.lib"
Library["SPIRV_Tools_Debug"] =			"%{LibraryDir.VulkanSDK_Debug}/SPIRV-Toolsd.lib"

Library["ShaderC_Release"] =			"%{LibraryDir.VulkanSDK_Debug}/shaderc_shared.lib"
Library["SPIRV_Cross_Release"] =		"%{LibraryDir.VulkanSDK_Debug}/spirv-cross-core.lib"
Library["SPIRV_Cross_GLSL_Release"] =	"%{LibraryDir.VulkanSDK_Debug}/spirv-cross-glsl.lib"