#include "EventBus.h"

template<typename Event>
void EventBus::Subscribe(Handler<Event> handler)
{
	auto& handlers = subscribers[typeid(Event)];
	handlers.push_back(
		[handler](const void* event) {
			handler(*static_cast<const Event*>(event))
		}
	);
}

template<typename Event>
void EventBus::Emit(const Event& event)
{
	auto it = subscribers.find(typeid(Event));

	if (it == subscribers.end())
		return;

	for (auto& handler : it->second)
	{
		handler(&event)
	}
}

template<typename Event>
void EventBus::Clear()
{
	subscribers.erase(typeid(Event));
}

