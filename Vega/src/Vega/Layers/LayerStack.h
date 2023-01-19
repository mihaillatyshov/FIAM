#pragma once

#include <vector>

#include "Vega/Core/Base.h"
#include "Layer.h"

namespace LM
{
	/**
	 * Adds to each component of the rotator.
	 *
	 * @param DeltaPitch Change in pitch. (+/-)
	 * @param DeltaYaw Change in yaw. (+/-)
	 * @param DeltaRoll Change in roll. (+/-)
	 * @return Copy of rotator after addition.
	 */

	class LayerStack
	{
	public:
		LayerStack() = default;
		~LayerStack();

		// Add Layer in stack
		void PushLayer	(Ref<Layer> _Layer);
		
		// Add Layer in stack over all layers
		void PushOverlay(Ref<Layer> _Layer);
		void PopLayer	(Ref<Layer> _Layer);
		void PopOverpaly(Ref<Layer> _Layer);
		//void PopLayer	(Layer* _Layer);
		//void PopOverpaly(Layer* _Layer);

		void OnAttach();
		void OnDetach();
		void OnUpdate();
		void OnEvent(Ref<Event> _Event);
	protected:
		std::vector<Ref<Layer>> m_Layers;
		uint32_t m_LayersInsertIndex = 0;
	};

}