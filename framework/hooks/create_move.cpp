#include "create_move.hpp"
#include "../features/misc.hpp"
#include "../features/backtrack.hpp"

decltype( &hooks::create_move ) hooks::original::create_move = nullptr;

bool __fastcall hooks::create_move( void* ecx, void* edx, float sampletime, ucmd_t* ucmd ) {
	auto ret = original::create_move(ecx, edx, sampletime, ucmd);
	if (!ucmd->m_cmdnum)
		return ret;

	auto local = csgo::i::ent_list->get<player_t*>(csgo::i::engine->get_local_player());

	if (ret)
		csgo::i::pred->set_local_viewangles(ucmd->m_angs);

	g::ucmd = ucmd;

	const auto backup_angle = ucmd->m_angs;
	const auto backup_sidemove = ucmd->m_smove;
	const auto backup_forwardmove = ucmd->m_fmove;
	const int unpredicted_flags = g::local->flags();
	const float unpredicted_velocity = std::floor(g::local->vel().z);

	features::misc::bhop(ucmd);
	features::misc::duck_stamina(ucmd);
	features::misc::sniper_crosshair();

	//features::misc::clantag_changer();
	features::prediction::run( [&] ( ) {
		features::backtrack::run(ucmd, local);
		features::backtrack::update();
		} );

	const int predicted_flags = g::local->flags();
	const int predicted_velocity = std::floor(g::local->vel().z);

	features::misc::edgebug(ucmd, unpredicted_flags, predicted_flags, unpredicted_velocity);
	features::misc::jumpbug(ucmd, unpredicted_flags);
	features::misc::edgebug_detection(ucmd, unpredicted_velocity);

	csgo::clamp( ucmd->m_angs );
	csgo::rotate_movement( ucmd );
	csgo::fix_movement( ucmd );

	*( bool* ) ( *( uintptr_t* ) ( uintptr_t( _AddressOfReturnAddress( ) ) - 4 ) - 28 ) = g::send_packet;

	return false;
}