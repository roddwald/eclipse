#include "lock_cursor.hpp"
#include "../menu/menu.hpp"

decltype( &hooks::lock_cursor ) hooks::original::lock_cursor = nullptr;

void __fastcall hooks::lock_cursor( void* ecx, void* edx ) {
	if ( menu::opened ) {
		csgo::i::surface->unlock_cursor( );
		return;
	}

	original::lock_cursor( ecx, edx );
}