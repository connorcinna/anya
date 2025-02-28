#include <stdio.h>
#include <string>
#include <cstring>
#include <iostream>
#include <unistd.h>
#include <unordered_map>
#include <functional>
#include <csignal>
#include <chrono>
#include <thread>
#ifdef __linux__
#include <GL/glew.h>
#include <GL/glu.h>
#endif
#ifdef _WIN32
#include <GL\glew.h>
#include <GL\glu.h>
#endif
#include <SDL.h>
#include <SDL_events.h>
#include <SDL_opengl.h>
#include <SDL_video.h>
#include <SDL_stdinc.h>
#include "EventProcessor.h"
#include "GameLogic.h"
#include "Renderer.h"

//the window we're rendering
SDL_Window* g_window = nullptr;
//the surface contained by the window
SDL_GLContext g_context;
//the image to be loaded on the surface
SDL_Surface* g_image = nullptr; 
//GLSL shader program for dead cells
GLuint dead_program_id = 0;
//GLSL shader program for alive cells
GLuint alive_program_id = 0;
//vertex shader position
//documentation says this should be GLuint, but the demo code shows it as GLint,
//and can't initialize it to -1 as GLuint, so...
//reference to the event processor
EventProcessor* evp;
//reference to game grid
Grid* grid;
//reference to renderer
Renderer* renderer;
const char* vertex_shader_source = 
R"(
	#version 330 core
	layout (location = 0) in vec3 aPos;

	void main()
	{
		gl_Position = vec4(aPos.x, aPos.y, aPos.z, 1.0);
	}
)";
const char* dead_fragment_shader_source = 
R"(
	#version 330 core
	out vec4 FragColor;
	void main()
	{
		FragColor = vec4( 0.0, 0.0, 0.0, 1.0 );
	}
)";
const char* alive_fragment_shader_source = 
R"(
	#version 330 core
	out vec4 FragColor;
	void main()
	{
		FragColor = vec4( 1.0, 1.0, 1.0, 1.0 );
	}
)";

void usage() 
{
	SDL_Log("Usage: anya.exe -w [width] -h [height]\n");
	SDL_Log("Example: anya.exe -w 1920 -h 1080\n");
	exit(-1);
}

void signal_handler(int signum)
{
	switch(signum)
	{
		case SIGABRT:
			SDL_Log("SIGABRT received\n");
			abort();
			break;
	}
}

void sdl_close()
{
	SDL_Log("sdl_close()\n");
	g_context = nullptr;
	//Destroy window
	SDL_DestroyWindow(g_window);
	g_window = nullptr;

	//Quit SDL subsystems
	SDL_Quit();
	delete evp;
	exit(0);
}

void update()
{
	SDL_Event e;
	for(;;)
	{
		while (SDL_PollEvent(&e))
		{ 
			evp->process_event(e);
		}

		grid->update_grid();
		grid->render_grid(dead_program_id, alive_program_id);
		SDL_GL_SwapWindow(g_window);
		SDL_Delay(1000);
	}
	sdl_close();
}

bool init_gl(int w_width, int w_height, Grid* grid)
{
	glViewport(0, 0, w_width, w_height);
	dead_program_id = glCreateProgram();
	alive_program_id = glCreateProgram();

	GLuint vertex_shader = glCreateShader(GL_VERTEX_SHADER); 
	glShaderSource(vertex_shader, 1, &vertex_shader_source, NULL);
	glCompileShader(vertex_shader);
	GLint vertex_shader_compiled = GL_FALSE;
	glGetShaderiv(vertex_shader, GL_COMPILE_STATUS, &vertex_shader_compiled);
	if (vertex_shader_compiled != GL_TRUE)
	{
		SDL_Log("vertex_shader_compiled\n");
		return false;
	}
	glAttachShader(dead_program_id, vertex_shader);
	glAttachShader(alive_program_id, vertex_shader);

	GLuint dead_fragment_shader = glCreateShader(GL_FRAGMENT_SHADER);
	GLuint alive_fragment_shader = glCreateShader(GL_FRAGMENT_SHADER);

	//compile and link dead fragment shader
	glShaderSource(dead_fragment_shader, 1, &dead_fragment_shader_source, NULL);
	glCompileShader(dead_fragment_shader);
	GLint dead_fragment_shader_compiled = GL_FALSE;
	glGetShaderiv(dead_fragment_shader, GL_COMPILE_STATUS, &dead_fragment_shader_compiled);
	if (dead_fragment_shader_compiled != GL_TRUE)
	{
		SDL_Log("fragment_shader_compiled\n");
		return false;
	}
	glAttachShader(dead_program_id, dead_fragment_shader);

	//comple and link alive fragment shader
	glShaderSource(alive_fragment_shader, 1, &alive_fragment_shader_source, NULL);
	glCompileShader(alive_fragment_shader);
	GLint alive_fragment_shader_compiled = GL_FALSE;
	glGetShaderiv(alive_fragment_shader, GL_COMPILE_STATUS, &alive_fragment_shader_compiled);
	if (alive_fragment_shader_compiled != GL_TRUE)
	{
		SDL_Log("fragment_shader_compiled\n");
		return false;
	}
	//now both shaders are compiled and attached to the program, link it
	glLinkProgram(dead_program_id);
	glLinkProgram(alive_program_id);
	GLint dead_program_linked = GL_FALSE;
	GLint alive_program_linked = GL_FALSE;
	glGetProgramiv(dead_program_id, GL_LINK_STATUS, &dead_program_linked);
	if (dead_program_linked != GL_TRUE)
	{
		SDL_Log("dead_program_linked\n");
		return false;
	}
	glGetProgramiv(alive_program_id, GL_LINK_STATUS, &alive_program_linked);
	if (alive_program_linked != GL_TRUE)
	{
		SDL_Log("alive_program_linked\n");
		return false;
	}
	//initialize vertex array objects for each cell
	std::vector<std::vector<GLuint>> VBOs(grid->width, std::vector<GLuint>(grid->height));
	std::vector<std::vector<GLuint>> VAOs(grid->width, std::vector<GLuint>(grid->height));
	for (size_t i = 0; i < VAOs.size(); ++i)
	{
		for (size_t j = 0; j < VAOs[i].size(); ++j)
		{
			//create VAO
			glGenVertexArrays(1, &VAOs[i][j]);
			CHECK_GL_ERR();
			//create VBO
			glGenBuffers(1, &VBOs[i][j]);
			CHECK_GL_ERR();
			//bind the VAO to the GL_ARRAY_BUFFER
			glBindVertexArray(VAOs[i][j]);
			CHECK_GL_ERR();
			//bind the VBO to the VAO
			glBindBuffer(GL_ARRAY_BUFFER, VBOs[i][j]);
			CHECK_GL_ERR();
		}
	}
	//set the grid object's VBOs and VAOs
	grid->VBOs = VBOs;
	grid->VAOs = VAOs;
	grid->init_cells();
	glDeleteShader(dead_fragment_shader);
	glDeleteShader(alive_fragment_shader);
	glDeleteShader(vertex_shader);

	return true;
}

bool init(int w_width, int w_height)
{
	evp = new EventProcessor();
	if (SDL_Init(SDL_INIT_VIDEO) < 0)
	{
		SDL_Log("SDL_Init\n");
		sdl_close();
		return false;
	}	

	SDL_GL_SetAttribute(SDL_GL_CONTEXT_MAJOR_VERSION, 3);
	SDL_GL_SetAttribute(SDL_GL_CONTEXT_MINOR_VERSION, 3);
	SDL_GL_SetAttribute(SDL_GL_CONTEXT_PROFILE_MASK, SDL_GL_CONTEXT_PROFILE_CORE);

	g_window = SDL_CreateWindow("anya", SDL_WINDOWPOS_UNDEFINED, SDL_WINDOWPOS_UNDEFINED, w_width, w_height, SDL_WINDOW_SHOWN | SDL_WINDOW_OPENGL);

	if (!g_window)
	{
		SDL_Log("SDL_CreateWindow\n");
		return false;
	}
	g_context = SDL_GL_CreateContext(g_window);
	if (!g_context)
	{
		SDL_Log("SDL_GL_CreateContext()\n");
		return false;
	} 
	glewExperimental = GL_TRUE;
	GLenum glewError = glewInit();
	if (glewError != GLEW_OK)
	{
		SDL_Log("glewInit()\n");
		return false;
	}
	//set vsync
	SDL_GL_SetSwapInterval(1);
	renderer = new Renderer();
	Config* config = new Config();
	grid = new Grid(config);
	grid->renderer = renderer;
	delete config;
	
	if (!init_gl(w_width, w_height, grid))
	{
		SDL_Log("init_gl()\n");
		return false;
	}

	return true;
}


int main(int argc, char* argv[])
{
	int w_width = 0, w_height = 0;
	int opt;
	while((opt = getopt(argc, argv, "w:h:")) != -1)
	{
		switch (opt)
		{
			case 'w':
				w_width = atoi(optarg);
				break;
			case 'h':
				w_height = atoi(optarg);
				break;
			default:
				//unrecognized argument
				usage();
				break;
		}
	}
	SDL_Log("w_width: %d - w_height: %d\n", w_width, w_height);
	if (!init(w_width, w_height))
	{
		SDL_Log("init()\n");
		sdl_close();
	}
	signal(SIGABRT, signal_handler);

	SDL_UpdateWindowSurface(g_window);
    
	//main loop
	update();
	
    sdl_close();

	return 0;
}
