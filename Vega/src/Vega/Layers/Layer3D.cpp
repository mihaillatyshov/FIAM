#include "Layer3D.h"

#include "Vega/Events/EventDispatcher.h"
#include "Vega/Events/WindowEvent.h"
#include "Vega/Events/KeyEvent.h"
#include "Vega/Renderables/Mesh.h"
#include "Vega/Core/Application.h"
#include "Vega/Debug/ConsoleLog.h"

namespace LM
{

	void Layer3D::OnAttach()
	{
		m_FIAM = FIAM::Get();


		Tex = Texture2D::Create("./res/Textures/CDRAGON.jpg");
		Tex->Add(Tex, "ta", Application::Get()->GetRendererCommand());
		std::vector<MeshVertex> MV{
			{ glm::vec3(0.0f, -0.5f, -0.5f), glm::vec2(1.0f, 1.0f), glm::vec3(1.0f, 0.0f, 0.0f), glm::vec3(0.0f) },
			{ glm::vec3(0.0f,  0.5f, -0.5f), glm::vec2(1.0f, 0.0f), glm::vec3(1.0f, 0.0f, 0.0f), glm::vec3(0.0f) },
			{ glm::vec3(0.0f,  0.5f,  0.5f), glm::vec2(0.0f, 0.0f), glm::vec3(1.0f, 0.0f, 0.0f), glm::vec3(0.0f) },
			{ glm::vec3(0.0f, -0.5f,  0.5f), glm::vec2(0.0f, 1.0f), glm::vec3(1.0f, 0.0f, 0.0f), glm::vec3(0.0f) }, };
		std::vector<uint32_t> MI{ 0, 1, 2, 0, 2, 3 };
		Ref<Mesh> RT = CreateRef<Mesh>(MV, MI);
		RT->Add(RT, CreateRef<Transformation>(), Application::Get()->GetRendererCommand());
	}

	void Layer3D::OnEvent(Ref<Event> _Event)
	{
		EventDispatcher EDisp(_Event);
		EDisp.Dispatch<KeyPressedEvent>([&](Ref<KeyPressedEvent> E) {
			
			if (E->GetKeyCode() == Key::W) { m_MoveUp = true; }
			return true;
		});
		EDisp.Dispatch<KeyReleasedEvent>([&](Ref<KeyReleasedEvent> E) {

			if (E->GetKeyCode() == Key::W) { m_MoveUp = false; }
			return true;
		});
	}

}