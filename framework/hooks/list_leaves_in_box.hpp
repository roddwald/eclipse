#pragma once
#include "hooks.hpp"

namespace hooks {
	int __fastcall list_leaves_in_box( void* ecx, void* edx, const vec3_t& mins, const vec3_t& maxs, unsigned short* list, int list_max );

	namespace original {
		extern decltype( &hooks::list_leaves_in_box ) list_leaves_in_box;
	}
}