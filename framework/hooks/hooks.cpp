#include "hooks.hpp"

#include "../renderer/d3d9.hpp"

/* utilities */
#include "../utils/event_handler.hpp"
#include "../utils/xor.hpp"
#include "../utils/draw.hpp"

/* hooks */
#include "create_move.hpp"
#include "draw_model_execute.hpp"
#include "list_leaves_in_box.hpp"
#include "frame_stage_notify.hpp"
#include "get_bool.hpp"
#include "get_eye_angles.hpp"
#include "override_view.hpp"
#include "paint_traverse.hpp"
#include "lock_cursor.hpp"
#include "fire_event.hpp"
#include "scene_end.hpp"
#include "end_scene.hpp"
#include "reset.hpp"

#include "../menu/menu.hpp"

/* handle our input here */
long __stdcall wndproc( HWND hwnd, std::uint32_t msg, std::uintptr_t wparam, std::uint32_t lparam ) {
	ngui::wndproc(hwnd, msg, wparam, lparam);

	if ( menu::opened )
		return true;

	return ::CallWindowProc( g_wndproc, hwnd, msg, wparam, lparam );
}

namespace ngui_wrapper {
	void rectangle( int x, int y, int w, int h, ngui::color col ) noexcept {
		draw::rectangle( x, y, w, h, color( col.r, col.g, col.b, col.a ) );
	}

	void gradient( int x, int y, int w, int h, ngui::color col, ngui::color col2, bool horizontal ) noexcept {
		draw::gradient( x, y, w, h, color( col.r, col.g, col.b, col.a ), color( col2.r, col2.g, col2.b, col2.a ), horizontal );
	}

	void outline( int x, int y, int w, int h, ngui::color col ) noexcept {
		draw::outline( x, y, w, h, color( col.r, col.g, col.b, col.a ) );
	}

	void get_text_size( const char* text, unsigned long font, int& w, int& h ) noexcept {
		w = draw::get_text_size( font, text ).right;
		h = draw::get_text_size( font, text ).bottom;
	}

	void text( int x, int y, unsigned long font, ngui::color col, bool center, const char* text ) noexcept {
		draw::string( x, y, color( col.r, col.g, col.b, col.a ), font, center, text );
	}
};

bool hooks::init( ) {
	auto create_hook = [] ( LPVOID target, LPVOID detour, LPVOID* original ) -> bool {
		if ( MH_CreateHook( target, detour, original ) != MH_OK ) {
			throw std::exception( "failed to hook, debug." );
			return false;
		}
		return true;
	};

	ngui::draw.filled_rect = ngui_wrapper::rectangle;
	ngui::draw.gradient = ngui_wrapper::gradient;
	ngui::draw.rect = ngui_wrapper::outline;
	ngui::draw.get_text_size = ngui_wrapper::get_text_size;
	ngui::draw.text = ngui_wrapper::text;

	const auto _c_csplayer_doextraboneprocessing = pattern::search( client_dll, _( "55 8B EC 83 E4 F8 81 EC FC 00 00 00 53 56 8B F1 57" ) ).get< void* >( );
	const auto _convar_getbool = pattern::search( client_dll, _( "8B 51 1C 3B D1 75 06" ) ).get< void* >( );
	const auto _overrideview = pattern::search( client_dll, _( "55 8B EC 83 E4 F8 83 EC 58 56 57 8B 3D ? ? ? ? 85 FF" ) ).get< void* >( );
	const auto _c_csplayer_get_eye_angles = pattern::search( client_dll, _( "56 8B F1 85 F6 74 32" ) ).get< void* >( );
	const auto _clientmodeshared_createmove = pattern::search( client_dll, _( "55 8B EC 8B 0D ? ? ? ? 85 C9 75 06 B0" ) ).get< decltype( &hooks::create_move ) >( );
	const auto _chlclient_framestagenotify = pattern::search( client_dll, _( "55 8B EC 8B 0D ? ? ? ? 8B 01 8B 80 74 01 00 00 FF D0 A2" ) ).get< decltype( &hooks::frame_stage_notify ) >( );
	const auto _c_baseanimating_setupbones = pattern::search( client_dll, _( "55 8B EC 83 E4 F0 B8 ? ? ? ? E8 ? ? ? ? 56 57 8B F9" ) ).get< void* >( );
	const auto _vguimatsurface_lockcursor = pattern::search( surface_dll, _( "80 3D ? ? ? ? 00 8B 91 A4 02 00 00 8B 0D ? ? ? ? C6 05 ? ? ? ? 01" ) ).get< decltype( &hooks::lock_cursor ) >( );

	const auto _panel_painttraverse = vfunc< decltype( &hooks::paint_traverse ) >( csgo::i::panel, ( int ) vtable_idx::painttraverse );
	const auto _renderview_sceneend = vfunc< decltype( &hooks::scene_end ) >( csgo::i::render_view, ( int ) vtable_idx::sceneend );
	const auto _engine_fireevent = vfunc< void* >( csgo::i::engine, ( int ) vtable_idx::fireevent );
	const auto _device_endscene = vfunc< decltype( &hooks::end_scene ) >( csgo::i::dev, ( int ) vtable_idx::endscene );
	const auto _device_reset = vfunc< decltype( &hooks::reset ) >( csgo::i::dev, ( int ) vtable_idx::reset );
	const auto _modelrender_drawmodelexecute = vfunc< decltype( &hooks::draw_model_execute ) >( csgo::i::mdl_render, ( int ) vtable_idx::drawmodelexecute );
	const auto _enginebsptreequery_listleavesinbox = vfunc< decltype( &hooks::list_leaves_in_box )>( csgo::i::engine->get_bsp_tree_query( ), ( int ) vtable_idx::listleavesinbox );

	/* initialize minhook */
	if ( MH_Initialize( ) != MH_OK ) {
		throw _( "failed to initialize minhook!" );
		return false;
	}

	/* create each and every one of our hooks */
	if ( !create_hook( _panel_painttraverse, hooks::paint_traverse, ( void** ) &original::paint_traverse ) )
		return false;

	if ( !create_hook( _renderview_sceneend, hooks::scene_end, ( void** ) &original::scene_end ) )
		return false;

	if ( !create_hook( _engine_fireevent, hooks::fire_event, ( void** ) &original::fire_event ) )
		return false;

	if ( !create_hook( _device_endscene, hooks::end_scene, ( void** ) &original::end_scene ) )
		return false;

	if ( !create_hook( _device_reset, hooks::reset, ( void** ) &original::reset ) )
		return false;

	if ( !create_hook( _modelrender_drawmodelexecute, hooks::draw_model_execute, ( void** ) &original::draw_model_execute ) )
		return false;

	if ( !create_hook( _clientmodeshared_createmove, hooks::create_move, ( void** ) &original::create_move ) )
		return false;

	if ( !create_hook( _chlclient_framestagenotify, hooks::frame_stage_notify, ( void** ) &original::frame_stage_notify ) )
		return false;

	if ( !create_hook( _vguimatsurface_lockcursor, hooks::lock_cursor, ( void** ) &original::lock_cursor ) )
		return false;

	if ( !create_hook( _c_csplayer_get_eye_angles, hooks::get_eye_angles, ( void** ) &original::get_eye_angles ) )
		return false;

	if ( !create_hook( _convar_getbool, hooks::get_bool, ( void** ) &original::get_bool ) )
		return false;

	if ( !create_hook( _overrideview, hooks::override_view, ( void** ) &original::override_view ) )
		return false;

	if ( !create_hook( _enginebsptreequery_listleavesinbox, hooks::list_leaves_in_box, ( void** ) &original::list_leaves_in_box ) )
		return false;

	/* make sure to enable all of our hooks, without this nothing will work! */
	MH_EnableHook( MH_ALL_HOOKS );

	/* hook wndproc handler so we can intercept input. */
	g_wndproc = ( WNDPROC ) SetWindowLongA( FindWindowA( _( "Valve001" ), nullptr ), GWLP_WNDPROC, ( long ) wndproc );

	/* initialize our event handler */
	event_handler = std::make_unique< c_event_handler >( );
	return true;
}