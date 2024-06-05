#include <SDL.h>
#include <SDL_events.h>
#include <GL\GLU.h>
#include <SDL_video.h>
#include <stdio.h>
#include <string>
#include <cstring>
#include <iostream>
#include <unistd.h>
#include <unordered_map>
#include <functional>
#include <csignal>
#include "SDL_stdinc.h"
#include "SDL_opengl.h"
#include "EventProcessor.h"
#include "GameLogic.h"

//the window we're rendering
SDL_Window* g_window = nullptr;
//the surface contained by the window
SDL_GLContext g_context;
//the image to be loaded on the surface
SDL_Surface* g_image = nullptr; 
//reference to the event processor
EventProcessor* evp;
//reference to game grid
Grid* grid;


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

//draw a rectangle to the screen
void render()
{
	glClear(GL_COLOR_BUFFER_BIT);
	glBegin(GL_QUADS);
	glVertex2f(-0.5f, -0.5f);
	glVertex2f(0.5f, -0.5f);
	glVertex2f(0.5f, 0.5f);
	glVertex2f(-0.5f, 0.5f);
	glEnd();
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
		render_grid(grid);
		SDL_GL_SwapWindow(g_window);
	}
}

static void sdl_close()
{
	SDL_Log("sdl_close()\n");
//	SDL_FreeGLContext(g_context);
	g_context = nullptr;
	//Destroy window
	SDL_DestroyWindow(g_window);
	g_window = nullptr;

	//Quit SDL subsystems
	SDL_Quit();
	delete evp;
	exit(0);
}

bool init_gl()
{
	glMatrixMode(GL_PROJECTION);
	glLoadIdentity();
	if (glGetError() != GL_NO_ERROR)
	{
		SDL_Log("glGetError()\n");
		return false;
	}

	glMatrixMode(GL_MODELVIEW);
	glLoadIdentity();
	if (glGetError() != GL_NO_ERROR)
	{
		SDL_Log("glGetError()\n");
		return false;
	}
	glClearColor(0.0f, 0.0f, 0.0f, 1.0f);
	if (glGetError() != GL_NO_ERROR)
	{
		SDL_Log("glGetError()\n");
		return false;
	}

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

	SDL_GL_SetAttribute(SDL_GL_CONTEXT_MAJOR_VERSION, 2);
	SDL_GL_SetAttribute(SDL_GL_CONTEXT_MINOR_VERSION, 1);

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
	//set vsync
	SDL_GL_SetSwapInterval(1);
	
	if (!init_gl())
	{
		SDL_Log("init_gl()\n");
		return false;
	}

	Config config = read_config();
	init_grid(&config, grid);

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
