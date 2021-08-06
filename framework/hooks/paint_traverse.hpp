#pragma once
#include "hooks.hpp"

namespace hooks {
	void __fastcall paint_traverse( void * ecx , void * edx , int panel , bool force_repaint , bool allow_force );

	namespace original {
		extern decltype( &hooks::paint_traverse ) paint_traverse;
	}
}