#include "OGL3Renderer3D.h"

#include "GLFW/glfw3.h"
#include "GL/glew.h"
#include "gtx/transform.hpp"

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

	OGL3Renderer3D::OGL3Renderer3D()
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
			{ ShaderSource::Type::VERTEX, ShaderSource::FromFile("./res/Renderer3D/basic.vert") },
			{ ShaderSource::Type::FRAGMENT, ShaderSource::FromFile("./res/Renderer3D/basic.frag") } }));

		const int MAX_SIZE = 10000;

		BufferLayout VerticesLayout({
			{ ShaderDataType::Float3, "a_Position" },
			{ ShaderDataType::Float2, "a_TexCoord" },
			{ ShaderDataType::Float3, "a_Normal" },
			{ ShaderDataType::Float3, "a_Tangent" } });

		m_VerticesVBO = VertexBuffer::Create(sizeof(MeshVertex) * MAX_SIZE);
		m_VerticesVBO->SetLayout(VerticesLayout);

		m_MaterailsVBO = VertexBuffer::Create(sizeof(int) * MAX_SIZE);
		m_MaterailsVBO->SetLayout(BufferLayout({ {ShaderDataType::Int, "a_MaterialId" } }));

		m_TransformationsVBO = VertexBuffer::Create(sizeof(int) * MAX_SIZE);
		m_TransformationsVBO->SetLayout(BufferLayout({ {ShaderDataType::Int, "a_TransformId" } }));

		m_IBO = IndexBuffer::Create(MAX_SIZE);
		
		std::vector<MeshVertex> MV{
					{ glm::vec3(0, 0.0f, 0.0f), glm::vec2(0.0f), glm::vec3(1.0f, 0.0f, 0.0f), glm::vec3(0.0f) },
					{ glm::vec3(0, 1.0f, 0.0f), glm::vec2(0.0f), glm::vec3(1.0f, 0.0f, 0.0f), glm::vec3(0.0f) },
					{ glm::vec3(0, 1.0f, 1.0f), glm::vec2(0.0f), glm::vec3(1.0f, 0.0f, 0.0f), glm::vec3(0.0f) } };
		std::vector<uint32_t> MI{ 0, 1, 2 };
		Ref<Mesh> RT = CreateRef<Mesh>(MV, MI);
		AddMesh(RT, CreateRef<Transformation>());

		m_VertexArray = VertexArray::Create();
		m_VertexArray->AddVertexBuffer(m_VerticesVBO);
		m_VertexArray->AddVertexBuffer(m_MaterailsVBO);
		m_VertexArray->AddVertexBuffer(m_TransformationsVBO);
		m_VertexArray->SetIndexBuffer(m_IBO);
	}

	void OGL3Renderer3D::AddMesh(Ref<Mesh> _Mesh, Ref<Transformation> _Transformation)
	{
		LOGI("Mesh added!");
		for (uint32_t Ind : _Mesh->GetIndices())
		{
			m_IndicesBuffer.push_back(Ind + m_VerticesBuffer.size());
		}

		m_VerticesBuffer.insert(m_VerticesBuffer.end(), _Mesh->GetVertices().begin(), _Mesh->GetVertices().end());
		
		for (int i = 0; i < _Mesh->GetVertices().size(); i++)
		{
			m_MaterialsBuffer.push_back(0);
			m_TransformationsBuffer.push_back(0);
		}

		m_VerticesVBO->SetData(m_VerticesBuffer.data(), sizeof(MeshVertex) * m_VerticesBuffer.size());
		m_MaterailsVBO->SetData(m_MaterialsBuffer.data(), sizeof(int) * m_MaterialsBuffer.size());
		m_TransformationsVBO->SetData(m_TransformationsBuffer.data(), sizeof(int) * m_TransformationsBuffer.size());
		m_IBO->SetIndices(m_IndicesBuffer.data(), m_IndicesBuffer.size());
	}

	void OGL3Renderer3D::Draw()
	{
		const float ClearColor[] = { 1.0, 0.0f, 0.0f, 1.0f };
		glClearColor(ClearColor[0], ClearColor[1], ClearColor[2], ClearColor[3]);
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT | GL_STENCIL_BUFFER_BIT);

		if (m_IBO->GetCount())
		{
			m_Shader->Enable();


			m_Shader->SetUniformMat4("u_ProjectionMatrix", glm::perspective(80.0f, 16.0f / 9.0f, 0.01f, 100.0f));
			glm::vec3 from = glm::rotate(float(glfwGetTime()) / 2.0f, glm::vec3(0.0f, 1.0f, 0.0f)) * glm::vec4(0.0f, 0.0f, 1.0f, 0.0f);
			m_Shader->SetUniformMat4("u_ViewMatrix", glm::lookAt(from, glm::vec3(0.0f, 0.0f, 0.0f), glm::vec3(0.0f, 1.0f, 0.0f)));
			m_Shader->SetUniform3f("u_ViewPos", from);
			//m_Shader->SetUniformMat4("u_ViewMatrix", camera->GetMaxtrix());
			//m_Shader->SetUniform3f("u_ViewPos", camera->getPosition());


			m_Shader->SetUniform3f("u_DirLight.direction", glm::vec3(-1.0f, 0.0f, 0.0f));
			m_Shader->SetUniform3f("u_DirLight.ambient", glm::vec3(0.1f));
			m_Shader->SetUniform3f("u_DirLight.diffuse", glm::vec3(0.7f));
			m_Shader->SetUniform3f("u_DirLight.specular", glm::vec3(1.0f));

			m_VertexArray->Bind();

			glDrawElements(GL_TRIANGLES, m_IBO->GetCount(), GL_UNSIGNED_INT, NULL);
		}
	}

}