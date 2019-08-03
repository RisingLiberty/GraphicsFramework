#pragma once

#include "Event.h"

enum class API;

class SwitchApiEvent : public Event
{
public:
	SwitchApiEvent(API api);
	virtual ~SwitchApiEvent();

	API GetApi() const;

private:
	API m_api;
};