#pragma once
#include "hooks.hpp"

namespace hooks {
	long __fastcall end_scene( void * ecx , void * edx , IDirect3DDevice9 * device );

	namespace original {
		extern decltype( &hooks::end_scene ) end_scene;
	}
}