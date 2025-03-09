#pragma once
#include "event.h"

class event_dispatcher
{
private:
	std::mutex m_dispatcher_lock;
public:
	event_dispatcher() = delete;
	
	template<class T>
	void add_handler(std::function<void(T&)>);
};

template<class T>
inline void event_dispatcher::add_handler(std::function<void(T&)>)
{
	const std::lock_guard<std::mutex> lock(m_dispatcher_lock);
	
	static std::map<std::size_t, std::vector<std::function<void(T)>>> typed_handlers;
}

