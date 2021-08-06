#pragma once
#include "hooks.hpp"

namespace hooks {
	long __fastcall reset( void * ecx , void * edx , IDirect3DDevice9 * device , D3DPRESENT_PARAMETERS * presentation_params );
	namespace original {
		extern decltype( &hooks::reset ) reset;
	}
}