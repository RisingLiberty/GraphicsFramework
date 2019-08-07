#pragma once

#include "core/application.h"

using AreFramesCapped = YesNoEnum;

class Win64Application : public Application
{
public:
	Win64Application(AreFramesCapped areFramesCapped);
	virtual ~Win64Application();

	void OnEvent(const Event& event) override;
	void Run() override;

protected:
	void Update(float dTime) override;
	void Draw() override;

private:
	AreFramesCapped m_are_frames_capped;
	bool m_switched_api_last_frame;
	API m_context_to_switch_to;
	bool m_is_exiting;
};