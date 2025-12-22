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
	static void Subscribe(Handler<Event> handler);

	template<typename Event>
	static void Emit(const Event& event);

	template<typename Event>
	static void Clear();

private:
	using ErasedHandler = std::function<void(const void*)>;
	static std::unordered_map<std::type_index, std::vector<ErasedHandler>> subscribers;
};
