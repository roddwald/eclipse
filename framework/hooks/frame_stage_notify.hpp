#pragma once
#include "hooks.hpp"

namespace hooks {
	void __fastcall frame_stage_notify( void * ecx , void * edx , int stage );
	namespace original {
		extern decltype( &hooks::frame_stage_notify ) frame_stage_notify;
	}
}