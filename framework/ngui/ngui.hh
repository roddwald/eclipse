#ifndef NGUI_HH
#define NGUI_HH

#include <type_traits>
#include <vector>
#include <deque>
#include <array>
#include <string>
#include <algorithm>
#include <windows.h>

#define NGUI_API __declspec( dllexport )

//  __   __     ______     __  __     __
// /\ "-.\ \   /\  ___\   /\ \/\ \   /\ \
// \ \ \-.  \  \ \ \__ \  \ \ \_\ \  \ \ \
//  \ \_\\"\_\  \ \_____\  \ \_____\  \ \_\
//   \/_/ \/_/   \/_____/   \/_____/   \/_/
//
//			developed by hypnotic
//			credits: jeremymetzler (kinda) aka stickrpg.
//			zxvnme for inspiration.
//
// this immediate mode framework is very easy to work with...
// it is still a work in progress and there are very simple steps to start using this
// examples will be provided below.
//
//
// For DirectX users:
// You can implement this very easily by making a wrapper in-fact this was made in a directx test environment and the same thing goes with surface users except you have
// to replace it with your functions
// Example:
//
//namespace wrapper {
//	void rectangle( int x, int y, int w, int h, ngui::color col ) noexcept {
//		render::rectangle( x, y, w, h, rgba( col.r, col.g, col.b, col.a ) );
//	}
//
//	void gradient( int x, int y, int w, int h, ngui::color col, ngui::color col2, bool horizontal ) noexcept {
//		render::gradient( x, y, w, h, rgba( col.r, col.g, col.b, col.a ), rgba( col2.r, col2.g, col2.b, col2.a ), horizontal );
//	}
//
//	void outline( int x, int y, int w, int h, ngui::color col ) noexcept {
//		render::outline( x, y, w, h, rgba( col.r, col.g, col.b, col.a ) );
//	}
//
//	void get_text_size( const char* text, unsigned long font, int& w, int& h ) noexcept {
//		std::string str = text;
//		std::wstring wstr = std::wstring( str.begin( ), str.end( ) );
//
//		render::dim dim;
//		render::text_size( ( void* ) font, wstr, dim );
//		w = dim.w;
//		h = dim.h;
//	}
//
//	void text( int x, int y, unsigned long font, ngui::color col, bool center, const char* text ) noexcept {
//		std::string str = text;
//		std::wstring wstr = std::wstring( str.begin( ), str.end( ) );
//		int w = 0;
//
//		if ( center ) {
//			render::dim dim;
//			render::text_size( ( void* ) font, wstr, dim );
//
//			w = dim.w / 2;
//		}
//
//		if ( center ) {
//			render::text( x - w + 1, y + 1, rgba( 0, 0, 0, col.a ), ( void* ) font, wstr );
//			render::text( x - w, y, rgba( col.r, col.g, col.b, col.a ), ( void* ) font, wstr );
//		}
//		else {
//			render::text( x + 1, y + 1, rgba( 0, 0, 0, col.a ), ( void* ) font, wstr );
//			render::text( x, y, rgba( col.r, col.g, col.b, col.a ), ( void* ) font, wstr );
//		}
//	}
//};
//
// And you can also do this --> (make sure the functions use noexcept)
// ngui::draw.filled_rect = wrapper::rectangle;
// ngui::draw.rect = wrapper::outline;
// ngui::draw.get_text_size = wrapper::get_text_size;
// ngui::draw.gradient = wrapper::gradient;
// ngui::draw.text = wrapper::text;
//
//

namespace ngui {
	// == STRUCTS
	struct vec2 {
		vec2( ) = default;
		vec2( float x, float y ) {
			this->x = x;
			this->y = y;
		}

		float x, y;
	};

	struct rect {
		rect( ) = default;
		rect( int x, int y, int w, int h ) {
			this->x = x;
			this->y = y;
			this->w = w;
			this->h = h;
		}

		int x, y, w, h;
	};

	struct color {
		color( ) = default;
		color( int r, int g, int b, int a ) {
			this->r = r;
			this->g = g;
			this->b = b;
			this->a = a;
		}

		color( int r, int g, int b ) {
			this->r = r;
			this->g = g;
			this->b = b;
			this->a = 255;
		}

		int r, g, b, a;
	};

	// == UTILITIES
	namespace utils {
		namespace hash {
			constexpr uint32_t hash( const char* str, const uint32_t val = 0x811C9DC5 ) noexcept {
				return ( str[0] == '\0' ) ? val : hash( &str[1], ( val ^ uint32_t( str[0] ) ) * 0x1000193 );
			}
		}

		namespace input {
			static std::array<int, 256> old_keystate;
			static std::array<int, 256> keystate;

			NGUI_API bool key_pressed( int i );

			NGUI_API bool key_down( int i );

			NGUI_API bool key_released( int i );

			NGUI_API bool in_region( int x, int y, int w, int h );
		}
	}

	static const char* key_str[254] = { ( "-" ), 				( "M1" ), 				( "M2" ), 				( "BRK" ), 				( "M3" ), 				( "M4" ), 				( "M5" ), 				( "-" ), 				( "BCK" ), 				( "TAB" ), 				( "-" ), 				( "-" ), 				( "-" ), 				( "ENT" ), 				( "-" ), 				( "-" ), 				( "SHI" ), 				( "CTRL" ), 				( "ALT" ), 				( "PSE" ), 				( "CAPS" ), 				( "-" ), 				( "-" ), 				( "-" ), 				( "-" ), 				( "-" ), 				( "-" ), 				( "ESC" ), 				( "-" ), 				( "-" ), 				( "-" ), 				( "-" ), 				( "SPCE" ), 				( "PGUP" ), 				( "PGDN" ), 				( "END" ), 				( "HME" ), 				( "LFT" ), 				( "UP" ), 				( "RGHT" ), 				( "DWN" ), 				( "-" ), 				( "PRNT" ), 				( "-" ), 				( "PSCR" ), 				( "INS" ), 				( "DEL" ), 				( "-" ), 				( "0" ), 				( "1" ), 				( "2" ), 				( "3" ), 				( "4" ), 				( "5" ), 				( "6" ), 				( "7" ), 				( "8" ), 				( "9" ), 				( "-" ), 				( "-" ), 				( "-" ), 				( "-" ), 				( "-" ), 				( "-" ), 				( "-" ), 				( "A" ), 				( "B" ), 				( "C" ), 				( "D" ), 				( "E" ), 				( "F" ), 				( "G" ), 				( "H" ), 				( "I" ), 				( "J" ), 				( "K" ), 				( "L" ), 				( "M" ), 				( "N" ), 				( "O" ), 				( "P" ), 				( "Q" ), 				( "R" ), 				( "S" ), 				( "T" ), 				( "U" ), 				( "V" ), 				( "W" ), 				( "X" ), 				( "Y" ), 				( "Z" ), 				( "LWIN" ), 				( "RWIN" ), 				( "-" ), 				( "-" ), 				( "-" ), 				( "NUM0" ), 				( "NUM1" ), 				( "NUM2" ), 				( "NUM3" ), 				( "NUM4" ), 				( "NUM5" ), 				( "NUM6" ), 				( "NUM7" ), 				( "NUM8" ), 				( "NUM9" ), 				( "*" ), 				( "+" ), 				( "" ), 				( "-" ), 				( "." ), 				( "/" ), 				( "F1" ), 				( "F2" ), 				( "F3" ), 				( "F4" ), 				( "F5" ), 				( "F6" ), 				( "F7" ), 				( "F8" ), 				( "F9" ), 				( "F10" ), 				( "F11" ), 				( "F12" ), 				( "F13" ), 				( "F14" ), 				( "F15" ), 				( "F16" ), 				( "F17" ), 				( "F18" ), 				( "F19" ), 				( "F20" ), 				( "F21" ), 				( "F22" ), 				( "F23" ), 				( "F24" ), 				( "-" ), 				( "-" ), 				( "-" ), 				( "-" ), 				( "-" ), 				( "-" ), 				( "-" ), 				( "-" ), 				( "NUML" ), 				( "SCRL" ), 				( "-" ), 				( "-" ), 				( "-" ), 				( "-" ), 				( "-" ), 				( "-" ), 				( "-" ), 				( "-" ), 				( "-" ), 				( "-" ), 				( "-" ), 				( "-" ), 				( "-" ), 				( "-" ), 				( "LSHI" ), 				( "RSHFT" ), 				( "LCTRL" ), 				( "RCTRL" ), 				( "LMENU" ), 				( "RMENU" ), 				( "-" ), 				( "-" ), 				( "-" ), 				( "-" ), 				( "-" ), 				( "-" ), 				( "-" ), 				( "-" ), 				( "-" ), 				( "-" ), 				( "NTRK" ), 				( "PTRK" ), 				( "STOP" ), 				( "PLAY" ), 				( "-" ), 				( "-" ), 				( "-" ), 				( "-" ), 				( "-" ), 				( "-" ), 				( ";" ), 				( "+" ), 				( ")," ), 				( "-" ), 				( "." ), 				( "/" ), 				( "~" ), 				( "-" ), 				( "-" ), 				( "-" ), 				( "-" ), 				( "-" ), 				( "-" ), 				( "-" ), 				( "-" ), 				( "-" ), 				( "-" ), 				( "-" ), 				( "-" ), 				( "-" ), 				( "-" ), 				( "-" ), 				( "-" ), 				( "-" ), 				( "-" ), 				( "-" ), 				( "-" ), 				( "-" ), 				( "-" ), 				( "-" ), 				( "-" ), 				( "-" ), 				( "-" ), 				( "{" ), 				( "\\|" ), 				( "}" ), 				( "'\"" ), 				( "-" ), 				( "-" ), 				( "-" ), 				( "-" ), 				( "-" ), 				( "-" ), 				( "-" ), 				( "-" ), 				( "-" ), 				( "-" ), 				( "-" ), 				( "-" ), 				( "-" ), 				( "-" ), 				( "-" ), 				( "-" ), 				( "-" ), 				( "-" ), 				( "-" ), 				( "-" ), 				( "-" ), 				( "-" ), 				( "-" ), 				( "-" ), 				( "-" ), 				( "-" ), 				( "-" ), 				( "-" ), 				( "-" ), 				( "-" ) };

	// == MODIFICATIONS
	enum ngui_windowflags {
		windowflags_none,
		windowflags_centertitle = ( 1 >> 0 ),
		windowflags_showcursor = ( 2 >> 0 )
	};

	enum ngui_childflags {
		childflags_none,
		childflags_centertitle = ( 1 >> 0 )
	};

	// == SETTINGS
	enum class controls : int {
		checkbox,
		slider,
		listbox,
		colorpicker,
		button,
		label,
		divider,
		textbox,
		child,
		window,
		combobox,
		keybind,
		multicombobox
	};

	struct window_ctx_t {
		unsigned long font = 0;
		bool has_subtabs;
		vec2 size = vec2( 385, 320 );
		vec2 pos = vec2( 386, 256 );
		vec2 cursor_pos = vec2( 0, 0 );
		vec2 child_pos = vec2( 0, 0 );
		uint32_t blocking;
		int flags;
		bool is_dragging;
		bool override_input;
		bool control_opened;
		controls current_control;
		std::string typed_chars;
	}; inline window_ctx_t window_ctx;

	struct theme_t {
		color window_background = color( 20, 20, 20, 255 );
		color window_title = color( 40, 40, 40, 255 );
		color child_title = color( 50, 50, 50, 255 );
		color window_child = color( 35, 35, 35, 255 );
		color window_outline = color( 0, 0, 0, 0 );
		color window_title_text = color( 200, 200, 200, 255 );
		color window_text = color( 200, 200, 200, 255 );
		color window_accent = color(200, 200, 200, 255);
		color control_active = color( 30, 30, 30, 255 );
		color control_inactive = color( 40, 40, 40, 255 );
		color control_disabled = color( 55, 55, 55, 255 );
		color selection_active = color( 55, 55, 55, 255 );
		color selection_inactive = color( 60, 60, 60, 255 );

		int color_offset = 155;
		int extended_control_width = 175;
		int tab_height = 15;
		int subtab_height = 15;
		int control_spacing = 5;
	}; inline theme_t theme;

	struct helpers_t {
		vec2 mouse_pos;
		bool disable_bound;
		controls controltype_opened;
		struct {
			int value;
			std::deque<bool> multi_value;
			std::vector<const char*> items;
			rect control_area;
		} combobox;
		struct {
			color value;
			rect control_area;
		} colorpicker;
	}; inline helpers_t helpers;

	// == DRAW LIST: can be used for using different renderers by making a wrapper
	class c_draw_list {
	public:
		using draw_rectangle_fn = std::add_pointer_t< void( int x, int y, int w, int h, color col ) noexcept >;
		using draw_outline_fn = std::add_pointer_t< void( int x, int y, int w, int h, color col ) noexcept >;
		using draw_text_fn = std::add_pointer_t< void( int x, int y, unsigned long font, ngui::color col, bool center, const char* text ) noexcept >;
		using text_size_fn = std::add_pointer_t< void( const char* text, unsigned long font, int& w, int& h ) noexcept >;
		using gradient_fn = std::add_pointer_t< void( int x, int y, int w, int h, color col, color col2, bool horizontal ) noexcept >;

		text_size_fn get_text_size;
		gradient_fn gradient;
		draw_rectangle_fn filled_rect;
		draw_outline_fn rect;
		draw_text_fn text;
	}; inline c_draw_list draw;

	// == GUI API
	NGUI_API void example_window( );

	NGUI_API long __stdcall wndproc( HWND hwnd, UINT msg, WPARAM wparam, LPARAM lparam );

	NGUI_API void poll_input( const char* window );

	NGUI_API bool begin_window( const char* title, bool& opened, unsigned long font, const int flags = 0 );

	NGUI_API bool child( const char* title, vec2 size, const int flags = 0 );

	NGUI_API bool tabs( std::vector<const char*> items, int& value );

	NGUI_API bool subtabs( std::vector<const char*> items, int& value );

	NGUI_API void checkbox( const char* id, bool& value );

	NGUI_API bool key_method( int key, int type );

	NGUI_API void seperate_column( int x, int y );

	NGUI_API void divider( const char* id );

	NGUI_API bool button( const char* id );

	NGUI_API void slider_int( const char* id, int& value, int max );

	NGUI_API void slider_color( const char* id, int& value, color col );

	NGUI_API void listbox( const char* id, std::vector<const char*> items, int& value );

	NGUI_API void end_window( );

	NGUI_API void combobox( const char* id, int& value, std::vector<const char*> items );

	NGUI_API void colorpicker( const char* id, color& value );

	NGUI_API void multi_combobox( const char* id, std::deque<bool>& value, std::vector<const char*> items );

	NGUI_API void text( const char* id );

	NGUI_API void keybind( const char* id, int& key, int& type );

	NGUI_API void textbox( const char* id, std::string& value, bool censor = false );
}

#endif // !NGUI_HH