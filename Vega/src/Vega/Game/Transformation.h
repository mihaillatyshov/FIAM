#pragma once

#include <mutex>

#include <glm.hpp>

namespace LM
{

	class Transformation
	{
	public:
		Transformation();

		const glm::vec3 GetPosition()	const { return m_Position; }
		const glm::vec3 GetRotation()	const { return m_Rotation; }
		const glm::vec3 GetScale()		const { return m_Scale; }

		void SetPosition(const glm::vec3 _Position) { m_Position = _Position; }
		void SetRotation(const glm::vec3 _Rotation) { m_Rotation = _Rotation; }
		void SetScale	(const glm::vec3 _Scale)	{ m_Scale	 = _Scale; }
		
		std::unique_lock<std::mutex> CreateLock() const { return std::move(std::unique_lock(m_Mtx)); }
	protected:
		glm::vec3 m_Position;
		glm::vec3 m_Rotation;
		glm::vec3 m_Scale;
		mutable std::mutex m_Mtx;
	};

}