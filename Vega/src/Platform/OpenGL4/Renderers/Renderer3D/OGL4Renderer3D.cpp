#include "OGL4Renderer3D.h"

#include <GLFW/glfw3.h>
#include <GL/glew.h>
#include <gtx/transform.hpp>

#include "Vega/Events/WindowEvent.h"
#include "Vega/Events/EventDispatcher.h"

namespace LM
{

	static void message_callback(GLenum source, GLenum type, GLuint id, GLenum severity, GLsizei length, GLchar const* message, void const* user_param)
	{
		auto const src_str = [source]() {
			switch (source)
			{
			case GL_DEBUG_SOURCE_API: return "API";
			case GL_DEBUG_SOURCE_WINDOW_SYSTEM: return "WINDOW SYSTEM";
			case GL_DEBUG_SOURCE_SHADER_COMPILER: return "SHADER COMPILER";
			case GL_DEBUG_SOURCE_THIRD_PARTY: return "THIRD PARTY";
			case GL_DEBUG_SOURCE_APPLICATION: return "APPLICATION";
			case GL_DEBUG_SOURCE_OTHER: return "OTHER";
			}
		}();

		auto const type_str = [type]() {
			switch (type)
			{
			case GL_DEBUG_TYPE_ERROR: return "ERROR";
			case GL_DEBUG_TYPE_DEPRECATED_BEHAVIOR: return "DEPRECATED_BEHAVIOR";
			case GL_DEBUG_TYPE_UNDEFINED_BEHAVIOR: return "UNDEFINED_BEHAVIOR";
			case GL_DEBUG_TYPE_PORTABILITY: return "PORTABILITY";
			case GL_DEBUG_TYPE_PERFORMANCE: return "PERFORMANCE";
			case GL_DEBUG_TYPE_MARKER: return "MARKER";
			case GL_DEBUG_TYPE_OTHER: return "OTHER";
			}
		}();

		auto const severity_str = [severity]() {
			switch (severity) {
			case GL_DEBUG_SEVERITY_NOTIFICATION: return "NOTIFICATION";
			case GL_DEBUG_SEVERITY_LOW: return "LOW";
			case GL_DEBUG_SEVERITY_MEDIUM: return "MEDIUM";
			case GL_DEBUG_SEVERITY_HIGH: return "HIGH";
			}
		}();
		LOGE(src_str, ":", type_str, ":", severity_str, ":", id, ":", message);
	}

	OGL4Renderer3D::OGL4Renderer3D()
	{
		glEnable(GL_DEBUG_OUTPUT);
		glDebugMessageCallback(message_callback, nullptr);
		glDebugMessageControl(GL_DONT_CARE, GL_DONT_CARE, GL_DEBUG_SEVERITY_NOTIFICATION, 0, nullptr, GL_FALSE);

		m_Shader = Shader::Create(ShaderLayout({
			{ ShaderDataType::Float3, "a_Position" },
			{ ShaderDataType::Float2, "a_TexCoord" },
			{ ShaderDataType::Float3, "a_Normal" },
			{ ShaderDataType::Float3, "a_Tangent" },
			{ ShaderDataType::Int, "a_MaterialId" },
			{ ShaderDataType::Int, "a_TransformId"} }, {
			{ ShaderSource::Type::VERTEX, ShaderSource::FromFile("./res/Renderer3D/OGL4Basic.vert") },
			{ ShaderSource::Type::FRAGMENT, ShaderSource::FromFile("./res/Renderer3D/OGL4Basic.frag") } }));

		const int MAX_SIZE = 10'000'000;

		BufferLayout VerticesLayout({
			{ ShaderDataType::Float3, "a_Position" },
			{ ShaderDataType::Float2, "a_TexCoord" },
			{ ShaderDataType::Float3, "a_Normal" },
			{ ShaderDataType::Float3, "a_Tangent" } });

		m_VerticesVBO = VertexBuffer::Create(sizeof(MeshVertex) * MAX_SIZE);
		m_VerticesVBO->SetLayout(VerticesLayout);

		m_MaterailsVBO = VertexBuffer::Create(sizeof(int) * MAX_SIZE);
		m_MaterailsVBO->SetLayout(BufferLayout({ {ShaderDataType::Int, "a_MaterialId" } }, true));

		m_TransformationsVBO = VertexBuffer::Create(sizeof(int) * MAX_SIZE);
		m_TransformationsVBO->SetLayout(BufferLayout({ BufferElement(ShaderDataType::Int, "a_TransformId") }, true));

		m_IBO = IndexBuffer::Create(MAX_SIZE);

		m_VertexArray = VertexArray::Create();
		m_VertexArray->AddVertexBuffer(m_VerticesVBO);
		m_VertexArray->AddVertexBuffer(m_MaterailsVBO);
		m_VertexArray->AddVertexBuffer(m_TransformationsVBO);
		m_VertexArray->SetIndexBuffer(m_IBO);

		m_FrameBuffer = FrameBuffer::Create(FrameBufferProps::FromWindow({ FrameBufferColorMASK::NONE }, FrameBufferMASK::DEPTH));

		m_MaterialsSSBO = ShaderStorageBuffer::Create(1000 * sizeof(uint64_t));

		m_ScreenFrameBuffer = FrameBuffer::Create(FrameBufferProps{
			Application::Get()->GetWindow()->GetWidth(),
			Application::Get()->GetWindow()->GetHeight(), WindowMSAA::None, {{ FrameBufferColorMASK::NONE }} });

		m_ScreenShader = Shader::Create(ShaderLayout({
			{ ShaderDataType::Float2, "a_Position" } }, {
			{ ShaderSource::Type::VERTEX, ShaderSource::FromFile("./res/Renderer3D/Screen.vert") },
			{ ShaderSource::Type::FRAGMENT, ShaderSource::FromFile("./res/Renderer3D/Screen.frag") } }));


		glm::vec2 pos(10.0f, 1.0f);

		std::vector<glm::vec2> ScreenBuffer{ { 0.0f, 0.0f }, { 1.0f, 0.0f }, { 1.0f, 1.0f }, { 0.0f, 1.0f } };
		std::vector<uint32_t> ScreenIndices{ 0, 1, 2, 0, 2, 3 };
		Ref<VertexBuffer> ScreenVBO = VertexBuffer::Create(ScreenBuffer.data(), ScreenBuffer.size() * sizeof(glm::vec2));
		ScreenVBO->SetLayout({{ ShaderDataType::Float2, "a_Position" } });
		Ref<IndexBuffer> ScreenIBO = IndexBuffer::Create(ScreenIndices.data(), ScreenIndices.size());
		m_ScreenVertexArray = VertexArray::Create();
		m_ScreenVertexArray->AddVertexBuffer(ScreenVBO);
		m_ScreenVertexArray->SetIndexBuffer(ScreenIBO);
	}

	void OGL4Renderer3D::AddMesh(Ref<Mesh> _Mesh, Ref<Transformation> _Transformation)
	{
		LOGI("Mesh added!");
		m_Commands.emplace_back(_Mesh->GetIndices().size(), 1, m_IndicesBuffer.size(), m_VerticesBuffer.size(), m_Meshes.size());
		
		m_IndicesBuffer.insert(m_IndicesBuffer.end(), _Mesh->GetIndices().begin(), _Mesh->GetIndices().end());
		m_VerticesBuffer.insert(m_VerticesBuffer.end(), _Mesh->GetVertices().begin(), _Mesh->GetVertices().end());

		m_MaterialsBuffer.push_back(m_Meshes.size());
		m_TransformationsBuffer.push_back(m_Meshes.size());

		m_Meshes.push_back(_Mesh);
		m_MeshesNeedUpdate = true;
	}

	void OGL4Renderer3D::LoadTexture(Ref<Texture2D> _Texture)
	{
		_Texture->OnAttach();
		m_Materials.push_back((uint64_t)_Texture->GetHandle());
		m_Materials.push_back(0u);
		m_Materials.push_back(0u);
		m_Materials.push_back(0u);
		m_MaterialsSSBO->SetData(m_Materials.data(), m_Materials.size() * sizeof(uint64_t));
		m_Textures.push_back(_Texture);
	}

	void OGL4Renderer3D::OnEvent(const Ref<Event>& _Event)
	{
		EventDispatcher EDisp(_Event);
		EDisp.Dispatch<WindowResizeEvent>([&](Ref<WindowResizeEvent> E) {
			m_FrameBuffer->Resize(E->GetWidth(), E->GetHeight());
			m_ScreenFrameBuffer->Resize(E->GetWidth(), E->GetHeight());
			return false;
			});
	}

	void OGL4Renderer3D::Clear() {
		LOGI("OGL4Renderer3D CLEAR");
		for (auto& texture : m_Textures)
		{
			texture->OnDetach();
		}
		m_Textures.clear();
		m_Materials.clear();
		m_MaterialsSSBO->SetData(nullptr, 0);
		
		m_Commands.clear();
		m_IndicesBuffer.clear();
		m_VerticesBuffer.clear();
		m_MaterialsBuffer.clear();
		m_TransformationsBuffer.clear();
		m_Meshes.clear();
		m_MeshesNeedUpdate = true;

	}

	void OGL4Renderer3D::Draw()
	{
		if (m_MeshesNeedUpdate)
		{
			m_VerticesVBO->SetData(m_VerticesBuffer.data(), sizeof(MeshVertex) * m_VerticesBuffer.size());
			m_MaterailsVBO->SetData(m_MaterialsBuffer.data(), sizeof(int) * m_MaterialsBuffer.size());
			m_TransformationsVBO->SetData(m_TransformationsBuffer.data(), sizeof(int) * m_TransformationsBuffer.size());
			m_IBO->SetIndices(m_IndicesBuffer.data(), m_IndicesBuffer.size());
			m_MeshesNeedUpdate = false;
		}

		for (int i = m_Textures.size() - 1; i >= 0; i--)
		{
			if (m_Textures[i].use_count() == 1)
			{
				LOGI("Remove Texture: ", m_Textures[i].get());
				m_Textures.erase(m_Textures.begin() + i);
			}
		}

		const float ClearColor[] = { 0.8, 0.2f, 0.4f, 1.0f };
		glClearColor(ClearColor[0], ClearColor[1], ClearColor[2], ClearColor[3]);
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT | GL_STENCIL_BUFFER_BIT);
		m_FrameBuffer->Clear(glm::vec4(1.0f, 0.0f, 0.0f, 1.0f));
		m_FrameBuffer->Bind();

		glEnable(GL_DEPTH_TEST);
		if (m_IBO->GetCount())
		{
			m_Shader->Enable();
			m_MaterialsSSBO->Bind(1);
			
			for (const auto& Tex : m_Textures)
			{
				Tex->Bind(0);
			}

			m_Shader->SetUniformMat4("u_ProjectionMatrix", glm::perspective(80.0f, 16.0f / 9.0f, 0.01f, 100.0f));
			m_Shader->SetUniformMat4("u_ProjectionMatrix", glm::ortho(-0.5f, 0.5f, -0.5f, 0.5f));
			//glm::vec3 From = glm::rotate(float(glfwGetTime()) / 2.0f, glm::vec3(0.0f, 1.0f, 0.0f)) * glm::vec4(0.0f, 0.0f, 10.0f, 0.0f);
			glm::vec3 From = glm::rotate(glm::radians(90.0f), glm::vec3(0.0f, 1.0f, 0.0f)) * glm::vec4(0.0f, 0.0f, 0.5f, 0.0f);
			m_Shader->SetUniformMat4("u_ViewMatrix", glm::lookAt(From, glm::vec3(0.0f, 0.0f, 0.0f), glm::vec3(0.0f, 1.0f, 0.0f)));
			m_Shader->SetUniform3f("u_ViewPos", From);
			//m_Shader->SetUniformMat4("u_ViewMatrix", camera->GetMaxtrix());
			//m_Shader->SetUniform3f("u_ViewPos", camera->getPosition());


			m_Shader->SetUniform3f("u_DirLight.direction", glm::vec3(-1.0f, 0.0f, 0.0f));
			m_Shader->SetUniform3f("u_DirLight.ambient", glm::vec3(0.1f));
			m_Shader->SetUniform3f("u_DirLight.diffuse", glm::vec3(0.7f));
			m_Shader->SetUniform3f("u_DirLight.specular", glm::vec3(1.0f));

			m_VertexArray->Bind();

			//glDrawElements(GL_TRIANGLES, m_IBO->GetCount(), GL_UNSIGNED_INT, NULL);
			glMultiDrawElementsIndirect(GL_TRIANGLES, GL_UNSIGNED_INT, m_Commands.data(), m_Commands.size(), 0);
		}
		m_FrameBuffer->Unbind();
		glBlitNamedFramebuffer(
			(uint32_t)m_FrameBuffer->GetId(), (uint32_t)m_ScreenFrameBuffer->GetId(),
			0, 0, m_FrameBuffer->GetWidth(), m_FrameBuffer->GetHeight(), 
			0, 0, m_ScreenFrameBuffer->GetWidth(), m_ScreenFrameBuffer->GetHeight(), GL_COLOR_BUFFER_BIT, GL_LINEAR);
		//glViewport(0, 0, Application::Get()->GetWindow()->GetWidth(), Application::Get()->GetWindow()->GetHeight());
		
	}

}