project "glfw"
   kind "StaticLib"
   language "C"
   architecture "x64"
   
   targetdir "bin/%{cfg.buildcfg}"
   objdir "bin-int/%{cfg.buildcfg}"

   files
   {
      "glfw/include/GLFW/glfw3.h",
      "glfw/include/GLFW/glfw3native.h",
      "glfw/src/**.h",
      "glfw/src/**.c"
   }

   filter "system:windows"
      systemversion "latest"
      staticruntime "On"

      files
      {
         "glfw/src/win32_init.c",
         "glfw/src/win32_joystick.c",
         "glfw/src/win32_monitor.c",
         "glfw/src/win32_time.c",
         "glfw/src/win32_thread.c",
         "glfw/src/win32_window.c",
         "glfw/src/wgl_context.c",
         "glfw/src/egl_context.c",
         "glfw/src/osmesa_context.c"
      }

      defines 
      { 
         "_GLFW_WIN32",
         "_CRT_SECURE_NO_WARNINGS"
      }

   filter "configurations:Debug"
      defines { "DEBUG" }
      symbols "On"

   filter "configurations:Release"
      defines { "NDEBUG" }
      optimize "On"