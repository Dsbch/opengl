#pragma once

enum event_category {

};

enum event_type {
};

class event {
private:
	event_type m_type;
public:
	virtual ~event() = default;
	event(const event_type type) : m_type(type) {}
	event_type type() const { 
		typeid(12);
		return m_type; }
};
