#ifndef EVENTPROCESSOR_H
#define EVENTPROCESSOR_H

#include <unordered_map>
#include <functional>
#include <SDL_events.h>

class EventProcessor
{
	public:
		EventProcessor();
		virtual ~EventProcessor();
		// generic SDL events
		void process_event(SDL_Event e);
		void process_quit();
		void process_display();
		void process_window();
		void process_sys_wm();
		// keyboard events
		void process_keydown();
		void process_keyup();
		void process_textediting();
		void process_textinput();
		void process_keymap_changed();
		// mouse events
		void process_mouse_motion();
		void process_mouse_button_down();
		void process_mouse_button_up();
		void process_mouse_wheel();
		// joystick events
		void process_joy_axis_motion();
		void process_joy_ball_motion();
		void process_joy_hat_motion();
		void process_joy_button_down();
		void process_joy_button_up();
		void process_joy_device_added();
		void process_joy_device_removed();
		void process_joy_battery_updated();
		// game controller events
		void process_controller_axis_motion();
		void process_controller_button_down();
		void process_controller_button_up();
		void process_controller_device_added();
		void process_controller_device_removed();
		void process_controller_device_remapped();
		void process_controller_touchpad_down();
		void process_controller_touchpad_motion();
		void process_controller_touchpad_up();
		void process_controller_sensor_update();
		void process_controller_steam_handle_updated();
		// touch events
		void process_finger_down();
		void process_finger_up();
		void process_finger_motion();
		// gesture events
		void process_dollar_gesture();
		void process_dollar_record();
		void process_multigesture();
		// clipboard events
		void process_clipboard_update();
		// drag and drop events
		void process_drop_file();
		void process_drop_text();
		void process_drop_begin();
		void process_drop_complete();
		// audio hotplug events
		void process_audio_device_added();
		void process_audio_device_removed();
		// sensor update events
		void process_sensor_update();
		// render events 
		void process_render_targets_reset();
		void process_render_device_reset();
		// custom SDL events - likely to be unused
		void process_user_event();
		
	protected:
		std::unordered_map<int, std::function<void()>> event_map;
};

#endif
