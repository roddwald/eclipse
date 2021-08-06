#include "list_leaves_in_box.hpp"

decltype( &hooks::list_leaves_in_box ) hooks::original::list_leaves_in_box = nullptr;

int __fastcall hooks::list_leaves_in_box( void* ecx, void* edx, const vec3_t& mins, const vec3_t& maxs, unsigned short* list, int list_max ) {
	if ( *( uint32_t* ) _ReturnAddress( ) != 0x14244489 )
		return original::list_leaves_in_box( ecx, edx, mins, maxs, list, list_max );

	struct render_info_t {
		void* m_renderable;
		void* m_alpha_property;
		int m_enum_count;
		int m_render_frame;
		unsigned short m_first_shadow;
		unsigned short m_leaf_list;
		short m_area;
		uint16_t m_flags;
		uint16_t m_flags2;
		vec3_t m_bloated_abs_mins;
		vec3_t m_bloated_abs_maxs;
		vec3_t m_abs_mins;
		vec3_t m_abs_maxs;
		PAD( 4 );
	};

	auto info = *( render_info_t** ) ( ( uintptr_t ) _AddressOfReturnAddress( ) + 0x14 );
	if ( !info || !info->m_renderable )
		return original::list_leaves_in_box( ecx, edx, mins, maxs, list, list_max );

	/* sesame */
	auto get_client_unknown = [] ( void* renderable ) {
		typedef void* ( __thiscall* o_fn )( void* );
		return ( *( o_fn** ) renderable )[0]( renderable );
	};

	auto get_base_entity = [] ( void* c_unk ) {
		typedef player_t* ( __thiscall* o_fn )( void* );
		return ( *( o_fn** ) c_unk )[7]( c_unk );
	};

	auto base_entity = get_base_entity( get_client_unknown( info->m_renderable ) );
	if ( !base_entity || !base_entity->is_player( ) )
		return original::list_leaves_in_box( ecx, edx, mins, maxs, list, list_max );

	info->m_flags &= ~0x100;
	info->m_flags2 |= 0xC0;

	static const vec3_t map_min = vec3_t( MIN_COORD_FLOAT, MIN_COORD_FLOAT, MIN_COORD_FLOAT );
	static const vec3_t map_max = vec3_t( MAX_COORD_FLOAT, MAX_COORD_FLOAT, MAX_COORD_FLOAT );

	original::list_leaves_in_box( ecx, edx, map_min, map_max, list, list_max );
}