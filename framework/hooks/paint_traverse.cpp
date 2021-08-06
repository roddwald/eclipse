#include "paint_traverse.hpp"
#include "../menu/menu.hpp"
#include "../features/esp.hpp"
#include "../features/misc.hpp"

decltype( &hooks::paint_traverse ) hooks::original::paint_traverse = nullptr;

void __fastcall hooks::paint_traverse( void* ecx, void* edx, int panel, bool force_repaint, bool allow_force ) {
	const auto panel_hash = fnv1a::hash( csgo::i::panel->get_name( panel ) );

	static auto override_post_processing_disable = pattern::search( client_dll, _( "80 3D ? ? ? ? ? 53 56 57 0F 85" ) ).add( 0x2 ).deref( ).get<bool*>( );

	switch ( panel_hash ) {
	case fnv1a::hash_const( "MatSystemTopPanel" ):
		features::esp::paint();
		features::misc::sniper_crosshair();
		features::misc::aspect_ratio();
		features::misc::spectator_list();
		menu::render( );
		break;
	case fnv1a::hash_const( "FocusOverlayPanel" ):
		break;
	}

	{ 	// reload fonts
		int screenx, screeny;
		static int old_screenx, old_screeny;
		csgo::i::engine->get_screen_size( screenx, screeny );

		if ( old_screenx != screenx || old_screeny != screeny ) {
			visual_fonts.init();
			fonts::initialize( );
			old_screenx = screenx;
			old_screeny = screeny;
		}
	}

	original::paint_traverse( ecx, edx, panel, force_repaint, allow_force );
}