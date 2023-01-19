#pragma once

#include "Event.h"

namespace LM
{

	class EventDispatcher
	{
	public:
		EventDispatcher(Ref<Event> _Event) : m_Event(_Event) { }

		template<typename T, typename F>
		bool Dispatch(const F& _Func)
		{
			if (m_Event->GetEventType() == T::GetStaticType())
			{
				m_Event->Handled = _Func(StaticRefCast<T>(m_Event));
				return true;
			}
			return false;
		}
	protected:
		Ref<Event> m_Event;
	};

}