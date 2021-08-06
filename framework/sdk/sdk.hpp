#pragma once
#include <windows.h>
#include <d3d9.h>
#include <d3dx9.h>
#include <functional>
#include <thread>
#include <chrono>

#include "../utils/utils.hpp"

#include "engine.hpp"
#include "surface.hpp"
#include "client.hpp"
#include "netvar.hpp"
#include "entlist.hpp"
#include "mat_system.hpp"
#include "mdl_info.hpp"
#include "mdl_render.hpp"
#include "render_view.hpp"
#include "ucmd.hpp"
#include "player.hpp"
#include "globals.hpp"
#include "trace.hpp"
#include "physics.hpp"
#include "mem_alloc.hpp"
#include "mdl_cache.hpp"
#include "prediction.hpp"
#include "localize.hpp"
#include "input.hpp"
#include "events.hpp"
#include "glow.hpp"
#include "cvar.hpp"
#include "panel.hpp"
#include "input_system.hpp"

#include "../utils/draw.hpp"
#include "../utils/xor.hpp"
#include "../utils/fnv1a.hpp"
#include "hud_chat.hpp"

// all game modules listed under here
#define client_dll _( "client.dll" )
#define engine_dll _( "engine.dll" )
#define localize_dll _( "localize.dll" )
#define vphysics_dll _( "vphysics.dll" )
#define vgui2_dll _( "vgui2.dll" )
#define surface_dll _( "vguimatsurface.dll" )
#define tier0_dll _( "tier0.dll" )
#define localize_dll _( "localize.dll" )
#define vstdlib_dll _( "vstdlib.dll" )
#define shaderapidx9_dll _( "shaderapidx9.dll" )
#define materialsystem_dll _( "materialsystem.dll" )
#define inputsystem_dll _( "inputsystem.dll" )

#pragma comment( lib, "minhook.lib" )

class viewsetup_t {
public:
	int			x;
	int			m_unscaled_x;
	int			y;
	int			m_unscaled_y;
	int			m_width;
	int			m_unscaled_width;
	int			m_height;
	int			m_unscaled_height;
	std::byte	pad0[0x90];
	float		m_fov;
	float		m_viewmodel_fov;
	vec3_t		m_origin;
	vec3_t		m_ang_view;
	std::byte	pad1[0x78];
	int			m_edge_blur;
};

class timer_t {
public:
	timer_t( bool start = false ) {
		if ( start )
			reset( );
	}

	void reset( ) {
		m_time_point = std::chrono::high_resolution_clock::now( );
	}

	template <class C = std::chrono::milliseconds>
	long long elapsed( ) const {
		return std::chrono::duration_cast< C >( std::chrono::high_resolution_clock::now( ) - m_time_point ).count( );
	}

private:
	std::chrono::high_resolution_clock::time_point m_time_point = { };
};

enum client_frame_stage_t {
	frame_undefined = -1,
	frame_start,
	frame_net_update_start,
	frame_net_update_postdataupdate_start,
	frame_net_update_postdataupdate_end,
	frame_net_update_end,
	frame_render_start,
	frame_render_end
};

namespace csgo {
	constexpr auto pi = 3.14159265358979f;

	namespace i {
		extern c_surface* surface;
		extern c_engine* engine;
		extern c_client* client;
		extern c_entlist* ent_list;
		extern c_panel* panel;
		extern c_matsys* mat_sys;
		extern c_mdlinfo* mdl_info;
		extern c_mdlrender* mdl_render;
		extern c_renderview* render_view;
		extern c_globals* globals;
		extern c_phys* phys;
		extern c_engine_trace* trace;
		extern c_localize* localize;
		extern c_glow_obj_mgr* glow_obj_mgr;
		extern c_clientstate* client_state;
		extern c_mem_alloc* mem_alloc;
		extern c_prediction* pred;
		extern c_move_helper* move_helper;
		extern c_movement* move;
		extern mdl_cache_t* mdl_cache;
		extern c_input* input;
		extern c_convar* cvar;
		extern c_hud_chat* hud_chat;
		extern c_game_event_mgr* events;
		extern IDirect3DDevice9* dev;
		extern c_input_system* input_sys;
	}

	namespace render {
		bool screen_transform( vec3_t& screen, vec3_t& origin );
		bool world_to_screen( vec3_t& screen, vec3_t& origin );
	}

	namespace util {
		void trace_line( const vec3_t& start, const vec3_t& end, std::uint32_t mask, const entity_t* ignore, trace_t* ptr );
		void clip_trace_to_players( const vec3_t& start, const vec3_t& end, std::uint32_t mask, trace_filter_t* filter, trace_t* trace_ptr );
	}

	constexpr int time2ticks( float t ) {
		return static_cast< int >( t / i::globals->m_ipt + 0.5f );
	}

	constexpr float ticks2time( int t ) {
		return static_cast< float >( t ) * i::globals->m_ipt;
	}

	float normalize( float ang );
	void clamp( vec3_t& ang );
	float rad2deg( float rad );
	float deg2rad( float deg );
	void sin_cos( float radians, float* sine, float* cosine );
	vec3_t calc_angle( const vec3_t& from, const vec3_t& to );
	vec3_t calc_angle( const vec3_t& from, const vec3_t& to, const vec3_t& ang );
	vec3_t vec_angle( vec3_t vec );
	vec3_t angle_vec( vec3_t angle );

	void for_each_player( std::function< void( player_t* ) > fn );
	void for_each_entity( std::function<void( player_t* )> fn );
	void util_tracehull( const vec3_t& start, const vec3_t& end, const vec3_t& mins, const vec3_t& maxs, unsigned int mask, const void* ignore, trace_t* tr );
	bool line_goes_through_smoke( const vec3_t& start, const vec3_t& end, const bool bloat );
	void util_traceline( const vec3_t& start, const vec3_t& end, unsigned int mask, const void* ignore, trace_t* tr );
	void rotate_movement( ucmd_t* ucmd );
	void fix_movement( ucmd_t* ucmd );

	std::uintptr_t* find_hud_element( const char* name );

	__forceinline float calc_fov( const vec3_t& src, const vec3_t& dst ) {
		vec3_t ang = angle_vec( src ), aim = angle_vec( dst );
		return rad2deg( acosf( aim.dot_product( ang ) / aim.length_sqr( ) ) );
	}

	template < typename t >
	t create_interface( const char* module, const char* iname );
	template<class T>
	static T* find_hud_elem_t(const char* name);
	template<typename... args_t>
	inline void console_color_printf(const color& clr, const char* msg, args_t... args) {
		struct {
			uint8_t r, g, b, a;
		} s_clr;

		s_clr = { static_cast <uint8_t> (clr.r * 255.0f), static_cast <uint8_t> (clr.g * 255.0f), static_cast <uint8_t> (clr.b * 255.0f), static_cast <uint8_t> (clr.a * 255.0f) };

		static auto import = (void(*)(const decltype(s_clr)&, const char*, ...))GetProcAddress(GetModuleHandleA("tier0.dll"), "?ConColorMsg@@YAXABVColor@@PBDZZ");
		return import(s_clr, msg, args...);
	}

	void force_full_update( );

	bool init( );
}