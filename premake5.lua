workspace "Yunomi"
    architecture "x64"
    startproject "Sandbox"

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
    IncludeDir["Vulkan"] = "/usr/include"
end

include "Yunomi/vendor/GLFW"

project "Yunomi"
    location "Yunomi"
    kind "SharedLib"
    language "C++"
    cppdialect "C++17"

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
        libdirs { "C:/VulkanSDK/1.3.280.0/Lib" }
        links
        {
            "GLFW",
            "opengl32.lib",
            "vulkan-1.lib",
            "shaderc_shared.lib"
        }

    filter "system:linux"
        staticruntime "On"
        buildoptions { "-Wall" }
        links
        {
            "pthread",
            "dl",
            "X11",
            "Xrandr",
            "Xi",
            "Xxf86vm",
            "Xcursor",
            "Xinerama",
            "GL",
            "m",
            "vulkan",
            "shaderc_shared"
        }
        defines { "YNM_PLATFORM_LINUX" }
        systemversion "latest"

        -- Link GLFW static library with whole archive to pull in all symbols
        linkoptions {
            "-Wl,--whole-archive",
            "/home/zach/Source/Yunomi/Yunomi/vendor/GLFW/bin/Debug/linux/x86_64/GLFW/libGLFW.a",
            -- "Yunomi/vendor/GLFW/bin/Debug/linux/x86_64/GLFW/libGLFW.a",
            "-Wl,--no-whole-archive",
            "-Wl,--export-dynamic"
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
    cppdialect "C++17"

    targetdir ("bin/%{cfg.buildcfg}/%{cfg.system}/%{cfg.architecture}")
    objdir ("bin-int/%{cfg.buildcfg}/%{cfg.system}/%{cfg.architecture}/Sandbox")

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

    filter "system:linux"
        links
        {
            "Yunomi",      -- Link to your shared library
            "pthread",
            "dl",
            "vulkan",
            "shaderc_shared"
        }

    filter "system:windows"
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
        links
        {
            "Yunomi",
            "opengl32.lib",
            "vulkan-1.lib",
            "shaderc_shared.lib"
        }

    filter "configurations:Debug"
        defines { "YNM_DEBUG" }
        symbols "On"

    filter "configurations:Release"
        defines { "YNM_RELEASE" }
        optimize "On"
