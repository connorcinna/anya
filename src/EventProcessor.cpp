#include <type_traits>
#include "EventProcessor.h"
#include <SDL.h>
//some of the event types are not defined for the SDL2 version I have

EventProcessor::EventProcessor() : event_map 
{
	// generic SDL events
	{SDL_QUIT, [this]() { process_quit(); }},
#ifdef _WIN32
	{SDL_DISPLAYEVENT, [this]() { process_display(); }},
#endif
	{SDL_WINDOWEVENT, [this]() { process_window(); }},
	{SDL_SYSWMEVENT, [this]() {process_sys_wm(); }},
	// keyboard events
	{SDL_KEYDOWN, [this]() { process_keydown(); }},
	{SDL_KEYUP, [this]() { process_keyup(); }},
	{SDL_TEXTEDITING, [this]() { process_textediting(); }},
	{SDL_TEXTINPUT, [this]() { process_textinput(); }},
	{SDL_KEYMAPCHANGED, [this]() { process_keymap_changed(); }},
	// mouse events
	{SDL_MOUSEMOTION, [this]() { process_mouse_motion(); }},
	{SDL_MOUSEBUTTONDOWN, [this]() { process_mouse_button_down(); }},
	{SDL_MOUSEBUTTONUP, [this]() { process_mouse_button_up(); }},
	{SDL_MOUSEWHEEL, [this]() { process_mouse_wheel(); }},
	// joystick events
	{SDL_JOYAXISMOTION, [this]() { process_joy_axis_motion(); }},
	{SDL_JOYBALLMOTION, [this]() { process_joy_ball_motion(); }},
	{SDL_JOYHATMOTION, [this]() { process_joy_hat_motion(); }},
	{SDL_JOYBUTTONDOWN, [this]() { process_joy_button_down(); }},
	{SDL_JOYBUTTONUP, [this]() { process_joy_button_up(); }},
	{SDL_JOYDEVICEADDED, [this]() { process_joy_device_added(); }},
	{SDL_JOYDEVICEREMOVED, [this]() { process_joy_device_removed(); }},
#ifdef _WIN32
	{SDL_JOYBATTERYUPDATED, [this]() { process_joy_battery_updated(); }},
#endif
	// game controller events
	{SDL_CONTROLLERAXISMOTION, [this]() { process_controller_axis_motion(); }},
	{SDL_CONTROLLERBUTTONDOWN, [this]() { process_controller_button_down(); }},
	{SDL_CONTROLLERBUTTONUP, [this]() { process_controller_button_up(); }},
	{SDL_CONTROLLERDEVICEADDED, [this]() { process_controller_device_added(); }},
#ifdef _WIN32
	{SDL_CONTROLLERTOUCHPADDOWN, [this]() { process_controller_touchpad_down(); }},
	{SDL_CONTROLLERTOUCHPADMOTION, [this]() { process_controller_touchpad_motion(); }},
	{SDL_CONTROLLERTOUCHPADUP, [this]() { process_controller_touchpad_up(); }},
	{SDL_CONTROLLERSENSORUPDATE, [this]() { process_controller_sensor_update(); }},
	{SDL_CONTROLLERSTEAMHANDLEUPDATED, [this]() { process_controller_steam_handle_updated(); }},
#endif
	// touch events
	{SDL_FINGERDOWN, [this]() { process_finger_down(); }},
	{SDL_FINGERUP, [this]() { process_finger_up(); }},
	{SDL_FINGERMOTION, [this]() { process_finger_motion(); }},
	// gesture events
	{SDL_DOLLARGESTURE, [this]() { process_dollar_gesture(); }},
	{SDL_DOLLARRECORD, [this]() { process_dollar_record(); }},
	{SDL_MULTIGESTURE, [this]() { process_multigesture(); }},
	// clipboard events
	{SDL_CLIPBOARDUPDATE, [this]() { process_clipboard_update(); }},
	// drag and drop events
	{SDL_DROPFILE, [this]() { process_drop_file(); }},
	{SDL_DROPTEXT, [this]() { process_drop_text(); }},
	{SDL_DROPBEGIN, [this]() { process_drop_begin(); }},
	{SDL_DROPCOMPLETE, [this]() { process_drop_complete(); }},
	// audio hotplug events
	{SDL_AUDIODEVICEADDED, [this]() { process_audio_device_added(); }},
	{SDL_AUDIODEVICEREMOVED, [this]() { process_audio_device_removed(); }},
	// sensor events
#ifdef _WIN32
	{SDL_SENSORUPDATE, [this]() { process_sensor_update(); }},
#endif
	// render events
	{SDL_RENDER_TARGETS_RESET, [this]() { process_render_targets_reset(); }},
	{SDL_RENDER_DEVICE_RESET, [this]() { process_render_device_reset(); }},
	// user events
	{SDL_USEREVENT, [this]() { process_user_event(); }},
} { } 

EventProcessor::~EventProcessor()
{
	//unused
}
//generic event processor - the only instance where event_map is accessed
//child classes should simply push their own methods into the map and allow this
//method to execute them
void EventProcessor::process_event(SDL_Event e)
{
//	SDL_Log("Processing event: %x\n", e.type);
	event_map.at(e.type)();
}
void EventProcessor::process_quit()
{
	//let the OS free the stuff out of context, because we are quitting out anwyays
	SDL_Quit();
	exit(0);
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
void EventProcessor::process_keydown()
{

}
void EventProcessor::process_keyup()
{

}
void EventProcessor::process_textediting()
{

}
void EventProcessor::process_textinput()
{

}
void EventProcessor::process_keymap_changed()
{

}
void EventProcessor::process_mouse_motion()
{


}
void EventProcessor::process_mouse_button_down()
{

}
void EventProcessor::process_mouse_button_up()
{

}
void EventProcessor::process_mouse_wheel()
{
}
void EventProcessor::process_joy_axis_motion()
{
}
void EventProcessor::process_joy_ball_motion()
{
}
void EventProcessor::process_joy_hat_motion()
{
}
void EventProcessor::process_joy_button_down()
{
}
void EventProcessor::process_joy_button_up()
{
}
void EventProcessor::process_joy_device_added()
{
}
void EventProcessor::process_joy_device_removed()
{
}
void EventProcessor::process_joy_battery_updated()
{
}
void EventProcessor::process_controller_axis_motion()
{
}
void EventProcessor::process_controller_button_down()
{
}
void EventProcessor::process_controller_button_up()
{
}
void EventProcessor::process_controller_device_added()
{
}
void EventProcessor::process_controller_device_removed()
{
}
void EventProcessor::process_controller_device_remapped()
{
}
void EventProcessor::process_controller_touchpad_down()
{
}
void EventProcessor::process_controller_touchpad_motion()
{
}
void EventProcessor::process_controller_touchpad_up()
{
}
void EventProcessor::process_controller_sensor_update()
{
}
void EventProcessor::process_controller_steam_handle_updated()
{
}

void EventProcessor::process_finger_down()
{
}
void EventProcessor::process_finger_up()
{
}
void EventProcessor::process_finger_motion()
{
}

void EventProcessor::process_dollar_gesture()
{
}
void EventProcessor::process_dollar_record()
{
}
void EventProcessor::process_multigesture()
{
}
void EventProcessor::process_clipboard_update()
{
}
void EventProcessor::process_drop_file()
{
}
void EventProcessor::process_drop_text()
{
}
void EventProcessor::process_drop_begin()
{
}
void EventProcessor::process_drop_complete()
{
}
void EventProcessor::process_audio_device_added()
{
}
void EventProcessor::process_audio_device_removed()
{
}
void EventProcessor::process_sensor_update()
{
}
void EventProcessor::process_render_targets_reset()
{
}
void EventProcessor::process_render_device_reset()
{
}
void EventProcessor::process_user_event()
{
}
