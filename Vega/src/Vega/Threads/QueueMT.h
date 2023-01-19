#pragma once

#include <mutex>

#include "Vega/Core/Base.h"

namespace LM
{

	template<typename T>
	class QueueMT
	{
	protected:
		struct Node
		{
			Ref<T> Data;
			Scope<Node> Next;
		};
	public:
		QueueMT() : m_Head(new Node), m_Tail(m_Head.get()) { }
		QueueMT(const QueueMT& _Other) = delete;
		QueueMT& operator=(const QueueMT& _Other) = delete;

		Ref<T> TryPop()
		{
			Scope<Node> OldHead = TryPopHead();
			return OldHead ? OldHead->Data : Ref<T>();
		}

		bool TryPop(T& _Value)
		{
			Scope<Node> const OldHead = TryPopHead(_Value);
			return OldHead ? true : false;
		}

		void Push(T _NewValue)
		{
			Ref<T> NewData = CreateRef<T>(std::move(_NewValue));
			Scope<Node> NewNode(new Node);
			Node* const NewTail = NewNode.get();
			std::lock_guard TailLock(m_TailMutex);
			m_Tail->Data = NewData;
			m_Tail->Next = std::move(NewNode);
			m_Tail = NewTail;
		}
	protected:
		Node* GetTail()
		{
			std::lock_guard TailLock(m_TailMutex);
			return m_Tail;
		}

		Scope<Node> PopHead()
		{
			Scope<Node> OldHead = std::move(m_Head);
			m_Head = std::move(OldHead->Next);
			return OldHead;
		}

		Scope<Node> TryPopHead()
		{
			std::lock_guard HeadLock(m_HeadMutex);
			if (m_Head.get() == GetTail())
			{
				return Scope<Node>();
			}
			return PopHead();
		}

		Scope<Node> TryPopHead(T& _Value)
		{
			std::lock_guard HeadLock(m_HeadMutex);
			if (m_Head.get() == GetTail())
			{
				return Scope<Node>();
			}
			_Value = std::move(*m_Head->Data);
			return PopHead();
		}
	protected:
		std::mutex m_HeadMutex;
		Scope<Node> m_Head;
		std::mutex m_TailMutex;
		Node* m_Tail;
	};

}