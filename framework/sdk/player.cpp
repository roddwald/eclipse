#include <array>
#include "player.hpp"
#include "sdk.hpp"

void animstate_pose_param_cache_t::set_value( player_t* e, float val ) {
	if ( m_idx >= 0 )
		e->poses( )[m_idx] = val;
}

void animstate_t::reset( ) {
	static auto reset_animstate = pattern::search( client_dll, _( "56 6A 01 68 ? ? ? ? 8B F1" ) ).get< void( __thiscall* )( void* ) >( );
	reset_animstate( this );
}

int player_t::get_sequence_activity(int sequence) {
	auto hdr = csgo::i::mdl_info->studio_mdl(this->mdl());

	if (!hdr)
		return -1;

	using fn = int(__fastcall*)(void*, void*, int);
	static auto sequence_activity = pattern::search(client_dll, _("55 8B EC 53 8B 5D 08 56 8B F1 83")).get< fn >();

	return sequence_activity(this, hdr, sequence);
}

void animstate_t::update( vec3_t& ang ) {
	static auto update_animstate = pattern::search( client_dll, _( "55 8B EC 83 E4 F8 83 EC 18 56 57 8B F9 F3 0F 11 54" ) ).get< void( __vectorcall* )( void*, void*, float, float, float, void* ) >( );
	update_animstate( this, nullptr, ang.z, ang.y, ang.x, nullptr );
}

bool player_t::physics_run_think( int unk01 ) {
	static auto func = pattern::search( client_dll, _( "55 8B EC 83 EC 10 53 56 57 8B F9 8B 87 ? ? ? ? C1 E8 16" ) ).get< bool( __thiscall* )( void*, int ) >( );
	return func( this, unk01 );
}

std::uint32_t player_t::handle( ) {
	using fn = std::uint32_t( __thiscall* )( void* );
	static auto get_ehandle = pattern::search( client_dll, _( "8B 51 3C 83 FA FF" ) ).get< fn >( );
	return get_ehandle( this );
}

void player_t::create_animstate( animstate_t* state ) {
	static auto create_animstate = pattern::search( client_dll, _( "55 8B EC 56 8B F1 B9 ? ? ? ? C7 46" ) ).get< void( __thiscall* )( void*, void* ) >( );
	create_animstate( state, this );
}

void player_t::inval_bone_cache( ) {
	static auto invalidate_bone_cache = pattern::search( client_dll, _( "80 3D ? ? ? ? ? 74 16 A1 ? ? ? ? 48 C7 81" ) ).add( 10 ).get< std::uintptr_t >( );

	*( std::uint32_t* ) ( ( std::uintptr_t ) this + 0x2924 ) = 0xFF7FFFFF;
	*( std::uint32_t* ) ( ( std::uintptr_t ) this + 0x2690 ) = **( std::uintptr_t** ) invalidate_bone_cache - 1;
}

void player_t::set_abs_angles( const vec3_t& ang ) {
	using setabsangles_fn = void( __thiscall* )( void*, const vec3_t& );
	static auto set_abs_angles = pattern::search( client_dll, _( "55 8B EC 83 E4 F8 83 EC 64 53 56 57 8B F1 E8" ) ).get< setabsangles_fn >( );
	set_abs_angles( this, ang );
}

void player_t::update( ) {
	using update_clientside_animation_fn = void( __thiscall* )( void* );
	static auto update_clientside_animation = pattern::search( client_dll, _( "8B F1 80 BE ? ? ? ? 00 74 ? 8B 06 FF 90 ? ? ? ? 8B CE" ) ).sub( 5 ).get< update_clientside_animation_fn >( );
	update_clientside_animation( this );
}

vec3_t& player_t::abs_vel( ) {
	// enable changing of abs velocities
	// *( std::uint32_t* ) ( std::uintptr_t( this ) + 0xe8 ) |= 0x1000;
	// old abs velocity
	// *( vec3_t* ) ( std::uintptr_t( this ) + 0x94 ) = vel;
	// current abs velocity
	return *( vec3_t* ) ( std::uintptr_t( this ) + 0x94 );
}

void player_t::set_abs_vel( vec3_t& vel ) {
	using set_abs_vel_fn = void( __thiscall* )( void*, vec3_t& );
	static auto fn = pattern::search( client_dll, _( "55 8B EC 83 E4 F8 83 EC 0C 53 56 57 8B 7D 08 8B F1" ) ).get< set_abs_vel_fn >( );
	fn( this, vel );
}

void player_t::set_abs_origin( vec3_t& vec ) {
	using set_abs_origin_fn = void( __thiscall* )( void*, vec3_t& );
	static auto fn = pattern::search( client_dll, _( "55 8B EC 83 E4 F8 51 53 56 57 8B F1 E8 ? ? ? ? 8B 7D" ) ).get< set_abs_origin_fn >( );
	fn( this, vec );
}

vec3_t player_t::get_bone_position(int i) {
	assert(i > -1 && i < 256);

	std::array<matrix3x4_t, 256> bones_to_world = { };

	if (this->setup_bones(bones_to_world.data(), bones_to_world.size(), 0x0007FF00, 0.f))
		return bones_to_world.at(i).at(3);

	return vec3_t(0, 0, 0);
}

matrix3x4_t player_t::get_bone_matrix( int bone_id ) {
	matrix3x4_t matrix;
	auto offset = *reinterpret_cast< uintptr_t* >( uintptr_t( this ) + 0x26A8 );

	if ( offset )
		matrix = *reinterpret_cast< matrix3x4_t* >( offset + 0x30 * bone_id );

	return matrix;
}

vec3_t player_t::eyes( ) {
	using weapon_shootpos_fn = float* ( __thiscall* )( void*, vec3_t* );
	static auto fn = pattern::search( client_dll, _( "55 8B EC 56 8B 75 08 57 8B F9 56 8B 07 FF 90" ) ).get< weapon_shootpos_fn >( );

	vec3_t ret;
	fn( this, &ret );

	return ret;
}

bool player_t::is_visible( const vec3_t& from, const vec3_t& to, bool smoke_check ) {
	ray_t ray;
	ray.init( from, to );

	trace_t tr;
	trace_filter_t filter;

	filter.m_skip = this;

	csgo::i::trace->trace_ray( ray, mask_shot, &filter, &tr );

	if ( tr.m_fraction > 0.97f && !( smoke_check && csgo::line_goes_through_smoke( from, to, false ) ) )
		return true;

	return false;
}

bool player_t::has_c4( ) {
	static auto offset = pattern::search( client_dll, _( "56 8B F1 85 F6 74 31" ) ).get<bool>( );
	return offset;
}

std::vector<weapon_t*> player_t::weapons( ) {
	static auto offset = netvars::get_offset( _( "DT_BaseCombatCharacter->m_hMyWeapons" ) );

	std::vector<weapon_t*> ret {};

	const auto my_weapons = reinterpret_cast< uint32_t* >( reinterpret_cast< uintptr_t >( this ) + offset );

	for ( auto i = 0; my_weapons[i] != 0xFFFFFFFF; i++ ) {
		const auto weapon = csgo::i::ent_list->get_by_handle<weapon_t*>( my_weapons[i] );

		if ( !weapon )
			continue;

		ret.push_back( weapon );
	}

	return ret;
}

mstudiobbox_t* player_t::get_hitbox( int hitbox_index ) {
	if ( this->dormant( ) || this->health( ) <= 0 )
		return NULL;

	const auto mdl = this->mdl( );
	if ( !mdl )
		return NULL;

	auto m_studiohdr = csgo::i::mdl_info->studio_mdl( mdl );
	if ( !m_studiohdr )
		return NULL;

	auto set = m_studiohdr->hitbox_set( 0 );
	if ( !set )
		return NULL;

	if ( hitbox_index >= set->m_num_hitboxes || hitbox_index < 0 )
		return NULL;

	return set->hitbox( hitbox_index );
}

std::uint32_t& player_t::bone_count( ) {
	static auto offset = pattern::search( client_dll, _( "8B 87 ? ? ? ? 8B 4D 0C" ) ).add( 2 ).deref( ).get< std::uint32_t >( );
	return *( uint32_t* ) ( std::uintptr_t( renderable( ) ) + offset );
}

std::array< matrix3x4_t, 128 >& player_t::bone_cache( ) {
	static auto offset = pattern::search( client_dll, _( "FF B7 ? ? ? ? 52" ) ).add( 2 ).deref( ).get< std::uint32_t >( );
	return *( std::array< matrix3x4_t, 128 > * ) ( std::uintptr_t( renderable( ) ) + offset );
}

weapon_t* player_t::weapon( ) {
	return csgo::i::ent_list->get_by_handle< weapon_t* >( ( uint32_t ) weapon_handle( ) );
}