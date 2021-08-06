#pragma once
#include "../utils/vfunc.hpp"
#include "../utils/padding.hpp"
#include "vec3.hpp"
#include "ucmd.hpp"

class c_input {
public:
	void* pvftable; //0x00
	PAD( 0x8 );
	bool m_track_ir_available; //0x04
	bool m_mouse_initialized; //0x05
	bool m_mouse_active; //0x06
	bool m_joystick_advanced_init; //0x07
	PAD( 0x2C ); //0x08
	void* m_keys; //0x34
	PAD( 0x64 ); //0x38
	int pad_0x41;
	int pad_0x42;
	bool m_camera_intercepting_mouse; //0x9C
	bool m_camera_in_thirdperson; //0x9D
	bool m_camera_moving_with_mouse; //0x9E
	vec3_t m_camera_offset; //0xA0
	bool m_camera_distance_move; //0xAC
	int m_camera_old_x; //0xB0
	int m_camera_old_y; //0xB4
	int m_camera_x; //0xB8
	int m_camera_y; //0xBC
	bool m_camera_is_orthographic; //0xC0
	vec3_t m_prev_viewangles; //0xC4
	vec3_t m_prev_viewangles_tilt; //0xD0
	float m_last_forward_move; //0xDC
	int m_clear_input_state; //0xE0

	ucmd_t* get_usercmd( int slot, int sequence_number ) {
		auto _this = ( char* ) this;
		auto unk = (int) this;

		if ( slot != -1 )
			unk = (int)&_this [ 0xDC * slot ];

		auto ret = ( ucmd_t* ) ( *( std::uintptr_t *)( std::uintptr_t( unk ) + 0xF4 ) + 100 * ( sequence_number % 150 ) );

		if ( ret->m_cmdnum != sequence_number )
			ret = nullptr;

		return ret;
	}

	ucmd_t* get_usercmds( int slot ) {
		auto _this = ( char* ) this;
		auto unk = ( int ) this;

		if ( slot != -1 )
			unk = ( int ) &_this [ 0xDC * slot ];

		return ( ucmd_t* ) ( *( std::uintptr_t* )( std::uintptr_t( unk ) + 0xF4 ) );
	}
};