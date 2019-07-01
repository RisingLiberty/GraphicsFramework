#pragma once

class Context
{
public: 
	enum class API
	{
		DirectX11,
		DirectX12,
		OpenGL,
		Vulkan
	};

	Context();
	virtual ~Context();

	static Context* GetCurrent();
	static void Create(API api, void* windowHandle);

	virtual void Present() = 0;
	virtual API GetApiType() const = 0;

private:
	static std::unique_ptr<Context> s_current;
};