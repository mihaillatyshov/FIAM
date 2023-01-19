#pragma once

#include <queue>
#include <mutex>

#include "Event.h"

namespace LM
{

	class EventQueue
	{
	public:
		EventQueue() = default;

		void Add(const Ref<Event>& _Event)
		{
			std::unique_lock Lock(m_Mutex);
			m_Events.push(_Event);
		}

		Ref<Event> TryPop()
		{
			std::unique_lock Lock(m_Mutex);
			if (m_Events.empty())
				return Ref<Event>();
			
			Ref<Event> Res = m_Events.front();
			m_Events.pop();
			return Res;
		}

	protected:
		std::queue<Ref<Event>> m_Events;
		std::mutex m_Mutex;
	};

}