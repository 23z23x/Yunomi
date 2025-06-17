workspace "Yunomi"
    architecture "x64"

    configurations
    {
        "Debug",
        "Release"
    }

IncludeDir = {}

IncludeDir["GLFW"] = "Yunomi/vendor/GLFW/include"
if os.host() == "windows" then
    IncludeDir["Vulkan"] = "C:/VulkanSDK/1.3.280.0/Include"
else
    IncludeDir["Vulkan"] = "/usr/include" -- Vulkan installed via pacman
end

include "Yunomi/vendor/GLFW"             

project "Yunomi"
    location "Yunomi"
    kind "SharedLib"
    language "C++"

    targetdir ("bin/%{cfg.buildcfg}/%{cfg.system}/%{cfg.architecture}/%{prj.name}")
    objdir ("bin-int/%{cfg.buildcfg}/%{cfg.system}/%{cfg.architecture}/%{prj.name}")

    pchheader "pch.h"
    pchsource "Yunomi/src/pch.cpp"

    files
    {
        "%{prj.name}/src/**.h",
        "%{prj.name}/src/**.cpp",
        "%{prj.name}/vendor/glm/glm/**.hpp",
        "%{prj.name}/vendor/glm/glm/**.inl",
        "%{prj.name}/vendor/stb_image.h",
        "%{prj.name}/vendor/tiny_obj_loader.h"
    }

    includedirs
    {
        "%{prj.name}/src",
        "%{prj.name}/vendor/spdlog/include",
        "%{IncludeDir.GLFW}",
        "%{prj.name}/vendor/glm",
        "%{IncludeDir.Vulkan}",
        "%{prj.name}/vendor"
    }

    filter "system:windows"
        libdirs { "C:/VulkanSDK/1.3.280.0/Lib" }
        links 
        {
            "GLFW",
            "opengl32.lib",
            "vulkan-1.lib",
            "shaderc_shared.lib"
        }

    filter "system:linux"
        links 
        {
            "GL",
            "GLFW",
            "vulkan",
            "dl",
            "pthread"
        }

    filter "system:windows"
        cppdialect "C++17"
        staticruntime "On"
        systemversion "latest"

        defines
        {
            "YNM_PLATFORM_WINDOWS",
            "YNM_BUILD_DLL",
            "_SILENCE_ALL_MS_EXT_DEPRECATION_WARNINGS",
            "_SILENCE_STDEXT_ARR_ITERS_DEPRECATION_WARNING",
            "_WINDLL",
            "_UNICODE",
            "UNICODE"
        }

    filter "system:linux"
        cppdialect "C++17"
        systemversion "latest"
        buildoptions { "-Wall" }

    filter "system:windows"
        postbuildcommands
        {
            "{RMDIR} ../bin/%{cfg.buildcfg}/%{cfg.system}/%{cfg.architecture}/Sandbox",
            "{MKDIR} ../bin/%{cfg.buildcfg}/%{cfg.system}/%{cfg.architecture}/Sandbox",
            "{COPY} %{cfg.buildtarget.relpath} ../bin/%{cfg.buildcfg}/%{cfg.system}/%{cfg.architecture}/Sandbox"
        }

    filter "system:linux"
        postbuildcommands 
        {
            "{COPY} %{cfg.buildtarget.relpath} ../bin/%{cfg.buildcfg}/%{cfg.system}/%{cfg.architecture}/Sandbox/"
        }

    filter "configurations:Debug"
        defines { "YNM_DEBUG" }
        symbols "On"

    filter "configurations:Release"
        defines { "YNM_RELEASE" }
        optimize "On"

project "Sandbox"
    location "Sandbox"
    kind "ConsoleApp"
    language "C++"

    targetdir ("bin/%{cfg.buildcfg}/%{cfg.system}/%{cfg.architecture}/%{prj.name}")
    objdir ("bin-int/%{cfg.buildcfg}/%{cfg.system}/%{cfg.architecture}/%{prj.name}")

    files
    {
        "%{prj.name}/src/**.h",
        "%{prj.name}/src/**.cpp",
        "%{prj.name}/vendor/glm/glm/**.hpp",
        "%{prj.name}/vendor/glm/glm/**.inl"
    }

    includedirs
    {
        "Yunomi/vendor/spdlog/include",
        "Yunomi/src",
        "%{IncludeDir.GLFW}",
        "%{IncludeDir.Vulkan}",
        "Yunomi/vendor/glm",
        "Yunomi/vendor"
    }

    links
    {
        "Yunomi"
    }

    filter "system:windows"
        cppdialect "C++17"
        staticruntime "On"
        systemversion "latest"

        defines
        {
            "YNM_PLATFORM_WINDOWS",
            "_SILENCE_ALL_MS_EXT_DEPRECATION_WARNINGS",
            "_SILENCE_STDEXT_ARR_ITERS_DEPRECATION_WARNING",
            "_UNICODE",
            "UNICODE"
        }

    filter "system:linux"
        cppdialect "C++17"
        systemversion "latest"
        buildoptions { "-Wall" }

    filter "configurations:Debug"
        defines { "YNM_DEBUG" }
        symbols "On"

    filter "configurations:Release"
        defines { "YNM_RELEASE" }
        optimize "On"
