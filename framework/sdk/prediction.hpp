#pragma once
#include "../utils/vfunc.hpp"
#include "vec3.hpp"
#include "ucmd.hpp"

class c_move_helper {
public:
	virtual void pad00( ) = 0;
	virtual void set_host( void* host ) = 0;
	virtual void pad01( ) = 0;
	virtual void pad02( ) = 0;
	virtual void process_impacts( ) = 0;
};

class player_t;

class c_prediction {
public:
	void get_local_viewangles( vec3_t& angle ) {
		using fn = void( __thiscall* )( void*, vec3_t& );
		vfunc< fn >( this, 12 )( this, angle );
	}

	void set_local_viewangles( vec3_t& angle ) {
		using fn = void( __thiscall* )( void*, vec3_t& );
		vfunc< fn >( this, 13 )( this, angle );
	}

	void update( int start_frame, bool valid_frame, int inc_ack, int out_cmd ) {
		using fn = void( __thiscall* )( void*, int, bool, int, int );
		vfunc< fn >( this, 3 )( this, start_frame, valid_frame, inc_ack, out_cmd );
	}

	void check_moving_ground( player_t* player, double frametime ) {
		using fn = void( __thiscall* )( void*, player_t*, double );
		vfunc< fn >( this, 18 )( this, player, frametime );
	}

	void setup_move( player_t* player, ucmd_t* ucmd, c_move_helper* helper, void* movedata ) {
		using fn = void( __thiscall* )( void*, player_t*, ucmd_t*, c_move_helper*, void* );
		vfunc< fn >( this, 20 )( this, player, ucmd, helper, movedata );
	}

	void finish_move( player_t* player, ucmd_t* ucmd, void* movedata ) {
		using fn = void( __thiscall* )( void*, player_t*, ucmd_t*, void* );
		vfunc< fn >( this, 21 )( this, player, ucmd, movedata );
	}

	PAD( 8 );
	bool m_in_prediction;
	PAD( 1 );
	bool m_engine_paused;
	PAD( 13 );
	bool m_is_first_time_predicted;
};

class c_movement {
public:
public:
	virtual ~c_movement( void ) = 0;
	virtual void process_movement( void* player, void* move ) = 0;
	virtual void reset( void ) = 0;
	virtual void start_track_prediction_errors( void* player ) = 0;
	virtual void finish_track_prediction_errors( void* player ) = 0;
	virtual void diff_print( char const* fmt, ... ) = 0;
	virtual vec3_t const& get_player_mins( bool ducked ) const = 0;
	virtual vec3_t const& get_player_maxs( bool ducked ) const = 0;
	virtual vec3_t const& get_player_view_offset( bool ducked ) const = 0;
	virtual bool is_moving_player_stuck( void ) const = 0;
	virtual void* get_moving_player( void ) const = 0;
	virtual void unblock_pusher( void* player, void* pusher ) = 0;
	virtual void setup_movement_bounds( void* move ) = 0;
};