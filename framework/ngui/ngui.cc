#include "ngui.hh"

//=========================================================================
void ngui::poll_input( const char* window ) {
	HWND hwnd = FindWindowA( NULL, window );

	if ( !hwnd )
		throw std::exception( "failed to target window!" );

	for ( int i = 0; i < 256; i++ ) {
		utils::input::old_keystate[i] = utils::input::keystate[i];
		utils::input::keystate[i] = GetAsyncKeyState( i );
	}

	POINT pos;
	GetCursorPos( &pos );
	ScreenToClient( hwnd, &pos );

	helpers.mouse_pos = vec2( pos.x, pos.y );
}

//=========================================================================
long __stdcall ngui::wndproc( HWND hwnd, UINT msg, WPARAM wparam, LPARAM lparam ) {
	switch ( msg ) {
	case WM_CHAR:
		if ( wparam > 0 && wparam < 256 && window_ctx.control_opened )
			window_ctx.typed_chars += wchar_t( wparam );
		break;
	}
	return true;
}
//=========================================================================
//
//=========================================================================
bool ngui::utils::input::in_region( int x, int y, int w, int h ) {
	if ( !helpers.disable_bound && helpers.mouse_pos.x > x && helpers.mouse_pos.y > y && helpers.mouse_pos.x < x + w && helpers.mouse_pos.y < y + h )
		return true;

	return false;
}

bool ngui::utils::input::key_pressed( int i ) {
	return keystate[i] && !old_keystate[i];
}

bool ngui::utils::input::key_down( int i ) {
	return keystate[i];
}

bool ngui::utils::input::key_released( int i ) {
	return !keystate[i] && old_keystate[i];
}
//=========================================================================

//=========================================================================
bool ngui::begin_window( const char* title, bool& opened, unsigned long font, const int flags ) {
	if ( utils::input::key_pressed( VK_INSERT ) ||
		utils::input::key_pressed( VK_HOME )
		)
		opened = !opened;

	if ( !opened )
		return false;

	int titlebar_height = 15;
	const auto in_region = utils::input::in_region( window_ctx.pos.x, window_ctx.pos.y, window_ctx.size.x, titlebar_height );

	bool click = false;
	if ( GetAsyncKeyState( VK_LBUTTON ) )
		click = true;

	static vec2 drag = vec2( 0, 0 );

	if ( window_ctx.is_dragging && !click ) {
		window_ctx.is_dragging = false;
	}

	if ( window_ctx.is_dragging && click ) {
		window_ctx.pos.x = helpers.mouse_pos.x - drag.x;
		window_ctx.pos.y = helpers.mouse_pos.y - drag.y;
	}

	if ( in_region ) {
		window_ctx.is_dragging = true;
		drag.x = helpers.mouse_pos.x - window_ctx.pos.x;
		drag.y = helpers.mouse_pos.y - window_ctx.pos.y;
	}

	draw.filled_rect( window_ctx.pos.x, window_ctx.pos.y, window_ctx.size.x, window_ctx.size.y, theme.window_background );
	draw.filled_rect( window_ctx.pos.x, window_ctx.pos.y, window_ctx.size.x, titlebar_height, theme.window_title );

	if ( !( flags & ngui_windowflags::windowflags_centertitle ) ) {
		draw.text( window_ctx.pos.x + 1, window_ctx.pos.y + 1, font, theme.window_title_text, false, title );
	}
	else {
		draw.text( window_ctx.pos.x + 1 + window_ctx.size.x / 2, window_ctx.pos.y + 1, font, theme.window_title_text, true, title );
	}

	draw.rect( window_ctx.pos.x, window_ctx.pos.y, window_ctx.size.x, window_ctx.size.y, theme.window_outline );
	draw.rect( window_ctx.pos.x, window_ctx.pos.y, window_ctx.size.x, titlebar_height, theme.window_outline );

	// push variables
	window_ctx.current_control = controls::window;
	window_ctx.font = font;
	window_ctx.flags = flags;
	window_ctx.cursor_pos = vec2( 0, 0 );
	window_ctx.child_pos = window_ctx.pos;
	window_ctx.child_pos.x += 5;
	window_ctx.child_pos.y += window_ctx.has_subtabs ? ( 15 * 2 ) + theme.subtab_height + 5 : 20 + theme.tab_height;

	return true;
}
//=========================================================================

//=========================================================================
void ngui::end_window( ) {
	if ( window_ctx.control_opened ) {
		rect area = rect( helpers.combobox.control_area.x, helpers.combobox.control_area.y, helpers.combobox.control_area.w, helpers.combobox.items.size( ) * 15 );
		switch ( helpers.controltype_opened ) {
		case controls::combobox:
			for ( int i = 0; i < helpers.combobox.items.size( ); i++ ) {
				rect control_area = rect( helpers.combobox.control_area.x, helpers.combobox.control_area.y + i * 15, helpers.combobox.control_area.w, helpers.combobox.control_area.h );
				draw.filled_rect( control_area.x, control_area.y, control_area.w, control_area.h, theme.control_disabled );

				if ( helpers.combobox.value == i )
					draw.filled_rect( control_area.x, control_area.y, control_area.w, control_area.h, theme.control_inactive );

				draw.text( control_area.x + 3, control_area.y + 1, window_ctx.font, theme.window_text, false, helpers.combobox.items[i] );
			}
			draw.rect( area.x, area.y, area.w, area.h, theme.window_outline );
			break;
		case controls::keybind:
			for ( int i = 0; i < helpers.combobox.items.size( ); i++ ) {
				rect control_area = rect( helpers.combobox.control_area.x, helpers.combobox.control_area.y + i * 15, helpers.combobox.control_area.w, helpers.combobox.control_area.h );
				draw.filled_rect( control_area.x, control_area.y, control_area.w, control_area.h, theme.control_disabled );

				if ( helpers.combobox.value == i )
					draw.filled_rect( control_area.x, control_area.y, control_area.w, control_area.h, theme.control_inactive );

				draw.text( control_area.x + 3, control_area.y + 1, window_ctx.font, theme.window_text, false, helpers.combobox.items[i] );
			}

			draw.rect( area.x, area.y, area.w, area.h, theme.window_outline );
			break;
		case controls::multicombobox:
			for ( int i = 0; i < helpers.combobox.items.size( ); i++ ) {
				rect control_area = rect( helpers.combobox.control_area.x, helpers.combobox.control_area.y + i * 15, helpers.combobox.control_area.w, helpers.combobox.control_area.h );
				draw.filled_rect( control_area.x, control_area.y, control_area.w, control_area.h, theme.control_disabled );

				if ( helpers.combobox.multi_value[i] )
					draw.filled_rect( control_area.x, control_area.y, control_area.w, control_area.h, theme.control_inactive );

				draw.text( control_area.x + 3, control_area.y + 1, window_ctx.font, theme.window_text, false, helpers.combobox.items[i] );
			}
			rect area = rect( helpers.combobox.control_area.x, helpers.combobox.control_area.y, helpers.combobox.control_area.w, helpers.combobox.items.size( ) * 15 );
			draw.rect( area.x, area.y, area.w, area.h, theme.window_outline );
			break;
		}
	}

	if ( window_ctx.flags & ngui_windowflags::windowflags_showcursor )
		draw.rect( helpers.mouse_pos.x - 2, helpers.mouse_pos.y - 2, 5, 5, color( 255, 255, 255 ) );

	helpers.disable_bound = false;
}
//=========================================================================

//=========================================================================
void ngui::multi_combobox( const char* id, std::deque < bool >& value, std::vector < const char* > items ) {
	rect control_area = rect( window_ctx.cursor_pos.x, window_ctx.cursor_pos.y + 12, theme.extended_control_width, 15 );
	std::deque < bool >& vec_value = value;

	auto draw_dropdown = [&] ( int x, int y, color col ) {
		for ( int i = 0; i < 3; i++ ) {
			draw.filled_rect( x, y + i * 2, 7, 1, col );
		}
	};

	const auto in_region = utils::input::in_region( control_area.x, control_area.y, control_area.w, control_area.h );

	static bool buffer = false;
	static bool take_input = false;

	if ( !helpers.disable_bound ) {
		bool skip = false;
		if ( utils::input::key_pressed( VK_LBUTTON ) && in_region ) {
			buffer ? ( window_ctx.blocking = utils::hash::hash( id ), window_ctx.control_opened = !window_ctx.control_opened, buffer = false ) : NULL;
			skip = true;
		}
		else if ( in_region ) {
			buffer = true;
		}

		if ( window_ctx.control_opened && window_ctx.blocking == utils::hash::hash( id ) ) {
			int clicked_in_bounds = 0;
			for ( int i = 0; i < items.size( ); i++ ) {
				rect dropdown_area = { control_area.x, control_area.y + i * control_area.h, control_area.w, control_area.h };
				if ( const bool hovered = utils::input::in_region( control_area.x, control_area.y + control_area.h + 1 + i * 15, control_area.w, control_area.h ) && utils::input::key_pressed( VK_LBUTTON ) ) {
					vec_value[i] = !vec_value[i];
					clicked_in_bounds++;
				}
			}

			if ( ( utils::input::key_pressed( VK_LBUTTON ) && clicked_in_bounds <= 0 && !skip ) )
				window_ctx.control_opened = false;
		}
	}

	std::string selected = "";
	for ( int i = 0; i < items.size( ); i++ ) {
		if ( vec_value[i] ) {
			if ( selected.length( ) > 0 )
				selected.append( ", " );

			selected += items[i];

			if ( selected.length( ) > 27 ) {
				selected.resize( 26 );
				selected.append( "..." );
			}
		}
	}

	if ( !selected.length( ) )
		selected = "none";

	draw.text( control_area.x, control_area.y - 15, window_ctx.font, theme.window_text, false, id );
	draw.filled_rect( control_area.x, control_area.y, control_area.w, control_area.h, theme.control_disabled );
	draw.rect( control_area.x, control_area.y, control_area.w, control_area.h, theme.window_outline );
	draw.text( control_area.x + 3, control_area.y + 1, window_ctx.font, theme.window_text, false, selected.c_str( ) );
	draw_dropdown( control_area.x + control_area.w - 12, control_area.y + 5, color( 100, 100, 100 ) );

	if ( ( in_region || window_ctx.control_opened ) && window_ctx.blocking == utils::hash::hash( id ) ) {
		helpers.controltype_opened = controls::multicombobox;

		helpers.combobox.control_area = { control_area.x, control_area.y + 16, control_area.w, control_area.h };
		helpers.combobox.items = items;
		helpers.combobox.multi_value = vec_value;

		helpers.disable_bound = true;
	}

	// push variables
	window_ctx.current_control = controls::multicombobox;
	window_ctx.cursor_pos.y += ( control_area.h + 12 ) + theme.control_spacing;
}
//=========================================================================

//=========================================================================
void ngui::combobox( const char* id, int& value, std::vector < const char* > items ) {
	rect control_area = rect( window_ctx.cursor_pos.x, window_ctx.cursor_pos.y + 12, theme.extended_control_width, 15 );

	auto draw_dropdown = [&] ( int x, int y, color col ) {
		for ( int i = 0; i < 3; i++ ) {
			draw.filled_rect( x, y + i * 2, 7, 1, col );
		}
	};

	const auto in_region = utils::input::in_region( control_area.x, control_area.y, control_area.w, control_area.h );

	static bool buffer = false;
	static bool take_input = false;

	if ( !helpers.disable_bound ) {
		bool skip = false;
		if ( utils::input::key_pressed( VK_LBUTTON ) && in_region ) {
			buffer ? ( window_ctx.blocking = utils::hash::hash( id ), window_ctx.control_opened = !window_ctx.control_opened, buffer = false ) : NULL;
			skip = true;
		}
		else if ( in_region ) {
			buffer = true;
		}

		if ( window_ctx.control_opened && window_ctx.blocking == utils::hash::hash( id ) ) {
			int clicked_in_bounds = 0;
			for ( int i = 0; i < items.size( ); i++ ) {
				rect dropdown_area = { control_area.x, control_area.y + i * control_area.h, control_area.w, control_area.h };
				if ( const bool hovered = utils::input::in_region( control_area.x, control_area.y + control_area.h + 1 + i * 15, control_area.w, control_area.h ) && utils::input::key_pressed( VK_LBUTTON ) ) {
					value = i;
					clicked_in_bounds++;
				}
			}

			if ( ( utils::input::key_pressed( VK_LBUTTON ) && clicked_in_bounds <= 0 && !skip ) )
				window_ctx.control_opened = false;
		}
	}

	draw.text( control_area.x, control_area.y - 15, window_ctx.font, theme.window_text, false, id );
	draw.filled_rect( control_area.x, control_area.y, control_area.w, control_area.h, theme.control_disabled );
	draw.rect( control_area.x, control_area.y, control_area.w, control_area.h, theme.window_outline );
	draw.text( control_area.x + 3, control_area.y + 1, window_ctx.font, theme.window_text, false, items[value] );
	draw_dropdown( control_area.x + control_area.w - 12, control_area.y + 5, color( 100, 100, 100 ) );

	if ( ( in_region || window_ctx.control_opened ) && window_ctx.blocking == utils::hash::hash( id ) ) {
		helpers.controltype_opened = controls::combobox;

		helpers.combobox.control_area = { control_area.x, control_area.y + 16, control_area.w, control_area.h };
		helpers.combobox.items = items;
		helpers.combobox.value = value;

		helpers.disable_bound = true;
	}

	// push variables
	window_ctx.current_control = controls::combobox;
	window_ctx.cursor_pos.y += ( control_area.h + 12 ) + theme.control_spacing;
}
//=========================================================================

//=========================================================================
bool ngui::key_method( int key, int type ) {
	switch ( type ) {
	case 0:
		return false;
		break;
	case 1:
		return GetKeyState( key );
		break;
	case 2:
		return GetAsyncKeyState( key );
		break;
	case 3:
		return true;
		break;
	}
	return false;
}
//=========================================================================

//=========================================================================
void ngui::textbox( const char* id, std::string& value, bool censor ) {
	rect control_area = rect( window_ctx.cursor_pos.x, window_ctx.cursor_pos.y + 12, theme.extended_control_width, 15 );

	const auto in_region = utils::input::in_region( control_area.x, control_area.y, control_area.w, control_area.h );

	if ( utils::input::key_pressed( VK_LBUTTON ) && in_region ) {
		window_ctx.control_opened = !window_ctx.control_opened;
		window_ctx.blocking = utils::hash::hash( id );
		helpers.controltype_opened = controls::textbox;
	}

	if ( window_ctx.blocking == utils::hash::hash( id ) && helpers.controltype_opened == controls::textbox ) {
		if ( utils::input::key_pressed( VK_LBUTTON ) && !in_region )
			window_ctx.control_opened = false;

		if ( window_ctx.control_opened ) {
			for ( int i = 8; i < 256; i++ ) {
				if ( utils::input::key_pressed( i ) ) {
					if ( i == VK_SHIFT )
						continue;

					if ( i == VK_TAB || i == VK_ESCAPE || i == VK_RETURN ) {
						window_ctx.control_opened = false;
						continue;
					}
				}
			}

			if ( window_ctx.control_opened ) {
				if ( !window_ctx.typed_chars.empty( ) && value.size( ) < 32 ) {
					for ( auto& _char : window_ctx.typed_chars ) {
						if ( _char >= 32 && _char < 255 )
							value.push_back( _char );
						else if ( !value.empty( ) && _char == '\b' )
							value.pop_back( );
					}

					window_ctx.typed_chars.erase( );
				}
			}
		}
	}

	std::string str_val = value;

	if ( censor && !str_val.empty( ) )
		std::fill( str_val.begin( ), str_val.end( ), '*' );

	if ( !censor && window_ctx.blocking == utils::hash::hash( id ) && window_ctx.control_opened )
		str_val.append( "_" );

	draw.text( control_area.x, control_area.y - 15, window_ctx.font, theme.window_text, false, id );
	draw.filled_rect( control_area.x, control_area.y, control_area.w, control_area.h, ( window_ctx.blocking == utils::hash::hash( id ) && window_ctx.control_opened ) && ( helpers.controltype_opened == controls::textbox ) ? color( 45, 45, 45 ) : theme.selection_active );
	draw.rect( control_area.x, control_area.y, control_area.w, control_area.h, theme.window_outline );
	draw.text( control_area.x + 3, control_area.y + 1, window_ctx.font, theme.window_text, false, str_val.c_str( ) );

	// push variables
	window_ctx.current_control = controls::textbox;
	window_ctx.cursor_pos.y += ( control_area.h + 12 ) + theme.control_spacing;
}
//=========================================================================

//=========================================================================
void ngui::colorpicker( const char* id, color& value ) {
	rect control_area = rect( window_ctx.cursor_pos.x + theme.color_offset, window_ctx.cursor_pos.y, 20, 10 );

	draw.filled_rect( control_area.x, control_area.y, control_area.w, control_area.h, value );
	draw.rect( control_area.x, control_area.y, control_area.w, control_area.h, theme.window_outline );

	const auto in_region = utils::input::in_region( control_area.x, control_area.y, control_area.w, control_area.h );

	static uint32_t id_selected;
	if ( in_region && utils::input::key_pressed( VK_LBUTTON ) ) {
		window_ctx.control_opened = !window_ctx.control_opened;
		id_selected = utils::hash::hash( id );
	}

	if ( window_ctx.control_opened ) {
		if ( id_selected == utils::hash::hash( id ) ) {
			std::string red = id;
			red += ": red";
			std::string green = id;
			green += ": green";
			std::string blue = id;
			blue += ": blue";
			std::string alpha = id;
			alpha += ": alpha";

			slider_color( red.c_str( ), value.r, value );
			slider_color( green.c_str( ), value.g, value );
			slider_color( blue.c_str( ), value.b, value );
			slider_color( alpha.c_str( ), value.a, value );
		}
	}

	// push variables
	window_ctx.current_control = controls::colorpicker;
}
//=========================================================================

//=========================================================================
void ngui::keybind( const char* id, int& key, int& type ) {
	rect control_area = rect( window_ctx.cursor_pos.x, window_ctx.cursor_pos.y + 12, theme.extended_control_width, 15 );

	const auto in_region = utils::input::in_region( control_area.x, control_area.y, control_area.w, control_area.h );

	static auto is_getting_key = false;
	static auto good_key_name = false;
	const char* key_name = "-";
	{
		static bool take_input = false;
		char buffer[128];

		if ( is_getting_key && window_ctx.blocking == utils::hash::hash( id ) ) {
			key_name = "-";
			for ( int i = 0; i < 255; i++ ) {
				if ( utils::input::key_pressed( i ) ) {
					key = i == VK_ESCAPE ? -1 : i;
					is_getting_key = false;
				}
			}
		}
		else {
			if ( key >= 0 ) {
				key_name = key_str[key];
				if ( key_name ) {
					good_key_name = true;
				}
				else {
					if ( GetKeyNameTextA( key << 16, buffer, 127 ) ) {
						key_name = buffer;
						good_key_name = true;
					}
				}
			}

			if ( !good_key_name ) {
				key_name = "none";
			}
		}
	}

	std::vector< const char* > methods = {
		"Disabled",
		"Toggle",
		"Hold",
		"Always on"
	};

	static bool buffer = false;
	static bool take_input = false;

	if ( !helpers.disable_bound ) {
		bool skip = false;
		//if ( utils::input::key_pressed( VK_RBUTTON ) && in_region ) {
		//	buffer ? ( window_ctx.blocking = utils::hash::hash( id ), window_ctx.control_opened = !window_ctx.control_opened, buffer = false ) : NULL;
		//	skip = true;
		//}
		//else if ( in_region ) {
		//	buffer = true;
		//}

		if ( window_ctx.control_opened && window_ctx.blocking == utils::hash::hash( id ) ) {
			int clicked_in_bounds = 0;
			for ( int i = 0; i < methods.size( ); i++ ) {
				rect dropdown_area = { control_area.x, control_area.y + i * control_area.h, control_area.w, control_area.h };
				if ( const bool hovered = utils::input::in_region( control_area.x, control_area.y + control_area.h + 1 + i * 15, control_area.w, control_area.h ) && utils::input::key_pressed( VK_LBUTTON ) ) {
					type = i;
					clicked_in_bounds++;
					window_ctx.blocking = 0;
				}
			}

			if ( ( utils::input::key_pressed( VK_LBUTTON ) && clicked_in_bounds <= 0 && !skip ) || window_ctx.blocking == 0 )
				window_ctx.control_opened = false;
		}
	}

	if ( utils::input::key_pressed( VK_LBUTTON ) && in_region && !window_ctx.control_opened ) {
		is_getting_key = true;
		window_ctx.blocking = utils::hash::hash( id );
	}

	draw.text( control_area.x, control_area.y - 15, window_ctx.font, theme.window_text, false, id );
	draw.filled_rect( control_area.x, control_area.y, control_area.w, control_area.h, window_ctx.blocking == utils::hash::hash( id ) && is_getting_key ? theme.selection_inactive : theme.control_disabled );
	draw.rect( control_area.x, control_area.y, control_area.w, control_area.h, theme.window_outline );
	draw.text( control_area.x + 3, control_area.y + 1, window_ctx.font, theme.window_text, false, key_name );

	if ( ( in_region || window_ctx.control_opened ) && window_ctx.blocking == utils::hash::hash( id ) ) {
		helpers.controltype_opened = controls::keybind;

		helpers.combobox.control_area = { control_area.x, control_area.y + 16, control_area.w, control_area.h };
		helpers.combobox.items = methods;
		helpers.combobox.value = type;

		helpers.disable_bound = true;
	}

	// push variables
	window_ctx.current_control = controls::keybind;
	window_ctx.cursor_pos.y += ( control_area.h + 12 ) + theme.control_spacing;
}
//=========================================================================

//=========================================================================
void ngui::example_window( ) {
	static bool opened = true;
	static int tab = 0;
	static int key = -1, type;
	static int slider1 = 0;
	static float slider2 = 0;
	static int tab2 = 1;
	static bool check1 = false;
	static std::string sss = "hi";
	static bool check2 = false;
	static int listbox = 0;
	static bool check3 = false;
	static ngui::color col = ngui::color( 255, 255, 255 );
	static int combo1 = false;
	static std::deque<bool> multi = { false, false };

	// removed this due to font expansion
	//ngui::poll_input( "example" );
	//if ( ngui::begin_window( "ngui example window", opened, ( unsigned long ) fonts::def, ngui::windowflags_showcursor ) ) {
	//	if ( ngui::tabs( { "tab1", "tab2" }, tab ) ) {
	//		if ( ngui::child( "aaa", ngui::vec2( 185, 215 ) ) ) {
	//			ngui::checkbox( "aaa", check1 );
	//			ngui::divider( "hello" );
	//			ngui::colorpicker( "##col", col );
	//			ngui::checkbox( "aaa2", check2 );
	//			ngui::combobox( "combo1", combo1, { "item1", "item2" } );
	//			ngui::multi_combobox( "dddd", multi, { "item1", "item2" } );
	//			ngui::slider_int( "slider int", slider1, 100 );
	//			ngui::divider( "no" );
	//			ngui::checkbox( "aaa2", check2 );
	//		}
	//		ngui::seperate_column( 190, 0 );
	//		if ( ngui::child( "asaa", ngui::vec2( 185, 215 ) ) ) {
	//			ngui::listbox( "list1", { "1", "2", "3" }, listbox );
	//			ngui::checkbox( "aaa", check1 );
	//			ngui::text( "aaa hello" );
	//			ngui::checkbox( "aaa", check1 );
	//			ngui::keybind( "fuck you", key, type );
	//			ngui::checkbox( "aaa", check1 );
	//			ngui::textbox( "textbox", sss );
	//			if ( ngui::button( "hello" ) ) {
	//			}

	//			if ( ngui::button( "world" ) ) {
	//			}
	//		}
	//	}
	//	ngui::end_window( );
	//}
}
//=========================================================================

//=========================================================================
bool ngui::child( const char* title, vec2 size, const int flags ) {
	int titlebar_height = 15;

	rect control_area = rect( window_ctx.child_pos.x, window_ctx.child_pos.y, size.x, size.y );

	draw.filled_rect( control_area.x, control_area.y, control_area.w, control_area.h, theme.window_child );
	draw.filled_rect( control_area.x, control_area.y, control_area.w, titlebar_height, theme.child_title );

	if ( !( flags & ngui_childflags::childflags_centertitle ) ) {
		draw.text( control_area.x + 1, control_area.y + 1, window_ctx.font, theme.window_title_text, false, title );
	}
	else {
		draw.text( control_area.x + 1 + control_area.w / 2, control_area.y + 1, window_ctx.font, theme.window_title_text, true, title );
	}

	draw.rect( control_area.x, control_area.y, control_area.w, control_area.h, theme.window_outline );
	draw.rect( control_area.x, control_area.y, control_area.w, titlebar_height, theme.window_outline );

	// push variables
	window_ctx.current_control = controls::child;
	window_ctx.cursor_pos = vec2( control_area.x + 5, control_area.y + 20 );

	return true;
}
//=========================================================================

//=========================================================================
void ngui::checkbox( const char* id, bool& value ) {
	rect control_area = rect( window_ctx.cursor_pos.x, window_ctx.cursor_pos.y, 10, 10 );

	int text_w, text_h;
	draw.get_text_size( id, window_ctx.font, text_w, text_h );

	const auto in_region = utils::input::in_region( control_area.x, control_area.y, control_area.w + text_w + 5, control_area.h );

	draw.filled_rect( control_area.x, control_area.y, control_area.w, control_area.h, theme.control_disabled );

	static bool buffer = false;
	if ( !helpers.disable_bound ) {
		if ( utils::input::key_pressed( VK_LBUTTON ) && in_region ) {
			buffer ? ( value = !value, buffer = false ) : 0;
		}
		else if ( in_region ) {
			buffer = true;
		}
	}

	if ( value )
		draw.filled_rect( control_area.x, control_area.y, control_area.w, control_area.h, theme.window_accent );

	draw.rect( control_area.x, control_area.y, control_area.w, control_area.h, theme.window_outline );

	draw.text( control_area.x + control_area.w + 5, control_area.y - 1, window_ctx.font, theme.window_text, false, id );

	// push variables
	window_ctx.current_control = controls::checkbox;
	window_ctx.cursor_pos.y += control_area.h + theme.control_spacing;
}
//=========================================================================

//=========================================================================
void ngui::slider_int( const char* id, int& value, int max ) {	// note: make more slider types soon
	rect control_area = rect( window_ctx.cursor_pos.x, window_ctx.cursor_pos.y + 12, theme.extended_control_width, 12 );
	float pixel = max / ( float ) control_area.w;

	const auto in_region = utils::input::in_region( control_area.x, control_area.y, control_area.w, control_area.h );

	draw.filled_rect( control_area.x, control_area.y, control_area.w, control_area.h, theme.control_disabled );

	static bool take_input = false;
	static bool buffer = false;
	if ( !helpers.disable_bound ) {
		if ( utils::input::key_pressed( VK_LBUTTON ) && in_region ) {
			buffer ? ( window_ctx.blocking = utils::hash::hash( id ), take_input = !take_input, buffer = false ) : 0;
		}
		else if ( in_region ) {
			buffer = true;
		}
		window_ctx.override_input = take_input;
	}

	if ( utils::input::key_down( VK_LBUTTON ) && take_input && window_ctx.blocking == utils::hash::hash( id ) )
		value = ( helpers.mouse_pos.x - ( window_ctx.cursor_pos.x ) ) * pixel;
	else if ( !utils::input::key_down( VK_LBUTTON ) && !in_region && take_input )
		take_input = false;

	value = std::clamp<int>( value, 0, max );

	if ( value )
		draw.filled_rect( control_area.x, control_area.y, ( value / pixel ), control_area.h, theme.window_accent );

	// this is kinda bad
	int text_w, text_h;
	draw.get_text_size( std::to_string( value ).c_str( ), window_ctx.font, text_w, text_h );

	draw.text( control_area.x, control_area.y - 15, window_ctx.font, theme.window_text, false, id );
	draw.text( control_area.x + control_area.w - text_w, control_area.y - 15, window_ctx.font, theme.window_text, false, std::to_string( value ).c_str( ) );

	draw.rect( control_area.x, control_area.y, control_area.w, control_area.h, theme.window_outline );

	// push variables
	window_ctx.current_control = controls::slider;
	window_ctx.cursor_pos.y += ( control_area.h + 12 ) + theme.control_spacing;
}
//=========================================================================

//=========================================================================
void ngui::slider_color( const char* id, int& value, color col ) {	// note: make more slider types soon
	rect control_area = rect( window_ctx.cursor_pos.x, window_ctx.cursor_pos.y + 12, theme.extended_control_width, 12 );
	float pixel = 255 / ( float ) control_area.w;

	const auto in_region = utils::input::in_region( control_area.x, control_area.y, control_area.w, control_area.h );

	draw.filled_rect( control_area.x, control_area.y, control_area.w, control_area.h, theme.control_disabled );

	static bool take_input = false;
	static bool buffer = false;
	if ( !helpers.disable_bound ) {
		if ( utils::input::key_pressed( VK_LBUTTON ) && in_region ) {
			buffer ? ( window_ctx.blocking = utils::hash::hash( id ), take_input = !take_input, buffer = false ) : 0;
		}
		else if ( in_region ) {
			buffer = true;
		}
		window_ctx.override_input = take_input;
	}

	if ( utils::input::key_down( VK_LBUTTON ) && take_input && window_ctx.blocking == utils::hash::hash( id ) )
		value = ( helpers.mouse_pos.x - ( window_ctx.cursor_pos.x ) ) * pixel;
	else if ( !utils::input::key_down( VK_LBUTTON ) && !in_region && take_input )
		take_input = false;

	value = std::clamp<int>( value, 0, 255 );

	if ( value )
		draw.filled_rect( control_area.x, control_area.y, ( value / pixel ), control_area.h, col );

	// this is kinda bad
	int text_w, text_h;
	draw.get_text_size( std::to_string( value ).c_str( ), window_ctx.font, text_w, text_h );

	draw.text( control_area.x, control_area.y - 15, window_ctx.font, theme.window_text, false, id );
	draw.text( control_area.x + control_area.w - text_w, control_area.y - 15, window_ctx.font, theme.window_text, false, std::to_string( value ).c_str( ) );

	draw.rect( control_area.x, control_area.y, control_area.w, control_area.h, theme.window_outline );

	// push variables
	window_ctx.current_control = controls::slider;
	window_ctx.cursor_pos.y += ( control_area.h + 12 ) + theme.control_spacing;
}
//=========================================================================

//=========================================================================
void ngui::listbox( const char* id, std::vector< const char* > items, int& value ) {
	for ( int i = 0; i < items.size( ); i++ ) {
		rect control_area = rect( window_ctx.cursor_pos.x, window_ctx.cursor_pos.y + i * 15, theme.extended_control_width, 15 );

		draw.filled_rect( control_area.x, control_area.y, control_area.w, control_area.h, theme.selection_active );

		if ( const auto in_region = utils::input::in_region( control_area.x, control_area.y, control_area.w, control_area.h ) && utils::input::key_pressed( VK_LBUTTON ) && !window_ctx.override_input )
			value = i;

		if ( value == i )
			draw.filled_rect( control_area.x, control_area.y, control_area.w, control_area.h, theme.control_inactive );

		draw.rect( control_area.x, control_area.y, control_area.w, control_area.h, theme.window_outline );

		draw.text( control_area.x + 3, control_area.y + 1, window_ctx.font, theme.window_text, false, items[i] );
	}

	// push variables
	window_ctx.current_control = controls::listbox;
	window_ctx.cursor_pos.y += ( items.size( ) * 15 ) + theme.control_spacing;
}
//=========================================================================

//=========================================================================
void ngui::divider( const char* id ) {
	rect control_area = rect( window_ctx.cursor_pos.x, window_ctx.cursor_pos.y + 2, theme.extended_control_width, 1 );

	draw.filled_rect( control_area.x, control_area.y, control_area.w, control_area.h, theme.control_disabled );

	int text_w, text_h;
	draw.get_text_size( id, window_ctx.font, text_w, text_h );

	draw.rect( control_area.x + 3, control_area.y, text_w + 4, text_h, theme.window_child );
	draw.text( control_area.x + 6, control_area.y - 5, window_ctx.font, theme.window_text, false, id );

	// push variables
	window_ctx.current_control = controls::divider;
	window_ctx.cursor_pos.y += control_area.h + 10;
}
//=========================================================================

//=========================================================================
void ngui::seperate_column( int x, int y ) {
	// repositon widgets
	window_ctx.child_pos.x += x;
	window_ctx.child_pos.y += y;
}
//=========================================================================

//=========================================================================
bool ngui::tabs( std::vector<const char*> items, int& value ) {
	for ( int i = 0; i < items.size( ); i++ ) {
		rect control_area = rect( window_ctx.pos.x + i * ( window_ctx.size.x / items.size( ) ), window_ctx.pos.y + 15, ( window_ctx.size.x / items.size( ) ), theme.tab_height );

		draw.filled_rect( control_area.x, control_area.y, control_area.w, control_area.h, theme.control_inactive );

		if ( const auto in_region = utils::input::in_region( control_area.x, control_area.y, control_area.w, control_area.h ) && utils::input::key_pressed( VK_LBUTTON ) )
			value = i;

		if ( value == i ) {
			draw.filled_rect( control_area.x, control_area.y, control_area.w, control_area.h, theme.control_active );
			//draw.filled_rect( control_area.x, control_area.y + control_area.h - 1, control_area.w, 1, theme.window_accent );
		}

		draw.rect( control_area.x, control_area.y, control_area.w, control_area.h, theme.window_outline );

		int w, h;
		draw.get_text_size( items[i], window_ctx.font, w, h );

		draw.text( control_area.x + 1 + control_area.w / 2, control_area.y + control_area.h / 2 - h / 2, window_ctx.font, theme.window_text, true, items[i] );
	}

	return true;
}
//=========================================================================

//=========================================================================
bool ngui::button( const char* id ) {
	rect control_area = rect( window_ctx.cursor_pos.x, window_ctx.cursor_pos.y, theme.extended_control_width, 15 );

	const auto in_region = utils::input::in_region( control_area.x, control_area.y, control_area.w, control_area.h );
	if ( in_region && utils::input::key_down( VK_LBUTTON ) && !window_ctx.override_input )
		draw.filled_rect( control_area.x, control_area.y, control_area.w, control_area.h, theme.selection_active );
	else
		draw.filled_rect( control_area.x, control_area.y, control_area.w, control_area.h, theme.selection_inactive );

	draw.text( control_area.x + 1 + control_area.w / 2, control_area.y + 1, window_ctx.font, theme.window_text, true, id );

	draw.rect( control_area.x, control_area.y, control_area.w, control_area.h, theme.window_outline );

	// push variables
	window_ctx.current_control = controls::button;
	window_ctx.cursor_pos.y += control_area.h + theme.control_spacing;

	if ( in_region && utils::input::key_pressed( VK_LBUTTON ) && !window_ctx.override_input )
		return true;

	return false;
}
//=========================================================================

//=========================================================================
void ngui::text( const char* id ) {
	int w, h;
	draw.get_text_size( id, window_ctx.font, w, h );
	rect control_area = rect( window_ctx.cursor_pos.x, window_ctx.cursor_pos.y, w, h );

	draw.text( control_area.x, control_area.y, window_ctx.font, theme.window_text, false, id );

	// push variables
	window_ctx.current_control = controls::label;
	window_ctx.cursor_pos.y += control_area.h + theme.control_spacing;
}
//=========================================================================

bool ngui::subtabs( std::vector<const char*> items, int& value ) {
	for ( int i = 0; i < items.size( ); i++ ) {
		rect control_area = rect( window_ctx.pos.x + i * ( window_ctx.size.x / items.size( ) ), window_ctx.pos.y + 30, ( window_ctx.size.x / items.size( ) ), theme.subtab_height );

		draw.filled_rect( control_area.x, control_area.y, control_area.w, control_area.h, theme.control_inactive );

		if ( const auto in_region = utils::input::in_region( control_area.x, control_area.y, control_area.w, control_area.h ) && utils::input::key_pressed( VK_LBUTTON ) )
			value = i;

		if ( value == i ) {
			draw.filled_rect( control_area.x, control_area.y, control_area.w, control_area.h, theme.control_active );
			//draw.filled_rect( control_area.x, control_area.y + control_area.h - 1, control_area.w, 1, theme.window_accent );
		}

		draw.rect( control_area.x, control_area.y, control_area.w, control_area.h, theme.window_outline );

		draw.text( control_area.x + 1 + control_area.w / 2, control_area.y + 1, window_ctx.font, theme.window_text, true, items[i] );
	}

	window_ctx.has_subtabs = true;

	return true;
}
//=========================================================================