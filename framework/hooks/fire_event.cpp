#include "fire_event.hpp"

decltype( &hooks::fire_event ) hooks::original::fire_event = nullptr;

bool __fastcall hooks::fire_event( void* ecx, void* edx ) {
	auto local = csgo::i::ent_list->get< player_t* >( csgo::i::engine->get_local_player( ) );

	if ( !local->valid( ) )
		return original::fire_event( ecx, edx );

	struct event_t {
	public:
		PAD( 4 );
		float delay;
		PAD( 48 );
		event_t* next;
	};

	auto ei = *( event_t** ) ( std::uintptr_t( csgo::i::client_state ) + 0x4E6C );

	event_t* next = nullptr;
	if ( !ei )
		return original::fire_event( ecx, edx );

	do {
		next = ei->next;
		ei->delay = 0.f;
		ei = next;
	} while ( next );

	return original::fire_event( ecx, edx );
}