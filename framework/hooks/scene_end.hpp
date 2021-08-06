#pragma once
#include "hooks.hpp"

namespace hooks {
	void __fastcall scene_end( void * ecx , void * edx );

	namespace original {
		extern decltype( &hooks::scene_end ) scene_end;
	}
}