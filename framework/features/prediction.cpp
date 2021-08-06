#include "../features/prediction.hpp"
#include "../globals.hpp"

float predicted_curtime = 0.0f;

namespace prediction_util {
	void start(ucmd_t* ucmd) {
		features::prediction::in_prediction = true;

		if (!csgo::i::engine->is_in_game() || !ucmd || !g::local || !g::local->alive())
			return;

		if (!movedata)
			movedata = malloc(256);

		if (!prediction_player || !prediction_seed) {
			prediction_seed = pattern::search(_("client.dll"), _("8B 47 40 A3")).add(4).deref().get< std::uintptr_t >();
			prediction_player = pattern::search(_("client.dll"), _("0F 5B C0 89 35")).add(5).deref().get< std::uintptr_t >();
		}

		if (features::prediction::vel_modifier < 1.0f) {
			*reinterpret_cast<bool*> (reinterpret_cast<uintptr_t>(csgo::i::pred) + 0x24) = true;
			*reinterpret_cast<int*> (reinterpret_cast<uintptr_t>(csgo::i::pred) + 0x1C) = 0;
		}

		first_time_pred = csgo::i::pred->m_is_first_time_predicted;
		in_pred = csgo::i::pred->m_in_prediction;

		*reinterpret_cast<int*>(prediction_seed) = ucmd ? ucmd->m_randseed : -1;
		*reinterpret_cast<uintptr_t*>(prediction_player) = reinterpret_cast<uintptr_t>(g::local);

		*reinterpret_cast<uint32_t*>(std::uintptr_t(g::local) + 0x3338) = reinterpret_cast<uintptr_t>(ucmd);
		*reinterpret_cast<ucmd_t*>(std::uintptr_t(g::local) + 0x3288) = *ucmd;

		flags = g::local->flags();
		features::prediction::crouch_amount = g::local->crouch_amount();
		features::prediction::vel = g::local->vel();

		curtime = csgo::i::globals->m_curtime;
		frametime = csgo::i::globals->m_frametime;
		tickcount = csgo::i::globals->m_tickcount;

		csgo::i::globals->m_curtime = predicted_curtime = csgo::ticks2time(g::local->tick_base());
		csgo::i::globals->m_frametime = csgo::i::pred->m_engine_paused ? 0.0f : csgo::i::globals->m_ipt;
		csgo::i::globals->m_tickcount = g::local->tick_base();

		csgo::i::pred->m_is_first_time_predicted = false;
		csgo::i::pred->m_in_prediction = true;

		if (*reinterpret_cast<uint8_t*> (reinterpret_cast<uintptr_t>(ucmd) + 52))
			*reinterpret_cast<uint32_t*>(reinterpret_cast<uintptr_t>(g::local) + 0x31FC) = *reinterpret_cast<uint8_t*> (reinterpret_cast<uintptr_t>(ucmd) + 52);

		*reinterpret_cast<uint32_t*> (reinterpret_cast<uintptr_t>(ucmd) + 48) |= *reinterpret_cast<uint32_t*> (reinterpret_cast<uintptr_t>(g::local) + 0x3334);
		*reinterpret_cast<uint32_t*> (reinterpret_cast<uintptr_t>(ucmd) + 48) &= ~*reinterpret_cast<uint32_t*> (reinterpret_cast<uintptr_t>(g::local) + 0x3330);

		const auto v16 = *reinterpret_cast<uint32_t*> (reinterpret_cast<uintptr_t>(ucmd) + 48);
		const auto v17 = v16 ^ *reinterpret_cast<uint32_t*> (reinterpret_cast<uintptr_t>(g::local) + 0x31F8);
		*reinterpret_cast<uint32_t*> (reinterpret_cast<uintptr_t>(g::local) + 0x31EC) = *reinterpret_cast<uint32_t*> (reinterpret_cast<uintptr_t>(g::local) + 0x31F8);
		*reinterpret_cast<uint32_t*> (reinterpret_cast<uintptr_t>(g::local) + 0x31F8) = v16;
		*reinterpret_cast<uint32_t*> (reinterpret_cast<uintptr_t>(g::local) + 0x31F0) = v16 & v17;
		*reinterpret_cast<uint32_t*> (reinterpret_cast<uintptr_t>(g::local) + 0x31F4) = v17 & ~v16;

		csgo::i::pred->check_moving_ground(g::local, csgo::i::globals->m_frametime);

		csgo::i::pred->set_local_viewangles(ucmd->m_angs);

		if (g::local->physics_run_think(0))
			g::local->pre_think();

		auto& next_think = *reinterpret_cast<uint32_t*> (reinterpret_cast<uintptr_t>(g::local) + 252);

		if (next_think > 0 && next_think <= g::local->tick_base()) {
			next_think = -1;
			g::local->think();
		}

		csgo::i::move_helper->set_host(g::local);

		csgo::i::pred->setup_move(g::local, ucmd, csgo::i::move_helper, movedata);
		csgo::i::move->process_movement(g::local, movedata);

		csgo::i::pred->finish_move(g::local, ucmd, movedata);
	}

	void end(ucmd_t* ucmd) {
		features::prediction::in_prediction = false;

		auto local = csgo::i::ent_list->get< player_t* >(csgo::i::engine->get_local_player());

		if (!csgo::i::engine->is_in_game() || !ucmd || !local || !local->alive())
			return;

		csgo::i::move_helper->set_host(nullptr);

		csgo::i::globals->m_curtime = curtime;
		csgo::i::globals->m_frametime = frametime;
		csgo::i::globals->m_tickcount = tickcount;

		*reinterpret_cast<uint32_t*>(reinterpret_cast<std::uintptr_t>(local) + 0x3338) = 0;
		*reinterpret_cast<int*>(prediction_seed) = -1;
		*reinterpret_cast<int*>(prediction_player) = 0;

		csgo::i::pred->m_is_first_time_predicted = first_time_pred;
		csgo::i::pred->m_in_prediction = in_pred;

		csgo::i::move->reset();
	}
}

float features::prediction::calculate_velocity_modifier(player_t* local, int m_cmd_num, bool pre_orig) {
	float result{};
	int delta{};
	delta = m_cmd_num - backup::packet_end_cmdnum - 1;

	if (!pre_orig)
		delta = m_cmd_num - backup::packet_end_cmdnum;

	if (delta < 0 || backup::velocity_modifier == 1.f) {
		result = local->velocity_modifier();
	}
	else if (delta) {
		const float peenhis = ((csgo::i::globals->m_frametime * .40000001f) * delta)
			+ backup::velocity_modifier;

		if (peenhis <= 1.f) {
			if (peenhis >= 0.f) {
				result = peenhis;
			}
			else
				result = 0.f;
		}
		else {
			result = 1.f;
		}
	}
	else {
		result = backup::velocity_modifier;
	}
	return result;
}

int features::prediction::shift_tickbase() {
	return 0;
}

float features::prediction::curtime() {
	return predicted_curtime;
}

void features::prediction::update(int stage) {
	if (!csgo::i::engine->is_in_game() || !csgo::i::engine->is_connected() || stage != 4)
		return;
}

void features::prediction::fix_viewmodel(bool store) {
	//if ( !g::local || !g::local->alive( ) || g::local->viewmodel_handle( ) == 0xFFFFFFFF )
	//	return;
	//
	//const auto viewmodel = csgo::i::ent_list->get_by_handle< weapon_t* >( g::local->viewmodel_handle( ) );
	//
	//if ( !viewmodel )
	//	return;
	//
	//static float viewmodel_anim_time = 0.0f;
	//static float viewmodel_cycle = 0.0f;
	//
	//if ( store ) {
	//	viewmodel_anim_time = viewmodel->anim_time( );
	//	viewmodel_cycle = viewmodel->cycle( );
	//}
	//else {
	//	viewmodel->anim_time( ) = viewmodel_anim_time;
	//	viewmodel->cycle( ) = viewmodel_cycle;
	//}

	return;
}

std::array< features::prediction::netvars_t, 150> cmd_netvars{};

bool features::prediction::fix_netvars(int cmd, bool store) {
	if (!g::local || !g::local->alive())
		return false;

	auto& cur_rec = cmd_netvars[cmd % cmd_netvars.size()];

	if (store)
		return cur_rec.store(g::local);

	if (g::local->tick_base() != cur_rec.m_tick_base)
		return false;

	return cur_rec.restore(g::local);
}

void features::prediction::run(const std::function< void() >& fn) {
	prediction_util::start(g::ucmd);
	fn();
	prediction_util::end(g::ucmd);
}