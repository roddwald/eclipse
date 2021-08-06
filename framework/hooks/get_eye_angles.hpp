#pragma once
#include "hooks.hpp"

namespace hooks {
	vec3_t * __fastcall get_eye_angles( void * ecx , void * edx );

	namespace original {
		extern decltype( &hooks::get_eye_angles ) get_eye_angles;
	}
}