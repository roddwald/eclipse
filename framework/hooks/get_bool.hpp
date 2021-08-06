#pragma once
#include "hooks.hpp"

namespace hooks {
	bool __fastcall get_bool( void * ecx , void * edx );

	namespace original {
		extern decltype( &hooks::get_bool ) get_bool;
	}
}