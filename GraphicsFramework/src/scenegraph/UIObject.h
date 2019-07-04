#pragma once

#include "LayerObject.h"

class UIObject : public LayerObject
{
public:
	UIObject(const std::string& name);
	virtual ~UIObject();

	virtual void Update(float dTime) override = 0;
	virtual void Draw() override = 0;

private:

};