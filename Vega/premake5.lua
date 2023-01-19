group "Dependencies"
	include "Vega/vendor/glfw"
	include "Vega/vendor/glew"
	include "Vega/vendor/imgui"
group ""

project "Vega"
	kind "StaticLib"
	language "C++"
	cppdialect "C++20"
	staticruntime "on"

	targetdir ("%{wks.location}/bin/" .. outputdir .. "/%{prj.name}")
	objdir ("%{wks.location}/bin-int/" .. outputdir .. "/%{prj.name}")

	include "premakeconfig.lua"
	BUILD_OGL = BUILD_OGL2 or BUILD_OGL3 or BUILD_OGL4

	files
	{
		"src/Vega/**.h",
		"src/Vega/**.hpp",
		"src/Vega/**.cpp",
		"src/Vega/**.c"
	}

	defines
	{
		"GLFW_INCLUDE_NONE",
		"GLEW_STATIC"
	}

	includedirs
	{
		"src",
		"%{IncludeDir.glfw}",
		"%{IncludeDir.glew}",
		"%{IncludeDir.ImGui}",
		"%{IncludeDir.glm}",
		"%{IncludeDir.stb_image}",
	}

	links
	{
		"glfw",
		"glew",
		"ImGui"
	}

	if BUILD_OGL then
		defines
		{
			"OPENGL"
		}

		files
		{
			"src/Platform/OpenGL/**.h",
			"src/Platform/OpenGL/**.hpp",
			"src/Platform/OpenGL/**.cpp",
			"src/Platform/OpenGL/**.c",
			"%{IncludeDir.ImGui}/backends/imgui_impl_glfw.h",
			"%{IncludeDir.ImGui}/backends/imgui_impl_glfw.cpp",
		}
	end

	if BUILD_OGL2 then
		defines
		{
			"OPENGL2"
		}

		files
		{
			"src/Platform/OpenGL2/**.h",
			"src/Platform/OpenGL2/**.hpp",
			"src/Platform/OpenGL2/**.cpp",
			"src/Platform/OpenGL2/**.c",
			"%{IncludeDir.ImGui}/backends/imgui_impl_opengl2.h",
			"%{IncludeDir.ImGui}/backends/imgui_impl_opengl2.cpp",
			
		}
	end

	if BUILD_OGL3 then
		defines
		{
			"OPENGL3"
		}

		files
		{
			"src/Platform/OpenGL3/**.h",
			"src/Platform/OpenGL3/**.hpp",
			"src/Platform/OpenGL3/**.cpp",
			"src/Platform/OpenGL3/**.c",
			"%{IncludeDir.ImGui}/backends/imgui_impl_opengl3.h",
			"%{IncludeDir.ImGui}/backends/imgui_impl_opengl3.cpp",
		}
	end

	if BUILD_OGL4 then
		defines
		{
			"OPENGL4"
		}

		files
		{
			"src/Platform/OpenGL4/**.h",
			"src/Platform/OpenGL4/**.hpp",
			"src/Platform/OpenGL4/**.cpp",
			"src/Platform/OpenGL4/**.c",
			"%{IncludeDir.ImGui}/backends/imgui_impl_opengl3.h",
			"%{IncludeDir.ImGui}/backends/imgui_impl_opengl3.cpp",
		}
	end

	filter "system:Windows"
		systemversion "latest"

		defines
		{
			"_CRT_SECURE_NO_WARNINGS",
			"WIN32",
			"__wglew_h__"
		}
		
		links
		{
			"opengl32.lib"
		}

		files
		{
			"src/Platform/Windows/**.h",
			"src/Platform/Windows/**.hpp",
			"src/Platform/Windows/**.cpp",
			"src/Platform/Windows/**.c"
		}

	filter "configurations:*Debug*"
		runtime "Debug"
		symbols "on"

		defines
		{
			"DEBUG"
		}

	filter "configurations:*Release*"
		runtime "Release"
		optimize "Speed"

		defines
		{
			"RELEASE",
			"NDEBUG"
		}

	