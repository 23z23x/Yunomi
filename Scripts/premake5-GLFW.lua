project "GLFW"
    kind "StaticLib"
    language "C"

    targetdir ("bin/%{cfg.buildcfg}/%{cfg.system}/%{cfg.architecture}/%{prj.name}")
    objdir ("bin-int/%{cfg.buildcfg}/%{cfg.system}/%{cfg.architecture}/%{prj.name}")

    includedirs { "include" }

    files {
        "include/GLFW/glfw3.h",
        "include/GLFW/glfw3native.h",
        "src/internal.h",
        "src/mappings.h",
        "src/platform.h",
        "src/context.c",
        "src/init.c",
        "src/input.c",
        "src/monitor.c",
        "src/platform.c",
        "src/vulkan.c",
        "src/window.c"
    }

    filter "system:linux"
        pic "On"
        staticruntime "On"
        systemversion "latest"

        files {
            "src/x11_init.c",
            "src/x11_monitor.c",
            "src/x11_window.c",
            "src/x11_platform.h",
            "src/xkb_unicode.c",
            "src/xkb_unicode.h",
            "src/posix_time.c",
            "src/posix_thread.c",
            "src/posix_module.c",
            "src/linux_joystick.c",
            "src/linux_joystick.h"
        }

        defines { "_GLFW_X11" }
        links { "X11", "pthread", "dl", "m" }

    filter "system:windows"
        buildoptions { "-std=c11", "-lgdi32" }
        systemversion "latest"
        staticruntime "On"

        files {
            "src/win32_init.c",
            "src/win32_joystick.c",
            "src/win32_monitor.c",
            "src/win32_time.c",
            "src/win32_thread.c",
            "src/win32_window.c",
            "src/win32_module.c",
            "src/wgl_context.c",
            "src/egl_context.c",
            "src/osmesa_context.c"
        }

        defines {
            "_GLFW_WIN32",
            "_CRT_SECURE_NO_WARNINGS"
        }

    filter { "system:windows", "configurations:Release" }
        buildoptions "/MT"
