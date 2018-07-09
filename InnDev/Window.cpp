#include "Window.h"
#include "../ThirdParty/TinyObj/tiny_obj_loader.h"
#include <SDL.h>
#include <GL/glew.h>
#include <stdio.h>

#include <../glm/gtc/matrix_transform.hpp>
#include <../glm/gtx/transform.hpp>
#include <vector>
#include <fstream>
#include <sstream>

#define width 640
#define height 480

GLuint Window::loadShaderProgram(const char * vertex_file_path, const char * fragment_file_path)
{
	// Create the shaders
	GLuint VertexShaderID = glCreateShader(GL_VERTEX_SHADER);
	GLuint FragmentShaderID = glCreateShader(GL_FRAGMENT_SHADER);

	// Read the Vertex Shader code from the file
	std::string VertexShaderCode;
	std::ifstream VertexShaderStream(vertex_file_path, std::ios::in);
	if (VertexShaderStream.is_open()) {
		std::stringstream sstr;
		sstr << VertexShaderStream.rdbuf();
		VertexShaderCode = sstr.str();
		VertexShaderStream.close();
	}
	else {
		printf("Impossible to open %s. Are you in the right directory ? Don't forget to read the FAQ !\n", vertex_file_path);
		getchar();
		return 0;
	}

	// Read the Fragment Shader code from the file
	std::string FragmentShaderCode;
	std::ifstream FragmentShaderStream(fragment_file_path, std::ios::in);
	if (FragmentShaderStream.is_open()) {
		std::stringstream sstr;
		sstr << FragmentShaderStream.rdbuf();
		FragmentShaderCode = sstr.str();
		FragmentShaderStream.close();
	}

	GLint Result = GL_FALSE;
	int InfoLogLength;

	// Compile Vertex Shader
	printf("Compiling shader : %s\n", vertex_file_path);
	char const * VertexSourcePointer = VertexShaderCode.c_str();
	glShaderSource(VertexShaderID, 1, &VertexSourcePointer, NULL);
	glCompileShader(VertexShaderID);

	// Check Vertex Shader
	glGetShaderiv(VertexShaderID, GL_COMPILE_STATUS, &Result);
	glGetShaderiv(VertexShaderID, GL_INFO_LOG_LENGTH, &InfoLogLength);
	if (InfoLogLength > 0) {
		std::vector<char> VertexShaderErrorMessage(InfoLogLength + 1);
		glGetShaderInfoLog(VertexShaderID, InfoLogLength, NULL, &VertexShaderErrorMessage[0]);
		printf("%s\n", &VertexShaderErrorMessage[0]);
	}

	// Compile Fragment Shader
	printf("Compiling shader : %s\n", fragment_file_path);
	char const * FragmentSourcePointer = FragmentShaderCode.c_str();
	glShaderSource(FragmentShaderID, 1, &FragmentSourcePointer, NULL);
	glCompileShader(FragmentShaderID);

	// Check Fragment Shader
	glGetShaderiv(FragmentShaderID, GL_COMPILE_STATUS, &Result);
	glGetShaderiv(FragmentShaderID, GL_INFO_LOG_LENGTH, &InfoLogLength);
	if (InfoLogLength > 0) {
		std::vector<char> FragmentShaderErrorMessage(InfoLogLength + 1);
		glGetShaderInfoLog(FragmentShaderID, InfoLogLength, NULL, &FragmentShaderErrorMessage[0]);
		printf("%s\n", &FragmentShaderErrorMessage[0]);
	}

	// Link the program
	printf("Linking program\n");
	GLuint ProgramID = glCreateProgram();
	glAttachShader(ProgramID, VertexShaderID);
	glAttachShader(ProgramID, FragmentShaderID);
	glLinkProgram(ProgramID);

	// Check the program
	glGetProgramiv(ProgramID, GL_LINK_STATUS, &Result);
	glGetProgramiv(ProgramID, GL_INFO_LOG_LENGTH, &InfoLogLength);
	if (InfoLogLength > 0) {
		std::vector<char> ProgramErrorMessage(InfoLogLength + 1);
		glGetProgramInfoLog(ProgramID, InfoLogLength, NULL, &ProgramErrorMessage[0]);
		printf("%s\n", &ProgramErrorMessage[0]);
	}

	glDetachShader(ProgramID, VertexShaderID);
	glDetachShader(ProgramID, FragmentShaderID);

	glDeleteShader(VertexShaderID);
	glDeleteShader(FragmentShaderID);

	return ProgramID;
}

Window::Window()
{
}


Window::~Window()
{
}

bool Window::Init()
{
	bool success = true;
	m_window = SDL_CreateWindow(
		"Fish First", 0, 0, width, height,
		SDL_WINDOW_OPENGL | SDL_WINDOW_RESIZABLE);
	if (m_window == NULL)
	{
		printf("Window could not be created! SDL_Error: %s\n", SDL_GetError());
		success = false;
	}
	SDL_SetWindowBordered(m_window, SDL_TRUE);
	SDL_SetWindowPosition(m_window, SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED);
	m_context = SDL_GL_CreateContext(m_window);
	
	if (m_context == NULL)
	{
		printf("OpenGL context could not be created! SDL_Error: %s\n", SDL_GetError());
		success = false;
	}

	glewExperimental = true; // Needed in core profile
	if (glewInit() != GLEW_OK) {
		printf("Failed to initialize GLEW\n");
		success = false;
	}
	//Enable backface culling
	glEnable(GL_CULL_FACE);
	glCullFace(GL_BACK);
	//Dissable Vsync
	SDL_GL_SetSwapInterval(0);
	//Enable z-buffer
	glEnable(GL_DEPTH_TEST);
	glDepthFunc(GL_LESS);
	m_defaultProgramID = loadShaderProgram("def.vert", "def.frag");

	return success;
}

void Window::Render(std::vector<Renderable>& renderables)
{
	temp += 0.0001;
	glClearColor(0, 0, 0, 1);
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	//Tell opengl to use my program
	glUseProgram(m_defaultProgramID);
	// Index buffer

	//Create MVP

	glm::mat4 Model = glm::rotate(temp, glm::vec3(0,1,0)) * glm::scale(glm::vec3(2.f, 2.f, 2.f));

	GLuint ModelMatrixID = glGetUniformLocation(m_defaultProgramID, "model");
	GLuint ViewPerspectiveMatrixID = glGetUniformLocation(m_defaultProgramID, "viewPerspective");
	GLuint LightDirID = glGetUniformLocation(m_defaultProgramID, "lightDir");
	GLuint TextureID = glGetUniformLocation(m_defaultProgramID, "myTextureSampler");



	for (int i = 0; i < renderables.size(); ++i) 
	{
		for (int j = 0; j < renderables[i].shapeCount; ++j) 
		{

			// Bind our texture in Texture Unit 0
			if (renderables[i].diffuseTexID.size() > 0)
			{
				glActiveTexture(GL_TEXTURE0);
				glBindTexture(GL_TEXTURE_2D, renderables[i].diffuseTexID[renderables[i].usedMat[j]]);
				// Set our "myTextureSampler" sampler to use Texture Unit 0
				glUniform1i(TextureID, 0);
			}
			glEnableVertexAttribArray(0);
			glBindBuffer(GL_ARRAY_BUFFER, renderables[i].interleavedBufferIDs[j]);
			glVertexAttribPointer(
				0,                  // attribute 0. No particular reason for 0, but must match the layout in the shader.
				3,                  // size
				GL_FLOAT,           // type
				GL_FALSE,           // normalized?
				11 * sizeof(float),    // stride
				(void*)0            // array buffer offset
			);
			glEnableVertexAttribArray(1);
			glVertexAttribPointer(
				1,                  // attribute 1. No particular reason for 1, but must match the layout in the shader.
				3,                  // size
				GL_FLOAT,           // type
				GL_FALSE,           // normalized?
				11 * sizeof(float),    // stride
				(void*)(3 * sizeof(float)) // array buffer offset
			);
			glEnableVertexAttribArray(2);
			glVertexAttribPointer(
				2,                  // attribute 2. No particular reason for 2, but must match the layout in the shader.
				3,                  // size
				GL_FLOAT,           // type
				GL_FALSE,           // normalized?
				11 * sizeof(float),    // stride
				(void*)(6 * sizeof(float)) // array buffer offset
			);
			glEnableVertexAttribArray(3);
			glVertexAttribPointer(
				3,                                // attribute 3. No particular reason for 3, but must match the layout in the shader.
				2,                                // size : U+V => 2
				GL_FLOAT,                         // type
				GL_TRUE,                         // normalized?
				11 * sizeof(float),               // stride
				(void*)(9 * sizeof(float))        // array buffer offset
			);


			glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, renderables[i].indexBufferIDs[j]);
			// Draw the triangles !
			glUniformMatrix4fv(ModelMatrixID, 1, GL_FALSE, &Model[0][0]);
			glUniformMatrix4fv(ViewPerspectiveMatrixID, 1, GL_FALSE, &vp[0][0]);
			glUniform3f(LightDirID, 0, 0, 1);


			glDrawElements(
				GL_TRIANGLES,      // mode
				renderables[i].indexCount[j],    // count
				GL_UNSIGNED_SHORT,   // type
				(void*)0           // element array buffer offset
			);

		}
		
	}
	SDL_GL_SwapWindow(m_window);
}

void Window::Clean()
{
	//Destroy OpenGL context
	SDL_GL_DeleteContext(m_context);

	//Destroy window
	SDL_DestroyWindow(m_window);
	m_window = NULL;
}
