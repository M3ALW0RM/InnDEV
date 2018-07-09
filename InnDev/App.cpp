#include "App.hpp"
#include <stdio.h>
#include <SDL.h>
#include <glm.hpp>
#include <../glm/gtc/matrix_transform.hpp>
#include <../glm/gtx/transform.hpp>
#include <random>
#include <time.h>
#include <IL/devil_cpp_wrapper.hpp>

App::App()
{
}


App::~App()
{
}

bool App::Init()
{
	bool success = true;
	if (SDL_Init(SDL_INIT_VIDEO) < 0) {
		printf("SDL could not initialize! SDL_Error: %s\n", SDL_GetError());
		success = false;
	}
	else {
		m_window.Init();
	}

	// Initialize IL
	ilInit();
	// Initialize ILU
	iluInit();
	// Initialize ILUT with OpenGL support.
	ilutRenderer(ILUT_OPENGL);

	srand(time(0));
	m_renderables.emplace_back();
	m_renderables[m_renderables.size() - 1].LoadOBJ("Models/fish/fish.obj", "Models/fish");

	return success;
}

void App::Run()
{

	m_run = true;
	SDL_Event e;
	double lastTime = SDL_GetTicks();
	int nbFrames = 0;
	glm::mat4 Projection = glm::perspective(glm::radians(45.0f), (float)640 / (float)480, 0.1f, 10000.0f);
	glm::mat4 View = glm::lookAt(
		glm::vec3(0, 0, 20), // Camera is at (4,3,3), in World Space
		glm::vec3(0, 0, 0), // and looks at the origin
		glm::vec3(0, 1, 0)  // Head is up (set to 0,-1,0 to look upside-down)
	);
	m_window.vp = Projection * View;
	while (m_run) {
		double currentTime = SDL_GetTicks();
		nbFrames++;
		if (currentTime - lastTime >= 1000.0) {
			printf("%i fps\n", nbFrames);
			printf("%f ms/frame\n", 1000.0 / double(nbFrames));
			nbFrames = 0;
			lastTime += 1000.0;
		}
		while (SDL_PollEvent(&e) != 0) {
			if (e.type == SDL_QUIT) {
				m_run = false;
			}
		}
		m_window.Render(m_renderables);
	}
}

void App::Clean()
{
	m_window.Clean();

	//Quit SDL subsystems
	SDL_Quit();
}
