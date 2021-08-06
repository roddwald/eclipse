#include "autowall.hpp"
#include "../globals.hpp"

void autowall::trace_line( vec3_t& start, vec3_t& end, unsigned int mask, player_t* ignore, trace_t* ptr ) {
	ray_t ray;
	ray.init( start, end );
	trace_filter_t filter;
	filter.m_skip = ignore;

	csgo::i::trace->trace_ray( ray, mask, &filter, ptr );
}

void autowall::clip_trace_to_players( vec3_t& start, vec3_t end, unsigned int mask, trace_filter_t* filter, trace_t* trace ) {
	static auto clip_trace_to_players_add = pattern::search( _( "client.dll" ), _( "53 8B DC 83 EC 08 83 E4 F0 83 C4 04 55 8B 6B 04 89 6C 24 04 8B EC 81 EC D8 ? ? ? 0F 57 C9" ) ).get< void* >( );

	__asm {
		mov eax, filter
		lea ecx, trace
		push ecx
		push eax
		push mask
		lea edx, end
		lea ecx, start
		call clip_trace_to_players_add
		add esp, 0xC
	}
}

void autowall::get_bullet_type_parameters( float& max_range, float& max_distance ) {
	max_range = 35.0;
	max_distance = 3000.0;
}

bool autowall::is_breakable_entity( player_t* entity ) {
	auto classname_is = [] ( player_t* entity, const char* class_name ) {
		return !std::strcmp( entity->client_class( )->m_network_name, class_name );
	};

	static auto __rtdynamiccast_fn = pattern::search( _( "client.dll" ), _( "6A 18 68 ? ? ? ? E8 ? ? ? ? 8B 7D 08" ) ).get<void*>( );
	static auto is_breakable_entity_fn = pattern::search( _( "client.dll" ), _( "55 8B EC 51 56 8B F1 85 F6 74 68 83 BE" ) ).get<void*>( );
	static auto multiplayer_phys_rtti_desc = *( uintptr_t* ) ( ( uintptr_t ) is_breakable_entity_fn + 0x50 );
	static auto baseentity_rtti_desc = *( uintptr_t* ) ( ( uintptr_t ) is_breakable_entity_fn + 0x55 );
	static auto breakablewithpropdata_rtti_desc = *( uintptr_t* ) ( ( uintptr_t ) is_breakable_entity_fn + 0xD5 );

	int( __thiscall * **v4 )( player_t* );
	int v5;

	if ( entity && ( *( std::uint32_t* ) ( entity + 256 ) >= 0 || ( *( int( ** )( void ) )( *( std::uint32_t* ) entity + 488 ) )( ) <= 0 ) && *( std::uint8_t* ) ( *( std::uint32_t* ) entity + 640 ) == 2 ) {
		auto v3 = *( std::uint32_t* ) ( *( std::uint32_t* ) entity + 1140 );

		if ( v3 != 17 && v3 != 6 && v3 )
			return false;

		if ( *( std::uint32_t* ) ( *( std::uint32_t* ) entity + 256 ) > 200 )
			return false;

		__asm {
			push 0
			push multiplayer_phys_rtti_desc
			push baseentity_rtti_desc
			push 0
			push entity
			call __rtdynamiccast_fn
			add esp, 20
			mov v4, eax
		}

		if ( v4 ) {
			if ( ( **v4 )( entity ) != 1 )
				return false;

			goto label_18;
		}

		if ( !classname_is( entity, _( "func_breakable" ) ) && !classname_is( entity, _( "func_breakable_surf" ) ) ) {
			if ( ( *( ( int( __thiscall** )( player_t* ) ) * ( std::uint32_t* ) entity + 604 ) )( entity ) & 0x10000 )
				return false;

			goto label_18;
		}

		if ( !classname_is( entity, _( "func_breakable_surf" ) ) || !*( ( uint8_t* ) entity + 2564 ) ) {
		label_18:
			__asm {
				push 0
				push breakablewithpropdata_rtti_desc
				push baseentity_rtti_desc
				push 0
				push entity
				call __rtdynamiccast_fn
				add esp, 20
				mov v5, eax
			}

			if ( v5 && ( ( float( __thiscall* )( uintptr_t ) ) * ( uintptr_t* ) ( *( uintptr_t* ) v5 + 12 ) )( v5 ) <= 0.0f )
				return true;
		}
	}

	return false;
}

void autowall::scale_damage( trace_t& enter_trace, weapon_info_t* weapon_data, float& current_damage ) {
	bool has_heavy_armor = false; /* meh */
	int armor_value = ( ( player_t* ) enter_trace.m_hit_entity )->armor( );
	int hitgroup = enter_trace.m_hitgroup;

	auto is_armored = [&enter_trace] ( )->bool {
		player_t* m_target = ( player_t* ) enter_trace.m_hit_entity;
		switch ( enter_trace.m_hitgroup ) {
		case hitgroup_head:
			return m_target->has_helmet( );
		case hitgroup_generic:
		case hitgroup_chest:
		case hitgroup_stomach:
		case hitgroup_leftarm:
		case hitgroup_rightarm:
			return true;
		default:
			return false;
		}
	};

	switch ( hitgroup ) {
	case hitgroup_head:
		current_damage *= 2.f;
		break;
	case hitgroup_stomach:
		current_damage *= 1.25f;
		break;
	case hitgroup_leftleg:
	case hitgroup_rightleg:
		current_damage *= 0.75f;
		break;
	default:
		break;
	}

	if ( armor_value > 0 && is_armored( ) ) {
		float bonus_value = 1.f, armor_bonus_ratio = 0.5f, armor_ratio = weapon_data->m_armor_ratio / 2.f;

		if ( has_heavy_armor ) {
			armor_bonus_ratio = 0.33f;
			armor_ratio *= 0.5f;
			bonus_value = 0.33f;
		}

		auto dmg = current_damage * armor_ratio;

		if ( has_heavy_armor )
			dmg *= 0.85f;

		if ( ( ( current_damage - ( current_damage * armor_ratio ) ) * ( bonus_value * armor_bonus_ratio ) ) > armor_value )
			dmg = current_damage - ( armor_value / armor_bonus_ratio );

		current_damage = dmg;
	}
}

bool autowall::trace_to_exit( trace_t& enter_trace, trace_t& exit_trace, vec3_t start_position, vec3_t direction ) {
	vec3_t start, end;
	float max_distance = 90.f, ray_extension = 4.f, current_distance = 0;
	int first_contents = 0;

	while ( current_distance <= max_distance ) {
		current_distance += ray_extension;

		start = start_position + direction * current_distance;

		if ( !first_contents )
			first_contents = csgo::i::trace->get_point_contents( start, mask_shot_hull | contents_hitbox, nullptr );
		int point_contents = csgo::i::trace->get_point_contents( start, mask_shot_hull | contents_hitbox, nullptr );

		if ( !( point_contents & mask_shot_hull ) || point_contents & contents_hitbox && point_contents != first_contents ) {
			end = start - ( direction * ray_extension );

			trace_line( start, end, mask_shot_hull | contents_hitbox, nullptr, &exit_trace );

			if ( exit_trace.m_startsolid && exit_trace.m_surface.m_flags & surf_hitbox ) {
				trace_line( start, start_position, mask_shot_hull, ( player_t* ) exit_trace.m_hit_entity, &exit_trace );

				if ( exit_trace.did_hit( ) && !exit_trace.m_startsolid ) {
					start = exit_trace.m_endpos;
					return true;
				}

				continue;
			}

			if ( exit_trace.did_hit( ) && !exit_trace.m_startsolid ) {
				if ( is_breakable_entity( ( player_t* ) enter_trace.m_hit_entity ) && is_breakable_entity( ( player_t* ) exit_trace.m_hit_entity ) )
					return true;

				if ( enter_trace.m_surface.m_flags & surf_nodraw || !( exit_trace.m_surface.m_flags & surf_nodraw ) && ( exit_trace.m_plane.m_normal.dot( direction ) <= 1.f ) ) {
					float mult_amt = exit_trace.m_fraction * 4.f;
					start -= direction * mult_amt;
					return true;
				}

				continue;
			}

			if ( !exit_trace.did_hit( ) || exit_trace.m_startsolid ) {
				if ( enter_trace.did_hit( ) && is_breakable_entity( ( player_t* ) enter_trace.m_hit_entity ) ) {
					exit_trace = enter_trace;
					exit_trace.m_endpos = start + direction;
					return true;
				}

				continue;
			}
		}
	}

	return false;
}

bool autowall::handle_bullet_penetration( weapon_info_t* weapon_data, trace_t& enter_trace, vec3_t& eye_position, vec3_t direction, int& possible_hits_remaining, float& current_damage, float penetration_power, bool sv_penetration_type, float ff_damage_reduction_bullets, float ff_damage_bullet_penetration ) {
	if ( !g::local->valid( ) )
		return false;

	auto data = bullet_data_t( g::local->eyes( ) );
	data.filter = trace_filter_t( );
	data.filter.m_skip = g::local;

	player_t* enter_player = ( player_t* ) enter_trace.m_hit_entity;
	surfacedata_t* enter_surface_data = csgo::i::phys->surface( enter_trace.m_surface.m_surface_props );
	int enter_material = enter_surface_data->m_game.m_material;
	float enter_surf_penetration_modifier = enter_surface_data->m_game.m_penetration_modifier;
	float enter_damage_modifier = enter_surface_data->m_game.m_damage_modifier;

	float thickness, modifier, lost_damage, final_damage_modifier, combined_penetration_modifier;
	bool is_solid_surf = ( ( enter_trace.m_contents >> 3 ) & contents_solid );
	bool is_light_surf = ( ( enter_trace.m_surface.m_flags >> 7 ) & surf_light );

	trace_t exit_trace;
	if ( possible_hits_remaining <= 0
		|| ( !possible_hits_remaining && !is_light_surf && !is_solid_surf && enter_material != char_tex_grate && enter_material != char_tex_glass )
		|| weapon_data->m_penetration <= 0.f
		|| !trace_to_exit( enter_trace, exit_trace, enter_trace.m_endpos, direction )
		&& !( csgo::i::trace->get_point_contents( enter_trace.m_endpos, mask_shot_hull, nullptr ) & mask_shot_hull ) )
		return false;

	surfacedata_t* exit_surface_data = csgo::i::phys->surface( exit_trace.m_surface.m_surface_props );
	int exit_material = exit_surface_data->m_game.m_material;
	float exit_surf_penetration_modifier = exit_surface_data->m_game.m_penetration_modifier;
	float exit_damage_modifier = exit_surface_data->m_game.m_damage_modifier;

	if ( enter_material == char_tex_grate || enter_material == char_tex_glass ) {
		combined_penetration_modifier = 3.f;
		final_damage_modifier = 0.05f;
	}
	else if ( is_solid_surf || is_light_surf ) {
		combined_penetration_modifier = 1.f;
		final_damage_modifier = 0.16f;
	}
	/* shooting through teammates */
	else if ( enter_material == char_tex_flesh && ( g::local->team( ) == enter_player->team( ) && ff_damage_reduction_bullets == 0.f ) ) {
		if ( ff_damage_bullet_penetration == 0.f )
			return false;

		combined_penetration_modifier = ff_damage_bullet_penetration;
		final_damage_modifier = 0.16f;
	}
	else {
		combined_penetration_modifier = ( enter_surf_penetration_modifier + exit_surf_penetration_modifier ) / 2.f;
		final_damage_modifier = 0.16f;
	}

	if ( enter_material == exit_material ) {
		if ( exit_material == char_tex_cardboard || exit_material == char_tex_wood )
			combined_penetration_modifier = 3.f;
		else if ( exit_material == char_tex_plastic )
			combined_penetration_modifier = 2.f;
	}

	thickness = ( exit_trace.m_endpos - enter_trace.m_endpos ).length_sqr( );
	modifier = fmaxf( 1.f / combined_penetration_modifier, 0.f );

	lost_damage = fmaxf(
		( ( modifier * thickness ) / 24.f )
		+ ( ( current_damage * final_damage_modifier )
			+ ( fmaxf( 3.75f / penetration_power, 0.f ) * 3.f * modifier ) ), 0.f );

	if ( lost_damage > current_damage )
		return false;

	if ( lost_damage > 0.f )
		current_damage -= lost_damage;

	if ( current_damage < 1.f )
		return false;

	eye_position = exit_trace.m_endpos;
	--possible_hits_remaining;

	return true;
}

bool autowall::fire_bullet( weapon_t* weapon, vec3_t& direction, float& current_damage ) {
	if ( !g::local->valid( ) || !weapon )
		return false;

	auto data = bullet_data_t( g::local->eyes( ) );
	data.filter = trace_filter_t( );
	data.filter.m_skip = g::local;

	bool sv_penetration_type;
	float current_distance = 0.f,
		penetration_power,
		penetration_distance,
		max_range,
		ff_damage_reduction_bullets,
		ff_damage_bullet_penetration,
		ray_extension = 40.f;

	vec3_t eye_position = g::local->eyes( );

	static auto m_penetration_system = csgo::i::cvar->find_var( _( "sv_penetration_type" ) );
	static auto m_damage_reduction_bullets = csgo::i::cvar->find_var( _( "ff_damage_reduction_bullets" ) );
	static auto m_damage_bullet_penetration = csgo::i::cvar->find_var( _( "ff_damage_bullet_penetration" ) );

	sv_penetration_type = m_penetration_system->get_int( );
	ff_damage_reduction_bullets = m_damage_reduction_bullets->get_float( );
	ff_damage_bullet_penetration = m_damage_bullet_penetration->get_float( );

	weapon_info_t* weapon_data = weapon->data( );
	if ( !weapon_data ) return false;

	trace_t enter_trace;
	trace_filter_t filter;
	filter.m_skip = g::local;

	max_range = weapon_data->m_range;
	get_bullet_type_parameters( penetration_power, penetration_distance );

	if ( sv_penetration_type )
		penetration_power = weapon_data->m_penetration;

	int possible_hits_remaining = 4;
	current_damage = weapon_data->m_dmg;

	while ( possible_hits_remaining > 0 && current_damage >= 1.f ) {
		max_range -= current_distance;

		vec3_t end = eye_position + direction * max_range;

		trace_line( eye_position, end, mask_shot_hull | contents_hitbox, g::local, &enter_trace );
		clip_trace_to_players( eye_position, end + direction * ray_extension, mask_shot_hull | contents_hitbox, &filter, &enter_trace );

		surfacedata_t* enter_surface_data = csgo::i::phys->surface( enter_trace.m_surface.m_surface_props );
		float enter_surf_penetration_modifier = enter_surface_data->m_game.m_penetration_modifier;
		int enter_material = enter_surface_data->m_game.m_material;

		if ( enter_trace.m_fraction == 1.f )
			break;

		current_distance += enter_trace.m_fraction * max_range;
		current_damage *= pow( weapon_data->m_range_modifier, ( current_distance / 500.f ) );

		if ( current_distance > penetration_distance && weapon_data->m_penetration > 0.f || enter_surf_penetration_modifier < 0.1f )
			break;

		bool can_do_damage = ( enter_trace.m_hitgroup != hitgroup_gear && enter_trace.m_hitgroup != hitgroup_generic );
		bool is_enemy = g::local->team( ) != enter_trace.m_hit_entity->team( );

		if ( ( can_do_damage && is_enemy ) ) {
			scale_damage( enter_trace, weapon_data, current_damage );
			return true;
		}

		if ( !handle_bullet_penetration( weapon_data, enter_trace, eye_position, direction, possible_hits_remaining, current_damage, penetration_power, sv_penetration_type, ff_damage_reduction_bullets, ff_damage_bullet_penetration ) )
			break;
	}

	return false;
}

float autowall::calculate_damage( const vec3_t& point ) {
	if ( !g::local->valid( ) )
		return 0.0f;

	auto data = bullet_data_t( g::local->eyes( ) );
	data.filter = trace_filter_t( );
	data.filter.m_skip = g::local;

	vec3_t angle = csgo::calc_angle( data.src, point );

	vec3_t direction = csgo::angle_vec( angle );
	direction.normalize( );

	auto weapon = g::local->weapon( );
	if ( !weapon )
		return 0.0f;

	auto weapon_data = weapon->data( );

	if ( !weapon_data )
		return 0.0f;

	float current_dmg = 0.f;
	if ( fire_bullet( weapon, direction, current_dmg ) )
		return current_dmg;

	return 0.0f;
}