#pragma once
#include "hooks.hpp"

namespace hooks {
	bool __fastcall fire_event( void * ecx , void * edx );

	namespace original {
		extern decltype( &hooks::fire_event ) fire_event;
	}
}