-- premake5.lua
workspace "opengl"
   location "%{_ACTION}"
   configurations { "Debug", "Release" }

   include "src/application.lua"
   include "vendor/glfw.lua"
   include "vendor/glad.lua"
   include "vendor/spdlog.lua"
   include "vendor/glm.lua"
