#include "get_eye_angles.hpp"

decltype( &hooks::get_eye_angles ) hooks::original::get_eye_angles = nullptr;

vec3_t * __fastcall hooks::get_eye_angles( void * ecx , void * edx ) {
	auto local = csgo::i::ent_list->get< player_t * >( csgo::i::engine->get_local_player( ) );

	if ( ecx != local )
		return original::get_eye_angles( ecx , edx );

	static auto ret_to_thirdperson_pitch = pattern::search( client_dll , _( "8B CE F3 0F 10 00 8B 06 F3 0F 11 45 ? FF 90 ? ? ? ? F3 0F 10 55 ?" ) ).get< std::uintptr_t >( );
	static auto ret_to_thirdperson_yaw = pattern::search( client_dll , _( "F3 0F 10 55 ? 51 8B 8E ? ? ? ?" ) ).get< std::uintptr_t >( );

	if ( std::uintptr_t( _ReturnAddress( ) ) == ret_to_thirdperson_pitch
		|| std::uintptr_t( _ReturnAddress( ) ) == ret_to_thirdperson_yaw )
		return g::ucmd ? &g::ucmd->m_angs : &local->angles( );

	return original::get_eye_angles( ecx , edx );
}