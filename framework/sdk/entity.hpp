#pragma once
#include "netvar.hpp"
#include "vec3.hpp"
#include "../utils/vfunc.hpp"

struct client_class_t;

class entity_t {
public:
	OFFSET( std::uint32_t, idx, 0x64 );
	OFFSET( bool, dormant, 0xED );
	POFFSET( void*, renderable, 0x4 );
	POFFSET( void*, networkable, 0x8 );
	NETVAR( vec3_t, origin, "DT_BaseEntity->m_vecOrigin" );
	NETVAR( std::uint32_t, team, "DT_BaseEntity->m_iTeamNum" );
	NETVAR( std::uint32_t, highlight_r, "DT_BaseAnimating->m_nHighlightColorR" );
	NETVAR( std::uint32_t, highlight_g, "DT_BaseAnimating->m_nHighlightColorG" );
	NETVAR( std::uint32_t, highlight_b, "DT_BaseAnimating->m_nHighlightColorB" );
	NETVAR( std::uint32_t, model_scale, "DT_BaseAnimating->m_flModelScale" );

	void draw( ) {
		using drawmodel_fn = int( __thiscall* )( void*, int, std::uint8_t );
		vfunc< drawmodel_fn >( renderable( ), 9 )( renderable( ), 1, 255 );
	}

	client_class_t* client_class( ) {
		using fn = client_class_t*( __thiscall* )( void* );
		return vfunc< fn >( networkable( ), 2 )( networkable( ) );
	}
};