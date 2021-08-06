#include "menu.hpp"

static int type;
static int type2;
static int type3;

void menu::render( ) {
	ngui::poll_input( "Counter-Strike: Global Offensive" );
	if ( ngui::begin_window( " eclipse for Counter-Strike:Global Offensive", opened, fonts::menu_bold, ngui::windowflags_none ) ) {
		if ( ngui::tabs( { "build: 8/6/2021"}, tab_value)) {
			if ( ngui::child( "main", ngui::vec2( 185, 280 ) ) ) {
				ngui::checkbox("backtrack", options::vars["backtrack.enable"].val.b);
				ngui::slider_int("backtrack limit", options::vars["backtrack.limit"].val.i, 14);
				ngui::checkbox("bhop", options::vars["misc.bhop"].val.b);
				ngui::checkbox("unlimited duck", options::vars["misc.unlimited_duck_stamina"].val.b);
				ngui::checkbox("edgebug", options::vars["misc.edgebug"].val.b);
				ngui::keybind("edgebug key", options::vars["misc.edgebug.key"].val.i, type);
				ngui::checkbox("jumpbug", options::vars["misc.jumpbug"].val.b);
				ngui::keybind("jumpbug key" , options::vars["misc.jumpbug.key"].val.i, type2);
				ngui::checkbox("strafe arrows", options::vars["misc.arrows"].val.b);
				ngui::checkbox("indicators", options::vars["misc.indicators"].val.b);
				ngui::checkbox("velocity", options::vars["misc.velocity_indicator"].val.b);
				ngui::checkbox("edgebug detection", options::vars["misc.edgebug_detection"].val.b);
				ngui::keybind("thirdperson", options::vars["misc.thirdperson"].val.i, type3);
			}
			ngui::seperate_column( 190, 0 );
			if ( ngui::child( "other", ngui::vec2( 185, 280 ) ) ) {
				ngui::checkbox("esp", options::vars["esp.enable"].val.b);
				ngui::checkbox("chams", options::vars["chams.enemy.visible"].val.b);
				ngui::slider_int("pearlescent", options::vars["chams.enemy.pearlescent"].val.i, 100);
				ngui::slider_int("phong", options::vars["chams.enemy.phong"].val.i, 100);
				ngui::checkbox("glow", options::vars["esp.glow"].val.b);
				ngui::checkbox("glow enemy", options::vars["esp.enemy_glow"].val.b);
				ngui::checkbox("aspect ratio", options::vars["misc.aspect_ratio.enable"].val.b);
				ngui::slider_int("aspect ratio value", options::vars["misc.aspect_ratio"].val.i, 100);
				ngui::checkbox("watermark", options::vars["misc.watermark"].val.b);
				ngui::checkbox("no smoke", options::vars["misc.no_smoke"].val.b);
				ngui::checkbox("no flash", options::vars["misc.no_flash"].val.b);
				ngui::checkbox("sniper crosshair", options::vars["misc.sniper_crosshair"].val.b);
				ngui::checkbox("spectator list", options::vars["misc.spectatorlist"].val.b);
				ngui::checkbox("hitsound", options::vars["misc.hitsound"].val.b);
			}
		}
		ngui::end_window( );
	}
}