#include <SDL.h>
#include <SDL_events.h>
#include <stdio.h>
#include <string>
#include <cstring>
#include <iostream>
#include <unistd.h>
#include <unordered_map>
#include <functional>
#include "EventProcessor.h"

//Screen dimension constants
const int SCREEN_HEIGHT = 2000;
const int SCREEN_WIDTH = 2000;


//the window we're rendering
SDL_Window* g_window = nullptr;
//the surface contained by the window
SDL_Surface* g_surface = nullptr;
//the image to be loaded on the surface
SDL_Surface* g_image = nullptr; 

//reference to the event processor
EventProcessor evp;

std::string bin_path;

const std::string img_path = R"(..\resources\peeta_muda.bmp)"; 

bool load_image(std::string bin_path, std::string img_path)
{
	#ifdef _LINUX
	//replace forward slashes with back slashes
	#endif
	#ifdef _WIN32
	//default
	#endif
	std::string load_path = bin_path + img_path;
	SDL_Log("load_path: %s\n", load_path.c_str());
	SDL_Log("img_path: %s\n", img_path.c_str());
	return (g_image = SDL_LoadBMP(load_path.c_str())) != nullptr;
}

void close()
{
	SDL_FreeSurface(g_surface);
	g_surface = nullptr;
	//Destroy window
	SDL_DestroyWindow(g_window);
	g_window = nullptr;

	//Quit SDL subsystems
	SDL_Quit();
	exit(0);
}

bool init()
{
	if (SDL_Init(SDL_INIT_VIDEO) < 0)
	{
		SDL_Log("SDL_Init\n");
		close();
		return false;
	}	

	g_window = SDL_CreateWindow("SDL Tutorial", SDL_WINDOWPOS_UNDEFINED, SDL_WINDOWPOS_UNDEFINED, SCREEN_WIDTH, SCREEN_HEIGHT, SDL_WINDOW_SHOWN);

	if (!g_window)
	{
		SDL_Log("SDL_CreateWindow\n");
		return false;
	}
	g_surface = SDL_GetWindowSurface(g_window);
	if (!g_surface)
	{
		SDL_Log("SDL_GetWindowSurface\n");
		return false;
	} 
	evp = EventProcessor();
	bin_path = getcwd(nullptr, 0);
	return true;
}

int main(int argc, char* args[])
{

	//Initialize SDL
	if (!init())
	{
		SDL_Log("init()\n");
		close();
	}

	bin_path = SDL_GetBasePath();
	if (!load_image(bin_path, img_path))
	{
		SDL_Log("load_image()\n");
		close();
	}
	
	SDL_BlitSurface(g_image, nullptr, g_surface, nullptr);
	SDL_UpdateWindowSurface(g_window);
    
    //Hack to get window to stay up
    SDL_Event e; 
	bool quit = false; 
	while (quit == false)
	{
		while (SDL_PollEvent(&e))
		{ 
			evp.process_event(e);
		}
	}
    close();

	return 0;
}
