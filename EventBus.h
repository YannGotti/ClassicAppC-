#pragma once
#include <unordered_map>
#include <vector>
#include <functional>
#include <typeindex>

class EventBus
{
public:
	template<typename Event>
	using Handler = std::function<void(const Event&)>;

	template<typename Event>
	static void Subscribe(Handler<Event> handler)
	{
		auto& handlers = subscribers[typeid(Event)];
		handlers.push_back(
			[handler](const void* event) {
				handler(*static_cast<const Event*>(event));
			}
		);
	}

	template<typename Event>
	static void Emit(const Event& event)
	{
		auto it = subscribers.find(typeid(Event));

		if (it == subscribers.end())
			return;

		for (auto& handler : it->second)
		{
			handler(&event);
		}
	}

	template<typename Event>
	static void Clear()
	{
		subscribers.erase(typeid(Event));
	}

private:
	using ErasedHandler = std::function<void(const void*)>;
	inline static std::unordered_map<std::type_index, std::vector<ErasedHandler>> subscribers;
};
