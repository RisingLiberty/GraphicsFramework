#include "pch.h"

#include "Sandbox.h"
#include "scenegraph/MainScene.h"

Sandbox::Sandbox():
	Win64Application(AreFramesCapped::NO)
{
}

Sandbox::~Sandbox()
{

}

void Sandbox::Initialize()
{
	this->PushScene(std::make_unique<MainScene>("Main Scene"));
}