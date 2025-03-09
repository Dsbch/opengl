#pragma once
#include "event.h"

enum key {
	W,
	A,
	S,
	D,
};

enum input_event_type {
	KEY_UP,
	KEY_DOWN,
	MOUSE_MOVE,
};

class input_event : public event {
private:
	input_event_type m_input_type;
	key m_key;
public:
	input_event(event_type type, input_event_type input_type, key key) : event(type), m_input_type(input_type), m_key(key) {}
	input_event_type input_type() const { return m_input_type; }
	key key() const { return m_key; }
};
