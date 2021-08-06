#pragma once
#include "../utils/vfunc.hpp"
#include "../utils/padding.hpp"
#include "matrix3x4.hpp"

struct mstudiobone_t {
	int m_sznameindex;

	char* const get_name( ) const {
		return reinterpret_cast< char* >( const_cast< mstudiobone_t* >( this ) ) + m_sznameindex;
	}

	int m_parent;
	int m_bonecontroller[ 6 ];

	vec3_t m_pos;
	float m_quat[ 4 ];
	vec3_t m_rot;
	vec3_t m_posscale;
	vec3_t m_rotscale;

	matrix3x4_t	m_pose_to_bone;
	float m_quaternion_alignment[ 4 ];
	int m_flags;
	int m_proctype;
	int m_procindex;
	mutable int m_physicsbone;

	void* get_procedure( ) const {
		if ( !this->m_procindex )
			return nullptr;
		else
			return ( void* ) ( ( ( std::uint8_t* ) this ) + this->m_procindex );
	};

	int m_surfacepropidx;

	char* const get_surface_props( void ) const {
		return ( ( char* ) this ) + m_surfacepropidx;
	}

	int m_contents;
	PAD( 32 );
};

struct mstudiobbox_t {
	int m_bone;
	int m_group;
	vec3_t m_bbmin;
	vec3_t m_bbmax;
	int m_hitbox_name_index;
	PAD( 12 );
	float m_radius;
	PAD( 16 );

	const char* get_name( ) {
		if ( !this->m_hitbox_name_index )
			return nullptr;

		return reinterpret_cast< const char* >( reinterpret_cast< std::uint8_t* >( const_cast< mstudiobbox_t* >( this ) ) + this->m_hitbox_name_index );
	}
};

struct mstudiohitboxset_t {
	int m_name_index;
	int m_num_hitboxes;
	int m_hitbox_index;

	const char* get_name( void ) {
		if ( !this->m_name_index )
			return nullptr;

		return reinterpret_cast< const char* >( reinterpret_cast< std::uint8_t* >( const_cast< mstudiohitboxset_t* >( this ) ) + this->m_name_index );
	}

	mstudiobbox_t* hitbox( int i ) const {
		if ( i > this->m_num_hitboxes )
			return nullptr;

		return reinterpret_cast< mstudiobbox_t* >( reinterpret_cast< std::uint8_t* >( const_cast< mstudiohitboxset_t* >( this ) ) + this->m_hitbox_index ) + i;
	}
};


struct studiohdr_t {
	int m_id;
	int m_version;
	long m_checksum;
	char m_name[ 64 ];
	int m_length;
	vec3_t m_vec_eye_pos;
	vec3_t m_vec_illumination_pos;
	vec3_t m_vec_hull_min;
	vec3_t m_vec_hull_max;
	vec3_t m_vec_min;
	vec3_t m_vec_max;
	int m_flags;
	int m_numbones;
	int m_boneindex;
	int m_numbonecontrollers;
	int m_bonecontrollerindex;
	int m_numhitboxsets;
	int m_hitboxsetindex;
	int m_numlocalanim;
	int m_localanimindex;
	int m_numlocalseq;
	int m_localseqindex;
	int m_activitylistversion;
	int m_eventsindexed;
	int m_numtextures;
	int m_textureindex;

	inline const char* get_name( void ) const {
		return this->m_name;
	}

	mstudiohitboxset_t* hitbox_set( int i ) {
		if ( i > this->m_numhitboxsets )
			return nullptr;

		return reinterpret_cast< mstudiohitboxset_t* > ( reinterpret_cast< std::uint8_t* >( const_cast< studiohdr_t* >( this ) ) + this->m_hitboxsetindex ) + i;
	}

	mstudiobone_t* bone( int i ) {
		if ( i > this->m_numbones )
			return nullptr;

		return reinterpret_cast< mstudiobone_t* > ( reinterpret_cast< std::uint8_t* >( const_cast< studiohdr_t* >( this ) ) + this->m_boneindex ) + i;
	}
};

class c_mdlinfo {
public:
	void* mdl( int idx ) {
		using getmdl_fn = void*( __thiscall* )( void*, int );
		return vfunc< getmdl_fn >( this, 1 )( this, idx );
	}

	std::uint32_t mdl_idx( const char* name ) {
		using getmdlidx_fn = std::uint32_t( __thiscall* )( void*, const char* );
		return vfunc< getmdlidx_fn >( this, 2 )( this, name );
	}

	const char* mdl_name( const void* mdl ) {
		using getmdlname_fn = const char*( __thiscall* )( void*, const void* );
		return vfunc< getmdlname_fn >( this, 3 )( this, mdl );
	}

	studiohdr_t* studio_mdl( void* mdl ) {
		using getstudiomdl_fn = studiohdr_t * ( __thiscall* )( void*, void* );
		return vfunc< getstudiomdl_fn >( this, 32 )( this, mdl );
	}
};