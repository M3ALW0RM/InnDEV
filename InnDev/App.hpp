#pragma once
#include "Window.h"
#include "Renderable.h"

class App
{
	std::vector<Renderable> m_renderables;
	Window m_window;
	bool m_run;
public:
	App();
	App& operator=(const App&) = delete;
	App& operator=(App&&) = delete;
	App(const App&) = delete;
	App(App&&) = delete;
	~App();
	bool Init();
	void Run();
	void Clean();
};

