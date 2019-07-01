#include "pch.h"

#include "Sandbox.h"
#include "scenegraph/MainScene.h"

Sandbox::Sandbox():
	Win64Application(AreFramesCapped::No)
{
	this->PushScene(std::make_unique<MainScene>("Main Scene"));
}

Sandbox::~Sandbox()
{

}