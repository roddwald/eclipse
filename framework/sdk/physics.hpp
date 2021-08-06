#pragma once
#include "../utils/vfunc.hpp"
#include "../utils/padding.hpp"

struct surfacephysicsparams_t {
	PAD( 20 );
};

struct surfaceaudioparams_t {
	PAD( 36 );
};

struct surfacesoundnames_t {
	PAD( 24 );
};

struct surfacegameprops_t {
	PAD( 8 );
	float m_penetration_modifier;
	float m_damage_modifier;
	std::uint16_t m_material;
	std::uint8_t m_climbable;
	PAD( 4 );
};

struct surfacedata_t {
	surfacephysicsparams_t m_physics;
	surfaceaudioparams_t m_audio;
	surfacesoundnames_t m_sounds;
	surfacegameprops_t m_game;
};

class c_phys {
public:
	surfacedata_t* surface( int idx ) {
		using fn = surfacedata_t*( __thiscall* )( void*, int );
		return vfunc< fn >( this, 5 )( this, idx );
	}
};