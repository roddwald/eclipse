#include "override_view.hpp"

#define CAM_HULL_OFFSET 16.0f
#define CAM_HULL_MIN vec3_t( -CAM_HULL_OFFSET , -CAM_HULL_OFFSET , -CAM_HULL_OFFSET )
#define CAM_HULL_MAX vec3_t( CAM_HULL_OFFSET , CAM_HULL_OFFSET , CAM_HULL_OFFSET )

decltype( &hooks::override_view ) hooks::original::override_view = nullptr;

void __fastcall hooks::override_view( void* ecx, void* edx, viewsetup_t* setup ) {
	auto get_ideal_dist = [&] ( float ideal_distance ) {
		vec3_t inverse;
		csgo::i::engine->get_viewangles( inverse );

		inverse.x *= -1.0f, inverse.y += 180.0f;

		vec3_t direction = csgo::angle_vec( inverse );

		ray_t ray;
		ray.init( g::local->eyes( ), g::local->eyes( ) + ( direction * ideal_distance ), CAM_HULL_MIN, CAM_HULL_MAX );

		trace_t trace;
		trace_filter_t filter( g::local );

		csgo::i::trace->trace_ray(
			ray,
			mask_solid & ~contents_monster,
			&filter,
			&trace
		);

		return ( trace.m_endpos - trace.m_startpos ).length( );
	};

	if ( g::local->valid( ) && GetKeyState(options::vars["misc.thirdperson"].val.i) ) {
		vec3_t ang;
		csgo::i::engine->get_viewangles( ang );
		csgo::i::input->m_camera_in_thirdperson = true;
		csgo::i::input->m_camera_offset = vec3_t( ang.x, ang.y, get_ideal_dist( 150.f ) );
	}
	else {
		csgo::i::input->m_camera_in_thirdperson = false;
	}

	original::override_view( ecx, edx, setup );
}