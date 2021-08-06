#include "get_bool.hpp"

decltype( &hooks::get_bool ) hooks::original::get_bool = nullptr;

bool __fastcall hooks::get_bool( void* ecx, void* edx ) {
	static auto cam_think = pattern::search( client_dll, _( "85 C0 75 30 38 86" ) ).get< std::uintptr_t >( );

	if ( !ecx )
		return false;

	auto local = csgo::i::ent_list->get< player_t* >( csgo::i::engine->get_local_player( ) );

	if ( std::uintptr_t( _ReturnAddress( ) ) == cam_think )
		return true;

	return original::get_bool( ecx, edx );
}