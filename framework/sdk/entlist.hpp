#pragma once
#include "../utils/vfunc.hpp"
#include "../utils/padding.hpp"

class c_entlist {
public:
	virtual void padfunc1( ) = 0;
	virtual void padfunc2( ) = 0;
	virtual void padfunc3( ) = 0;
	virtual void padfunc4( ) = 0;
	virtual void padfunc5( ) = 0;
	virtual int	num_of_entities( bool include_non_networkable ) = 0;
	virtual int	get_highest_entity_index( void ) = 0;
	virtual void set_max_entities( int maxents ) = 0;
	virtual int	get_max_entities( ) = 0;

	template < typename t >
	t get( int i ) {
		using getcliententity_fn = t( __thiscall* )( void*, int );
		return vfunc< getcliententity_fn >( this, 3 )( this, i );
	}

	template < typename t >
	t get_by_handle( std::uint32_t h ) {
		using getcliententityfromhandle_fn = t( __thiscall* )( void*, std::uint32_t );
		return vfunc< getcliententityfromhandle_fn >( this, 4 )( this, h );
	}
};