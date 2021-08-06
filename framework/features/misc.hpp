#pragma once
#include "../sdk/sdk.hpp"
#include "../globals.hpp"
#include "../features/prediction.hpp"

namespace features {
	namespace misc {
		void bhop(ucmd_t* ucmd);
		void duck_stamina(ucmd_t* ucmd);
		void edgebug(ucmd_t* ucmd, int unpredicted_flags, int post_flags, float unpredicted_velocity);
		void jumpbug(ucmd_t* ucmd, int unpredicted_flags);
		void aspect_ratio();
		void edgebug_detection(ucmd_t* ucmd, float unpredicted_velocity);
		void no_flash();
		void no_smoke();
		void sniper_crosshair();
		void spectator_list();
	}
}