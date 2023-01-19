#pragma once

#include "Vega/Core/Base.h"
#include "Vega/Core/Application.h"

extern LM::Application* CreateApplication();

int main()
{
	auto App = CreateApplication();

	App->Run();

	delete App;

	return 0;
}