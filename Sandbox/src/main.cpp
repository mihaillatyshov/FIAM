#include "Vega/Core/EntryPoint.h"
#include "Vega/Layers/Layer3D.h"

class Sandbox : public LM::Application
{
public:
	Sandbox()
	{
		m_LayerStack->PushLayer(LM::CreateRef<LM::Layer3D>());
	}

	~Sandbox() = default;
};

LM::Application* CreateApplication()
{
	return new Sandbox;
}