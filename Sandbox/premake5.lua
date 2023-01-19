project "Sandbox"
	kind "ConsoleApp"
	language "C++"
	cppdialect "C++20"
	staticruntime "on"

	targetdir ("%{wks.location}/bin/" .. outputdir .. "/%{prj.name}")
	objdir ("%{wks.location}/bin-int/" .. outputdir .. "/%{prj.name}")

	files
	{
		"src/**.h",
		"src/**.hpp",
		"src/**.cpp",
		"src/**.c"
	}

	includedirs
	{
		"src",
		"%{wks.location}/Vega/src",
		"%{IncludeDir.glm}",
		"%{IncludeDir.glfw}",
		"%{IncludeDir.glew}",
		"%{IncludeDir.ImGui}"
	}

	links
	{
		"Vega"
	}

	filter "system:Windows"
		systemversion "latest"

	filter "configurations:Debug*"
		runtime "Debug"
		symbols "on"
		
	filter "configurations:Release*"
		runtime "Release"
		optimize "Speed"