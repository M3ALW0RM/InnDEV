#pragma once
#include <vector>
#include <glm.hpp>
#include "Renderable.h"
struct SDL_Window;
struct SDL_Surface;
typedef void* SDL_GLContext;
typedef unsigned int GLuint;
class Window
{
	SDL_Window* m_window = nullptr;
	SDL_Surface* m_suface = nullptr;
	SDL_GLContext m_context = nullptr;
	// This will identify our vertex buffer
	GLuint m_vertexbuffer;
	GLuint m_defaultProgramID;
	GLuint loadShaderProgram(const char * vertex_file_path, const char * fragment_file_path);
	float temp = 0;
public:
	glm::mat4 vp;
	Window();
	~Window();
	bool Init();
	void Render(std::vector<Renderable>& renderables);
	void Clean();
};

