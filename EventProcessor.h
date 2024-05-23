#ifndef EVENTPROCESSOR_H
#define EVENTPROCESSOR_H

#include <unordered_map>
#include <functional>
#include <SDL_events.h>

class EventProcessor
{
	public:
		EventProcessor();
		void process_event(SDL_Event e);
		void process_quit();
		void process_mouse_motion();
		void process_display();
		void process_window();
		void process_sys_wm();
	protected:
		std::unordered_map<int, std::function<void()>> event_map;
};

#endif
