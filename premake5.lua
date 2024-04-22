workspace "Yunomi"
    architecture "x64"

    configurations
    {
        "Debug",
        "Release"
    }

outputdir = "%{cfg.buildcfg}%{cfg.system}%{cfg.architecture}"

IncludeDir = {}
IncludeDir["GLFW"] = "Yunomi/vendor/GLFW/include"

include "Yunomi/vendor/GLFW"
--include "Yunomi/vendor/glm"                

project "Yunomi"
    location "Yunomi"
    kind "SharedLib"
    language "C++"

    targetdir ("bin/" .. outputdir .. "/%{prj.name}")
    objdir ("bin-int/" .. outputdir .. "/%{prj.name}")

    pchheader "pch.h"
    pchsource "Yunomi/src/pch.cpp"

    files
    {
        "%{prj.name}/src/**.h",
        "%{prj.name}/src/**.cpp",

    }

    includedirs
    {
        "%{prj.name}/src",
        "%{prj.name}/vendor/spdlog/include",
        "%{IncludeDir.GLFW}",
        "%{prj.name}/vendor/glm"
        
    }

    links
    {
        "GLFW",
        "opengl32.lib"
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

        postbuildcommands
        {
            "{RMDIR} ../bin/" .. outputdir .. "/Sandbox",
            "{MKDIR} ../bin/" .. outputdir .. "/Sandbox",
            "{COPY} %{cfg.buildtarget.relpath} ../bin/" .. outputdir .. "/Sandbox"
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

        targetdir ("bin/" .. outputdir .. "/%{prj.name}")
        objdir ("bin-int/" .. outputdir .. "/%{prj.name}")
    
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
            "Yunomi/vendor/glm"
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
    
        filter "configurations:Debug"
            defines { "YNM_DEBUG" }
            symbols "On"
    
        filter "configurations:Release"
            defines { "YNM_RELEASE" }
            optimize "On"