#pragma once
#include <array>
#include "weapon.hpp"
#include "matrix3x4.hpp"
#include "mdl_info.hpp"
#include "entity.hpp"
#include <deque>
#include "../utils/vfunc.hpp"
#include <cassert>

#define	FL_ONGROUND				(1<<0)	// At rest / on the ground
#define FL_DUCKING				(1<<1)	// Player flag -- Player is fully crouched
#define	FL_WATERJUMP			(1<<2)	// player jumping out of water
#define FL_ONTRAIN				(1<<3) // Player is _controlling_ a train, so movement commands should be ignored on client during prediction.
#define FL_INRAIN				(1<<4)	// Indicates the entity is standing in rain
#define FL_FROZEN				(1<<5) // Player is frozen for 3rd person camera
#define FL_ATCONTROLS			(1<<6) // Player can't move, but keeps key inputs for controlling another entity
#define	FL_CLIENT				(1<<7)	// Is a player
#define FL_FAKECLIENT			(1<<8)	// Fake client, simulated server side; don't send network messages to them
// NON-PLAYER SPECIFIC (i.e., not used by GameMovement or the client .dll ) -- Can still be applied to players, though
#define	FL_INWATER				(1<<9)	// In water

// NOTE if you move things up, make sure to change this value
#define PLAYER_FLAG_BITS		10

#define	FL_FLY					(1<<10)	// Changes the SV_Movestep() behavior to not need to be on ground
#define	FL_SWIM					(1<<11)	// Changes the SV_Movestep() behavior to not need to be on ground (but stay in water)
#define	FL_CONVEYOR				(1<<12)
#define	FL_NPC					(1<<13)
#define	FL_GODMODE				(1<<14)
#define	FL_NOTARGET				(1<<15)
#define	FL_AIMTARGET			(1<<16)	// set if the crosshair needs to aim onto the entity
#define	FL_PARTIALGROUND		(1<<17)	// not all corners are valid
#define FL_STATICPROP			(1<<18)	// Eetsa static prop!
#define FL_GRAPHED				(1<<19) // worldgraph has this ent listed as something that blocks a connection
#define FL_GRENADE				(1<<20)
#define FL_STEPMOVEMENT			(1<<21)	// Changes the SV_Movestep() behavior to not do any processing
#define FL_DONTTOUCH			(1<<22)	// Doesn't generate touch functions, generates Untouch() for anything it was touching when this flag was set
#define FL_BASEVELOCITY			(1<<23)	// Base velocity has been applied this frame (used to convert base velocity into momentum)
#define FL_WORLDBRUSH			(1<<24)	// Not moveable/removeable brush entity (really part of the world, but represented as an entity for transparency or something)
#define FL_OBJECT				(1<<25) // Terrible name. This is an object that NPCs should see. Missiles, for example.
#define FL_KILLME				(1<<26)	// This entity is marked for death -- will be freed by game DLL
#define FL_ONFIRE				(1<<27)	// You know...
#define FL_DISSOLVING			(1<<28) // We're dissolving!
#define FL_TRANSRAGDOLL			(1<<29) // In the process of turning into a client side ragdoll.
#define FL_UNBLOCKABLE_BY_PLAYER (1<<30) // pusher that can't be blocked by the player

enum class movetypes {
	movetype_none = 0,
	movetype_isometric,
	movetype_walk,
	movetype_step,
	movetype_fly,
	movetype_flygravity,
	movetype_vphysics,
	movetype_push,
	movetype_noclip,
	movetype_ladder,
	movetype_observer,
	movetype_custom,
	movetype_last = movetype_custom,
	movetype_max_bits = 4
};

// Generated using ReClass 2016

/*
*	Reversed by oxy on 10/25/2019 for project o2
*	Do not remove.
*	Do not leak.
*/

class player_t;
class weapon_t;
class animstate_t;
class vec3_t;
class vec2_t;

struct animstate_pose_param_cache_t {
	std::uint8_t pad_0x0[0x4]; //0x0
	std::uint32_t m_idx; //0x4
	char* m_name; //0x8

	void set_value( player_t* e, float val );
};

class animstate_t {
public:
	std::uint8_t pad_0x0000[0x4]; //0x0000
	bool m_force_update; //0x0005
	std::uint8_t pad_0x0006[0x5A]; //0x0006
	player_t* m_entity; //0x0060
	weapon_t* m_weapon; //0x0064
	weapon_t* m_last_weapon; //0x0068
	float m_last_clientside_anim_update; //0x006C
	std::uint32_t m_last_clientside_anim_framecount; //0x0070
	float m_last_clientside_anim_update_time_delta; //0x0074
	float m_eye_yaw; //0x0078
	float m_pitch; //0x007C
	float m_abs_yaw; //0x0080
	float m_feet_yaw; //0x0084
	float m_body_yaw; //0x0088
	float m_body_yaw_clamped; //0x008C
	float m_feet_vel_dir_delta; //0x0090
	std::uint8_t pad_0x0094[0x4]; //0x0094
	float m_feet_cycle; //0x0098
	float m_feet_yaw_rate; //0x009C
	std::uint8_t pad_0x00A0[0x4]; //0x00A0
	float m_duck_amount; //0x00A4
	float m_landing_duck_additive; //0x00A8
	std::uint8_t pad_0x00AC[0x4]; //0x00AC
	vec3_t m_origin; //0x00B0
	vec3_t m_old_origin; //0x00BC
	vec2_t m_vel2d; //0x00C8
	std::uint8_t pad_0x00D0[0x10]; //0x00D0
	vec2_t m_last_accelerating_vel; //0x00E0
	std::uint8_t pad_0x00E8[0x4]; //0x00E8
	float m_speed2d; //0x00EC
	float m_up_vel; //0x00F0
	float m_speed_normalized; //0x00F4
	float m_run_speed; //0x00F8
	float m_unk_feet_speed_ratio; //0x00FC
	float m_time_since_move; //0x0100
	float m_time_since_stop; //0x0104
	bool m_on_ground; //0x0108
	bool m_hit_ground; //0x0109
	std::uint8_t pad_0x010A[0x4]; //0x010A
	float m_time_in_air; //0x0110
	std::uint8_t pad_0x0114[0x6]; //0x0114
	float m_ground_fraction; //0x011C
	std::uint8_t pad_0x0120[0x2]; //0x0120
	float m_unk_fraction; //0x0124
	std::uint8_t pad_0x0128[0xC]; //0x0128
	bool m_moving; //0x0134
	std::uint8_t pad_0x0135[0x7B]; //0x0135
	animstate_pose_param_cache_t m_lean_yaw_pose; //0x1B0
	animstate_pose_param_cache_t m_speed_pose; //0x01BC
	animstate_pose_param_cache_t m_ladder_speed_pose; //0x01C8
	animstate_pose_param_cache_t m_ladder_yaw_pose; //0x01D4
	animstate_pose_param_cache_t m_move_yaw_pose; //0x01E0
	animstate_pose_param_cache_t m_run_pose; //0x01EC
	animstate_pose_param_cache_t m_body_yaw_pose; //0x01F8
	animstate_pose_param_cache_t m_body_pitch_pose; //0x0204
	animstate_pose_param_cache_t m_dead_yaw_pose; //0x0210
	animstate_pose_param_cache_t m_stand_pose; //0x021C
	animstate_pose_param_cache_t m_jump_fall_pose; //0x0228
	animstate_pose_param_cache_t m_aim_blend_stand_idle_pose; //0x0234
	animstate_pose_param_cache_t m_aim_blend_crouch_idle_pose; //0x0240
	animstate_pose_param_cache_t m_strafe_yaw_pose; //0x024C
	animstate_pose_param_cache_t m_aim_blend_stand_walk_pose; //0x0258
	animstate_pose_param_cache_t m_aim_blend_stand_run_pose; //0x0264
	animstate_pose_param_cache_t m_aim_blend_crouch_walk_pose; //0x0270
	animstate_pose_param_cache_t m_move_blend_walk_pose; //0x027C
	animstate_pose_param_cache_t m_move_blend_run_pose; //0x0288
	animstate_pose_param_cache_t m_move_blend_crouch_pose; //0x0294
	std::uint8_t pad_0x02A0[0x4]; //0x02A0
	float m_vel_unk; //0x02A4
	std::uint8_t pad_0x02A8[0x86]; //0x02A8
	float m_min_yaw; //0x0330
	float m_max_yaw; //0x0334
	float m_max_pitch; //0x0338
	float m_min_pitch; //0x033C

	void reset( );
	void update( vec3_t& ang );
}; //Size=0x0340

struct animlayer_t {
	PAD( 20 );
	int	m_order;
	int	m_sequence;
	float m_previous_cycle;
	float m_weight;
	float m_weight_delta_rate;
	float m_playback_rate;
	float m_cycle;
	void* m_owner;
	PAD( 4 );
};

class c_bone_accessor {
public:
	int get_readable( ) {
		return m_readable;
	}

	void set_readable( int flags ) {
		m_readable = flags;
	}

	int get_writeable( ) {
		return m_writeable;
	}

	void set_writeable( int flags ) {
		m_writeable = flags;
	}

	const matrix3x4_t& get_bone( int bone ) const {
		return m_bones[bone];
	}

	matrix3x4_t& get_bone_for_write( int bone ) {
		return m_bones[bone];
	}

	std::array< matrix3x4_t, 128 >& get_bone_arr_for_write( ) const {
		return m_bones;
	};

private:
	const void* m_animating;
	std::array< matrix3x4_t, 128 >& m_bones;
	int m_readable;
	int m_writeable;
};

struct anim_list_record_t {
	player_t* m_ent;
	std::uint32_t m_flags;
};

struct anim_list_t {
	anim_list_record_t* m_data;
};

class player_t : public entity_t {
public:
public:
	NETVAR(std::uint32_t, flags, "DT_BasePlayer->m_fFlags");
	NETVAR(bool, has_defuser, "DT_CSPlayer->m_bHasDefuser");
	NETVAR(bool, immunity, "DT_CSPlayer->m_bGunGameImmunity");
	NETVAR(int32_t, shots_fired, "DT_CSPlayer->m_iShotsFired");
	NETVAR(vec3_t, angles, "DT_CSPlayer->m_angEyeAngles[0]");
	NETVAR(bool, has_heavy_armor, "DT_CSPlayer->m_bHasHeavyArmor");
	NETVAR(int, health, "DT_BasePlayer->m_iHealth");
	NETVAR(int, armor, "DT_CSPlayer->m_ArmorValue");
	NETVAR(bool, has_helmet, "DT_CSPlayer->m_bHasHelmet");
	NETVAR(bool, scoped, "DT_CSPlayer->m_bIsScoped");
	NETVAR(int, money, "DT_CSPlayer->m_iAccount");
	NETVAR(uint32_t, viewmodel_handle, "DT_BasePlayer->m_hViewModel[0]");
	NETVAR(matrix3x4_t, collision_group, "DT_BasePlayer->m_CollisionGroup");
	NETVAR(float, lby, "DT_CSPlayer->m_flLowerBodyYawTarget");
	NETVAR(int, account, "DT_CSPlayer->m_iAccount");
	NETVAR(float, flash_duration, "DT_CSPlayer->m_flFlashDuration");
	NETVAR(float, flash_alpha, "DT_CSPlayer->m_flFlashAlpha");
	NETVAR(float, health_shot_exp, "DT_CSPlayer->m_flHealthShotBoostExpirationTime");
	NETVAR(float, next_attack, "DT_CSPlayer->m_flNextAttack");
	NETVAR(std::uint8_t, life_state, "DT_BasePlayer->m_lifeState");
	NETVAR(std::uint32_t, tick_base, "DT_BasePlayer->m_nTickBase");
	NETVAR(float, crouch_amount, "DT_BasePlayer->m_flDuckAmount");
	NETVAR(vec3_t, view_punch, "DT_BasePlayer->m_viewPunchAngle");
	NETVAR(vec3_t, get_ragdoll_pos, "DT_Ragdoll->m_ragPos");
	NETVAR(float, velocity_modifier, "DT_CSPlayer->m_flVelocityModifier");
	NETVAR(bool, is_valve_ds, "DT_CSGameRulesProxy->m_bIsValveDS");
	NETVAR(vec3_t, aim_punch, "DT_BasePlayer->m_aimPunchAngle");
	NETVAR(vec3_t, vel, "DT_BasePlayer->m_vecVelocity[0]");
	NETVAR(bool, animate, "DT_BaseAnimating->m_bClientSideAnimation");
	NETVAR(vec3_t, base_vel, "DT_BasePlayer->m_vecBaseVelocity");
	NETVAR(vec3_t, stamina, "DT_CSPlayer->m_flStamina");
	NETVAR(float, fall_vel, "DT_BasePlayer->m_flFallVelocity");
	NETVAR(uint32_t, ground_entity_handle, "DT_BasePlayer->m_hGroundEntity");
	NETVAR(float, crouch_speed, "DT_BasePlayer->m_flDuckSpeed");
	NETVAR(uint32_t*, weapon_handle, "DT_BaseCombatCharacter->m_hActiveWeapon");
	NETVAR(vec3_t, view_offset, "DT_BasePlayer->m_vecViewOffset[0]");
	NETVAR(float, simtime, "DT_BaseEntity->m_flSimulationTime");
	NETVAR(bool, spotted, "DT_BaseEntity->m_bSpotted");
	NETVAR(float, get_max_flash_alpha, "DT_CSPlayer->m_flFlashMaxAlpha");
	NETVAR_ADDITIVE(movetypes, movetype, "DT_BaseEntity->m_nRenderMode", 1);
	NETVAR(vec3_t, mins, "DT_CSPlayer->m_vecMins");
	NETVAR(vec3_t, maxs, "DT_CSPlayer->m_vecMaxs");
	NETVAR(std::uint32_t, observer_mode, "DT_CSPlayer->m_iObserverMode");
	NETVAR(uint32_t, observertarget, "DT_BasePlayer->m_hObserverTarget");
	NETVAR(int32_t, hitbox_set, "DT_BaseAnimating->m_nHitboxSet");
	NETVAR(bool, defusing, "DT_CSPlayer->m_bIsDefusing");
	OFFSET(int, effects, 0xE4);
	OFFSET(int, eflags, 0xE8);
	OFFSET(vec3_t, aim_punch_vel, 0x3028);
	OFFSET(animlayer_t*, overlay, 0x2980);
	OFFSET(animstate_t*, animstate, 0x3914);
	OFFSET(float*, pose_params, 0x2774);
	OFFSET(void*, iks, 0x266C);
	OFFSET(bool, should_update, 0x289C);
	OFFSET(std::uint32_t, num_overlays, 0x298C);
	OFFSET(float, spawn_time, 0xA350);
	OFFSET(c_bone_accessor, bone_accessor, 0x290C);

	[[nodiscard]] int& get_take_damage( ) {
		return *( int* ) ( ( std::uintptr_t ) this + 0x280 );
	}

	vec3_t get_hitbox_position( int hitbox_id ) {
		auto hitbox = get_hitbox( hitbox_id );
		if ( !hitbox )
			return vec3_t( 0, 0, 0 );

		auto bone_matrix = this->get_bone_matrix( hitbox->m_bone );

		vec3_t bbmin, bbmax;

		VEC_TRANSFORM( hitbox->m_bbmin, bone_matrix, bbmin );
		VEC_TRANSFORM( hitbox->m_bbmax, bone_matrix, bbmax );

		return ( bbmin + bbmax ) * 0.5f;
	}

	bool is_player( ) {
		using fn = bool( __thiscall* )( void* );
		return vfunc< fn >( this, 155 )( this );
	}

	std::array< animlayer_t, 15 >& overlays( ) {
		return *reinterpret_cast< std::array< animlayer_t, 15 > * >( std::uintptr_t( this ) + 0x2980 );
	}

	std::array< float, 24 >& poses( ) {
		return *reinterpret_cast< std::array< float, 24 > * >( std::uintptr_t( this ) + 0x2774 );
	}

	void* seq_desc( int seq ) {
		auto group_hdr = *reinterpret_cast< void** >( std::uintptr_t( this ) + 0xA53 );
		auto i = seq;

		if ( seq < 0 || seq >= *reinterpret_cast< std::uint32_t* >( std::uintptr_t( group_hdr ) + 0xBC ) )
			i = 0;

		return reinterpret_cast< void* >( std::uintptr_t( group_hdr ) + *reinterpret_cast< std::uintptr_t* >( std::uintptr_t( group_hdr ) + 0xC0 ) + 0xD4 * i );
	}

	void set_local_viewangles( const vec3_t& ang ) {
		using fn = void( __thiscall* )( void*, const vec3_t& );
		vfunc< fn >( this, 371 )( this, ang );
	}

	int get_sequence_activity(int sequence);

	bool physics_run_think( int unk01 );

	void think( ) {
		vfunc< void( __thiscall* )( void* ) >( this, 138 )( this );
	}

	void pre_think( ) {
		vfunc< void( __thiscall* )( void* ) >( this, 317 )( this );
	}

	void post_think( ) {
		vfunc< void( __thiscall* )( void* ) >( this, 318 )( this );
	}

	vec3_t world_space( ) {
		vec3_t wspace;
		vec3_t va, vb;

		using fn = void( __thiscall* )( void*, vec3_t&, vec3_t& );
		vfunc< fn >( this + 0x4, 17 )( this + 0x4, va, vb );

		wspace = abs_origin( );
		wspace.z += ( va.z + vb.z ) * 0.5f;

		return wspace;
	}

	float& old_simtime( ) {
		return *( float* ) ( std::uintptr_t( &simtime( ) ) + 4 );
	}

	void* mdl( void ) {
		void* r = this->renderable( );

		if ( !r )
			return nullptr;

		using getmdl_fn = void* ( __thiscall* )( void* );
		return vfunc< getmdl_fn >( r, 8 )( r );
	}

	studiohdr_t* studio_mdl( void* mdl ) {
		using getstudiomdl_fn = studiohdr_t * ( __thiscall* )( void*, void* );
		return vfunc< getstudiomdl_fn >( this, 32 )( this, mdl );
	}

	bool alive( ) {
		return !!health( );
	}

	bool valid( ) {
		return this && alive( ) && !dormant( ) && !life_state( );
	}

	vec3_t& abs_origin( ) {
		using fn = vec3_t & ( __thiscall* )( void* );
		return vfunc< fn >( this, 10 )( this );
	}

	vec3_t& abs_angles( ) {
		using fn = vec3_t & ( __thiscall* )( void* );
		return vfunc< fn >( this, 11 )( this );
	}

	std::uint32_t handle( );

	bool setup_bones( matrix3x4_t* m, std::uint32_t max, std::uint32_t mask, float seed ) {
		using setupbones_fn = bool( __thiscall* )( void*, matrix3x4_t*, std::uint32_t, std::uint32_t, float );
		return vfunc< setupbones_fn >( renderable( ), 13 )( renderable( ), m, max, mask, seed );
	}

	void estimate_abs_vel( vec3_t& vec ) {
		using fn = void( __thiscall* )( void*, vec3_t& );
		vfunc< fn >( this, 144 )( this, vec );
	}

	vec3_t& const render_origin( ) {
		using fn = vec3_t& const( __thiscall* )( void* );
		return vfunc< fn >( renderable( ), 1 )( renderable( ) );
	}

	void create_animstate( animstate_t* state );
	void inval_bone_cache( );
	void set_abs_angles( const vec3_t& ang );
	void update( );
	vec3_t& abs_vel( );
	void set_abs_vel( vec3_t& vel );
	void set_abs_origin( vec3_t& vec );
	vec3_t get_bone_position(int i);
	matrix3x4_t get_bone_matrix( int bone_id );
	mstudiobbox_t* get_hitbox( int hitbox_index );
	std::uint32_t& bone_count( );
	std::array< matrix3x4_t, 128 >& bone_cache( );
	vec3_t eyes( );
	bool is_visible( const vec3_t& from, const vec3_t& to, bool smoke_check = false );
	bool has_c4( );
	std::vector<weapon_t*> weapons( );
	weapon_t* weapon( );

	float desync_amount( ) {
		auto state = animstate( );

		if ( !state )
			return 0.0f;

		auto running_speed = std::clamp( state->m_run_speed, 0.0f, 1.0f );
		auto yaw_modifier = ( ( ( state->m_ground_fraction * -0.3f ) - 0.2f ) * running_speed ) + 1.0f;

		if ( state->m_duck_amount > 0.0f ) {
			auto speed_factor = std::clamp( state->m_unk_feet_speed_ratio, 0.0f, 1.0f );
			yaw_modifier += ( ( state->m_duck_amount * speed_factor ) * ( 0.5f - yaw_modifier ) );
		}

		return yaw_modifier * state->m_max_yaw;
	}
};