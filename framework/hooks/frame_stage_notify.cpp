#include "frame_stage_notify.hpp"
#include "../features/misc.hpp"

decltype( &hooks::frame_stage_notify ) hooks::original::frame_stage_notify = nullptr;

void __fastcall hooks::frame_stage_notify( void* ecx, void* edx, int stage ) {
	g::local = ( !csgo::i::engine->is_connected( ) || !csgo::i::engine->is_in_game( ) ) ? nullptr : csgo::i::ent_list->get< player_t* >( csgo::i::engine->get_local_player( ) );

	constexpr std::array<const char*, 16> smoke_mats = {
		"particle/vistasmokev1/vistasmokev1",
		"particle/vistasmokev1/vistasmokev1_emods",
		"particle/vistasmokev1/vistasmokev1_emods_impactdust",
		"particle/vistasmokev1/vistasmokev1_fire",
		"particle/vistasmokev1/vistasmokev1_fire2",
		"particle/vistasmokev1/vistasmokev1_fire2_fogged",
		"particle/vistasmokev1/vistasmokev1_min_depth_nearcull",
		"particle/vistasmokev1/vistasmokev1_nearcull",
		"particle/vistasmokev1/vistasmokev1_nearcull_fog",
		"particle/vistasmokev1/vistasmokev1_nearcull_nodepth",
		"particle/vistasmokev1/vistasmokev1_no_rgb",
		"particle/vistasmokev1/vistasmokev1_smokegrenade",
		"particle/vistasmokev1/vistasmokev4_addself_nearcull",
		"particle/vistasmokev1/vistasmokev4_emods_nocul",
		"particle/vistasmokev1/vistasmokev4_nearcull",
		"particle/vistasmokev1/vistasmokev4_nocull"
	};

	switch ( stage ) {
	case frame_net_update_end:
		break;
	case frame_render_start:
		features::misc::no_flash();
		features::misc::no_smoke();
		break;
	}

	original::frame_stage_notify( ecx, edx, stage );
}