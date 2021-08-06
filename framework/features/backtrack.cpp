#include "backtrack.hpp"

std::array<std::deque<record_t>, 65> features::backtrack::records;

void features::backtrack::update() {
	if (!g::ucmd)
		return;

	for (auto record : records) {
		if (!options::vars["backtrack.enable"].val.b) {
			record.clear();
			return;
		}
	}

	player_t* local = csgo::i::ent_list->get<player_t*>(csgo::i::engine->get_local_player());
	for (int i = 1; i <= csgo::i::globals->m_max_clients; i++) {
		player_t* pl = csgo::i::ent_list->get<player_t*>(i);
		if (!pl
			|| !pl->alive()
			|| pl->dormant()
			|| pl->team() == local->team()
			)
			continue;

		record_t record(g::ucmd->m_tickcount, pl->simtime(), pl->get_hitbox_position(hitbox_head), pl->abs_origin());

		pl->setup_bones(record.matrix, 128, 0x0007FF00, csgo::i::globals->m_curtime);

		records[i].push_front(record);

		while (records[i].size() > options::vars["backtrack.limit"].val.i)
			records[i].pop_back();
	}
}

void features::backtrack::run(ucmd_t* ucmd, player_t* local) {
	if (!options::vars["backtrack.enable"].val.b)
		return;

	player_t* target = nullptr;

	int m_idx = -1;
	float m_best_fov = 180.f;
	for (int i = 1; i <= csgo::i::globals->m_max_clients; i++) {
		player_t* pl = csgo::i::ent_list->get<player_t*>(i);
		if (!pl
			|| !pl->alive()
			|| pl->dormant()
			|| pl->team() == local->team()
			) {
			if (records[i].empty())
				records[i].clear();
			continue;
		}

		auto m_ang = csgo::calc_angle(local->eyes(), pl->eyes());
		float m_fov = csgo::calc_fov(ucmd->m_angs + g::local->aim_punch() * 2.0f, m_ang);
		if (m_fov < m_best_fov) {
			m_best_fov = m_fov;
			m_idx = i;
			target = pl;
		}
	}

	int m_best_record = -1;
	if (m_idx != -1) {
		float m_best_fov = 180.f;
		for (auto i = 0; i < records[m_idx].size(); i++) {
			auto m_ang = csgo::calc_angle(local->eyes(), records[m_idx][i].hitbox);
			float m_fov = csgo::calc_fov(ucmd->m_angs + g::local->aim_punch() * 2.0f, m_ang);
			if (m_fov < m_best_fov) {
				m_best_fov = m_fov;
				m_best_record = i;
			}
		}
	}

	if (m_best_record != -1) {
		if (ucmd->m_buttons & in_attack || ucmd->m_buttons & in_attack2) {
			ucmd->m_tickcount = csgo::time2ticks(records[m_idx][m_best_record].simtime);
		}
	}
}