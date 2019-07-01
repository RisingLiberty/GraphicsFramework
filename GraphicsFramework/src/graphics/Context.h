#pragma once

class Renderer;

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

	static void Create(API api, void* windowHandle);

	static Context* GetCurrent();
	Renderer* GetRenderer() const;

	virtual void Present() = 0;
	virtual API GetApiType() const = 0;

protected:
	virtual void InitializeImGui() const = 0;

protected:
	std::unique_ptr<Renderer> m_renderer;

private:
	static std::unique_ptr<Context> s_current;

};