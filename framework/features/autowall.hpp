#pragma once
#include "../sdk/sdk.hpp"
#include "../sdk/vec3.hpp"
#include "../sdk/trace.hpp"

struct bullet_data_t {
	bullet_data_t( const vec3_t& eye_pos )
		: src( eye_pos ) {
	}

	vec3_t src;
	trace_t enter_trace;
	vec3_t direction;
	trace_filter_t filter;
	float trace_length;
	float trace_length_remaining;
	float current_damage;
	int penetrate_count;
};

namespace autowall {
	void trace_line( vec3_t& start, vec3_t& end, unsigned int mask, player_t* ignore, trace_t* ptr );
	void clip_trace_to_players( vec3_t& start, vec3_t end, unsigned int mask, trace_filter_t* filter, trace_t* trace );
	void get_bullet_type_parameters( float& max_range, float& max_distance );
	bool is_breakable_entity( player_t* entity );
	void scale_damage( trace_t& enter_trace, weapon_info_t* weapon_data, float& current_damage );
	bool trace_to_exit( trace_t& enter_trace, trace_t& exit_trace, vec3_t start_position, vec3_t direction );
	bool handle_bullet_penetration( weapon_info_t* weapon_data, trace_t& enter_trace, vec3_t& eye_position, vec3_t direction, int& possible_hits_remaining, float& current_damage, float penetration_power, bool sv_penetration_type, float ff_damage_reduction_bullets, float ff_damage_bullet_penetration );
	bool fire_bullet( weapon_t* weapon, vec3_t& direction, float& current_damage );
	float calculate_damage( const vec3_t& point );
}