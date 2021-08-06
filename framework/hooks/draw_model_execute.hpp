#pragma once
#include "hooks.hpp"

namespace hooks {
	void __fastcall draw_model_execute( void * ecx , void * edx , void * ctx , void * state , const mdlrender_info_t & info , matrix3x4_t * bone_to_world );

	namespace original {
		extern decltype( &hooks::draw_model_execute ) draw_model_execute;
	}
}