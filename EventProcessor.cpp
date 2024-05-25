#include "EventProcessor.h"

EventProcessor::EventProcessor() : event_map 
		{
			{SDL_QUIT, process_quit }},
			{SDL_MOUSEMOTION, process_mouse_motion()},
			{SDL_DISPLAYEVENT, process_display()}
		} { } 
//generic event processor - the only instance where event_map is accessed
//child classes should simply push their own methods into the map and allow this
//method to execute them
void EventProcessor::process_event(SDL_Event e)
{
	event_map.at(e.type)();
}
void EventProcessor::process_quit()
{

}
void EventProcessor::process_mouse_motion()
{

}
void EventProcessor::process_display()
{

}
void EventProcessor::process_window()
{

}
void EventProcessor::process_sys_wm()
{

}
//		void process_key_down()
//		{
//
//		}
//		void process_key_up()
//		{
//
//		}
//		void process_text_editing()
//		{
//
//		}
//		void process_text_input()
//		{
//
//		}
//		void process_key_map_changed()
//		{
//
//		}
