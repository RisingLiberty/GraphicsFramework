#pragma once

#include "core/application.h"

class Win64Application : public Application
{
public:
	Win64Application();
	~Win64Application();

	void OnEvent(const Event& event) override;
	void Run() override;

protected:
	void Update(float dTime) override;
	void Draw() override;


};