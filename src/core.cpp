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
#include <GL\glew.h>
#include <GL\glu.h>
#include <SDL.h>
#include <SDL_events.h>
#include <SDL_opengl.h>
#include <SDL_video.h>
#include <SDL_stdinc.h>
#include "EventProcessor.h"
#include "GameLogic.h"

//the window we're rendering
SDL_Window* g_window = nullptr;
//the surface contained by the window
SDL_GLContext g_context;
//the image to be loaded on the surface
SDL_Surface* g_image = nullptr; 
//GLSL shader program
GLuint g_program_id = 0;
//vertex shader position
//documentation says this should be GLuint, but the demo code shows it as GLint,
//and can't initialize it to -1 as GLuint, so...
GLint gVertexPos2DLocation = -1;
//vertex buffer object
GLuint gVBO = 0;
//index buffer object
GLuint gIBO = 0;
//reference to the event processor
EventProcessor* evp;
//reference to game grid
GameLogic::Grid* grid;


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
		glClear(GL_COLOR_BUFFER_BIT);
		//bind program
		glUseProgram(g_program_id);
		//enable vertex position 
		glEnableVertexAttribArray(gVertexPos2DLocation);
		//set vertex data 
		glBindBuffer(GL_ARRAY_BUFFER, gVBO);
		glVertexAttribPointer(gVertexPos2DLocation, 2, GL_FLOAT, GL_FALSE, 2*sizeof(GLfloat), nullptr);
		//set index data and render 
		glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, gIBO);
		glDrawElements(GL_TRIANGLE_FAN, 4, GL_UNSIGNED_INT, nullptr);
		glDisableVertexAttribArray(gVertexPos2DLocation);

//		grid->update_grid();
//		grid->render_grid();
		SDL_GL_SwapWindow(g_window);
		//unbind shader program
		glUseProgram(NULL);
//		SDL_Delay(1000);
	}
	sdl_close();
}

bool init_gl()
{
	g_program_id = glCreateProgram();
	GLuint vertex_shader = glCreateShader(GL_VERTEX_SHADER); 
	const GLchar* vertex_shader_source[] =
	{
		"#version 140\nin vec2 LVertexPos2D; void main() { gl_Position = vec4( LVertexPos2D.x, LVertexPos2D.y, 0, 1 ); }"
	};
	glShaderSource(vertex_shader, 1, vertex_shader_source, NULL);
	glCompileShader(vertex_shader);
	GLint vertex_shader_compiled = GL_FALSE;
	glGetShaderiv(vertex_shader, GL_COMPILE_STATUS, &vertex_shader_compiled);
	if (vertex_shader_compiled != GL_TRUE)
	{
		SDL_Log("vertex_shader_compiled\n");
		return false;
	}
	glAttachShader(g_program_id, vertex_shader);
	GLuint fragment_shader = glCreateShader(GL_FRAGMENT_SHADER);
	const GLchar* fragment_shader_source[] =
	{
		"#version 140\nout vec4 FragColor; void main() { FragColor = vec4( 1.0, 1.0, 1.0, 1.0 ); }"
	};
	glShaderSource(fragment_shader, 1, fragment_shader_source, NULL);
	glCompileShader(fragment_shader);
	GLint fragment_shader_compiled = GL_FALSE;
	glGetShaderiv(fragment_shader, GL_COMPILE_STATUS, &fragment_shader_compiled);
	if (fragment_shader_compiled != GL_TRUE)
	{
		SDL_Log("fragment_shader_compiled\n");
		return false;
	}
	glAttachShader(g_program_id, fragment_shader);
	//now both shaders are compiled and attached to the program, link it
	glLinkProgram(g_program_id);
	GLint program_linked = GL_FALSE;
	glGetProgramiv(g_program_id, GL_LINK_STATUS, &program_linked);
	if (program_linked != GL_TRUE)
	{
		SDL_Log("program_linked\n");
		return false;
	}
	//get vertex attribute location
	gVertexPos2DLocation = glGetAttribLocation(g_program_id, "LVertexPos2D");
	if (gVertexPos2DLocation == -1)
	{
		SDL_Log("gVertexPos2DLocation\n");
		return false;
	}
	//initialize clear color
	glClearColor(0.0f, 0.0f, 0.0f, 1.0f);

	//VBO data
	GLfloat vertex_data[] = 
	{
		-0.5f, -0.5f,
		0.5f, -0.5f,
		0.5f, 0.5f,
		-0.5f, 0.5f
	};

	//IBO data
	GLuint index_data[] = { 0, 1, 2, 3 };
	//create VBO
	glGenBuffers(1, &gVBO);
	glBindBuffer(GL_ARRAY_BUFFER, gVBO);
	glBufferData(GL_ARRAY_BUFFER, 2*4*sizeof(GLfloat), vertex_data, GL_STATIC_DRAW);

	//create IBO
	glGenBuffers(1, &gIBO);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, gIBO);
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, 4*sizeof(GLuint), index_data, GL_STATIC_DRAW);

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
	SDL_GL_SetAttribute(SDL_GL_CONTEXT_MINOR_VERSION, 1);
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

	GameLogic::Config* config = new GameLogic::Config();
	grid = new GameLogic::Grid(config);
	delete config;
	
	if (!init_gl())
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
