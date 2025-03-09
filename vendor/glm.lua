project "glm"
   kind "StaticLib"
   language "C"
   architecture "x64"

   targetdir "bin/%{cfg.buildcfg}"
   objdir "bin-int/%{cfg.buildcfg}"
   
   includedirs { "glm" }

   files
   {
      "glm/glm/**"
   }
    
   filter "system:windows"
      systemversion "latest"
      staticruntime "On"

      defines 
      { 
         "_GLM_WIN32",
         "_CRT_SECURE_NO_WARNINGS"
      }

   filter "configurations:Debug"
      runtime "Debug"
      symbols "on"

   filter "configurations:Release"
      runtime "Release"
      optimize "on"