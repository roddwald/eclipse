#pragma once
#include "../utils/vfunc.hpp"
#include "../utils/padding.hpp"
#include "matrix3x4.hpp"

struct mdlrender_info_t {
	vec3_t m_origin;
	vec3_t m_angles;
	PAD( 4 );
	void * m_renderable;
	void * m_model;
	matrix3x4_t * m_model_to_world;
	matrix3x4_t * m_lighting_offset;
	vec3_t * m_lighting_origin;
	int m_flags;
	int m_entity_index;
	int m_skin;
	int m_body;
	int m_hitbox_set;
	std::uint16_t m_instance;
};

struct renderable_info_t {
	void * m_renderable;
	void * m_alpha_properties;
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
	int pad;
};

class c_renderview {
public:
	void set_alpha( int a ) {
		using setblend_fn = void( __thiscall * )( void * , float );
		vfunc< setblend_fn >( this , 4 )( this , static_cast< float >( a ) / 255.0f );
	}

	void set_color( int r , int g , int b ) {
		using colormodulate_fn = void( __thiscall * )( void * , float * );
		float clrf[ ]{ static_cast< float >( r ) / 255.0f, static_cast< float >( g ) / 255.0f, static_cast< float >( b ) / 255.0f };
		vfunc< colormodulate_fn >( this , 6 )( this , clrf );
	}
};