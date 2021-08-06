#pragma once
#include "hooks.hpp"

namespace hooks {
	bool __fastcall create_move( void * ecx , void * edx , float sampletime , ucmd_t * ucmd );

	namespace original {
		extern decltype( &hooks::create_move ) create_move;
	}
}