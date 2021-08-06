#pragma once
#include "hooks.hpp"

namespace hooks {
	void __fastcall lock_cursor( void * ecx , void * edx );

	namespace original {
		extern decltype( &hooks::lock_cursor ) lock_cursor;
	}
}