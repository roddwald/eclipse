#pragma once
#include "hooks.hpp"

namespace hooks {
	void __fastcall override_view( void * ecx , void * edx , viewsetup_t * setup );
	namespace original {
		extern decltype( &hooks::override_view ) override_view;
	}
}