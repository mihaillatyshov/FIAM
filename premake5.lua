include "Dependencies.lua"

workspace "FIAM"
	architecture "x86_64"
	startproject "Sandbox"

	configurations
	{
		"Debug",
		"Release"
	}

	flags
	{
		"MultiProcessorCompile"
	}

outputdir = "%{cfg.buildcfg}-%{cfg.system}-%{cfg.architecture}"

include "Vega"
include "Sandbox"