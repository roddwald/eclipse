#pragma once
#include "../sdk/sdk.hpp"
#include "../globals.hpp"

struct record_t {
	record_t() = default;
	record_t(int tickcount, float simtime, vec3_t hitbox, vec3_t origin) {
		this->tickcount = tickcount;
		this->simtime = simtime;
		this->hitbox = hitbox;
		this->origin = origin;
	}

	int tickcount;
	float simtime;
	vec3_t hitbox;
	vec3_t origin;

	matrix3x4_t matrix[128];
};

namespace features {
	namespace backtrack {
		extern std::array<std::deque<record_t>, 65> records;

		inline std::deque<record_t>& get_records(int i) {
			return records[i];
		}

		void update();

		void run(ucmd_t* ucmd, player_t* local);
	}
}