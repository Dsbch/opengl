project "application"
   kind "ConsoleApp"
   language "C++"
   architecture "x64"

   targetdir "bin/%{cfg.buildcfg}"
   objdir "bin-int/%{cfg.buildcfg}"

   files {
      "**.hpp",
      "**.cpp",
      "**.c",
      "**.h",
   }

   pchheader ("pch.h")
   pchsource ("include/pch.cpp")

   includedirs { 
      "include",
      "../vendor/glfw/include", 
      "../vendor/spdlog/include", 
      "../vendor/glad/include",
      "../vendor/assimp/include",
      "../vendor/glm",
      "../vendor/stb/stb_image/include",
   }

   links { 
      "glad", 
      "glfw", 
      "spdlog",
      "glm",
   }

   -- only for assimp and zlib.
   libdirs { "../vendor/assimp" }
   
   filter "configurations:Debug"
      links {
         "assimp-vc143-mtd.lib",
         "zlibstaticd.lib",
      }


   filter "configurations:Release"
      links {
         "assimp-vc143-mt.lib",
         "zlibstatic.lib",
      }

   filter "configurations:Debug"
      defines { "DEBUG" }
      symbols "On"

   filter "configurations:Release"
      defines { "NDEBUG" }
      optimize "On"