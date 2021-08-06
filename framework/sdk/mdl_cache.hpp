#pragma once
#include "../utils/vfunc.hpp"

class mdl_cache_t {
public:
	void begin_lock( ) {
		using fn = void( __thiscall* )( void* );
		vfunc< fn >( this, 33 )( this );
	}

	void end_lock( ) {
		using fn = void( __thiscall* )( void* );
		vfunc< fn >( this, 34 )( this );
	}
};

class c_mdl_cache_critical_section {
public:
	c_mdl_cache_critical_section( );
	~c_mdl_cache_critical_section( );
};

#define MDLCACHE_CRITICAL_SECTION( ) c_mdl_cache_critical_section cache_critical_section()