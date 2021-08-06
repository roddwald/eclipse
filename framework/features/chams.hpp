#pragma once
#include "../sdk/sdk.hpp"
#include "../globals.hpp"
#include "../menu/options.hpp"

namespace features {
	namespace chams {
		void run(void* ecx, void* edx, void* ctx, void* state, const mdlrender_info_t& info, matrix3x4_t* bone_to_world);
	}

	namespace glow {
		void run();
	}
}