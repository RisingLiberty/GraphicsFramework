#include "stdafx.h"

#include "SwitchApiEvent.h"
#include "graphics/API.h"

SwitchApiEvent::SwitchApiEvent(API api) :
	m_api(api)
{

}

SwitchApiEvent::~SwitchApiEvent()
{

}

API SwitchApiEvent::GetApi() const
{
	return m_api;
}