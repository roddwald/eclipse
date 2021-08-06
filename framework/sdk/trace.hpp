#pragma once
#include <xmmintrin.h>
#include "matrix3x4.hpp"
#include "entity.hpp"
#include "bsp.hpp"

#define MAX_COORD_FLOAT		16'384.f			// max world coordinates
#define MIN_COORD_FLOAT		-MAX_COORD_FLOAT	// min world coordinates
#define MAX_DISTANCE		8'192.f				// max bullet distance, etc
#define MAX_FOV				180.f				// max field of view
#define MAX_DAMAGE			500.f				// max weapons damage
#define MAX_WEAPONS			64					// max number of weapons available

enum EDamageType : int {
	DAMAGE_NO = 0 ,
	DAMAGE_EVENTS_ONLY ,
	DAMAGE_YES ,
	DAMAGE_AIM
};

#pragma region valve_decals
#define char_tex_antlion		'A'
#define char_tex_bloodyflesh	'B'
#define char_tex_concrete		'C'
#define char_tex_dirt			'D'
#define char_tex_eggshell		'E'
#define char_tex_flesh			'F'
#define char_tex_grate			'G'
#define char_tex_alienflesh		'H'
#define char_tex_clip			'I'
#define char_tex_snow			'K'
#define char_tex_plastic		'L'
#define char_tex_metal			'M'
#define char_tex_sand			'N'
#define char_tex_foliage		'O'
#define char_tex_computer		'P'
#define char_tex_reflective		'R'
#define char_tex_slosh			'S'
#define char_tex_tile			'T'
#define char_tex_cardboard		'U'
#define char_tex_vent			'V'
#define char_tex_wood			'W'
#define char_tex_glass			'Y'
#define char_tex_warpshield		'Z'
#pragma endregion

// @note: see "sv_dump_class_info" command
enum class EClassIndex : int {
	CAI_BaseNPC = 0 ,
	CAK47 ,
	CBaseAnimating ,
	CBaseAnimatingOverlay ,
	CBaseAttributableItem ,
	CBaseButton ,
	CBaseCombatCharacter ,
	CBaseCombatWeapon ,
	CBaseCSGrenade ,
	CBaseCSGrenadeProjectile ,
	CBaseDoor ,
	player_t ,
	CBaseFlex ,
	CBaseGrenade ,
	CBaseParticleEntity ,
	CBasePlayer ,
	CBasePropDoor ,
	CBaseTeamObjectiveResource ,
	CBaseTempEntity ,
	CBaseToggle ,
	CBaseTrigger ,
	CBaseViewModel ,
	CBaseVPhysicsTrigger ,
	CBaseWeaponWorldModel ,
	CBeam ,
	CBeamSpotlight ,
	CBoneFollower ,
	CBRC4Target ,
	CBreachCharge ,
	CBreachChargeProjectile ,
	CBreakableProp ,
	CBreakableSurface ,
	CBumpMine ,
	CBumpMineProjectile ,
	CC4 ,
	CCascadeLight ,
	CChicken ,
	CColorCorrection ,
	CColorCorrectionVolume ,
	CCSGameRulesProxy ,
	CCSPlayer ,
	CCSPlayerResource ,
	CCSRagdoll ,
	CCSTeam ,
	CDangerZone ,
	CDangerZoneController ,
	CDEagle ,
	CDecoyGrenade ,
	CDecoyProjectile ,
	CDrone ,
	CDronegun ,
	CDynamicLight ,
	CDynamicProp ,
	CEconEntity ,
	CEconWearable ,
	CEmbers ,
	CEntityDissolve ,
	CEntityFlame ,
	CEntityFreezing ,
	CEntityParticleTrail ,
	CEnvAmbientLight ,
	CEnvDetailController ,
	CEnvDOFController ,
	CEnvGasCanister ,
	CEnvParticleScript ,
	CEnvProjectedTexture ,
	CEnvQuadraticBeam ,
	CEnvScreenEffect ,
	CEnvScreenOverlay ,
	CEnvTonemapController ,
	CEnvWind ,
	CFEPlayerDecal ,
	CFireCrackerBlast ,
	CFireSmoke ,
	CFireTrail ,
	CFish ,
	CFists ,
	CFlashbang ,
	CFogController ,
	CFootstepControl ,
	CFunc_Dust ,
	CFunc_LOD ,
	CFuncAreaPortalWindow ,
	CFuncBrush ,
	CFuncConveyor ,
	CFuncLadder ,
	CFuncMonitor ,
	CFuncMoveLinear ,
	CFuncOccluder ,
	CFuncReflectiveGlass ,
	CFuncRotating ,
	CFuncSmokeVolume ,
	CFuncTrackTrain ,
	CGameRulesProxy ,
	CGrassBurn ,
	CHandleTest ,
	CHEGrenade ,
	CHostage ,
	CHostageCarriableProp ,
	CIncendiaryGrenade ,
	CInferno ,
	CInfoLadderDismount ,
	CInfoMapRegion ,
	CInfoOverlayAccessor ,
	CItem_Healthshot ,
	CItemCash ,
	CItemDogtags ,
	CKnife ,
	CKnifeGG ,
	CLightGlow ,
	CMaterialModifyControl ,
	CMelee ,
	CMolotovGrenade ,
	CMolotovProjectile ,
	CMovieDisplay ,
	CParadropChopper ,
	CParticleFire ,
	CParticlePerformanceMonitor ,
	CParticleSystem ,
	CPhysBox ,
	CPhysBoxMultiplayer ,
	CPhysicsProp ,
	CPhysicsPropMultiplayer ,
	CPhysMagnet ,
	CPhysPropAmmoBox ,
	CPhysPropLootCrate ,
	CPhysPropRadarJammer ,
	CPhysPropWeaponUpgrade ,
	CPlantedC4 ,
	CPlasma ,
	CPlayerPing ,
	CPlayerResource ,
	CPointCamera ,
	CPointCommentaryNode ,
	CPointWorldText ,
	CPoseController ,
	CPostProcessController ,
	CPrecipitation ,
	CPrecipitationBlocker ,
	CPredictedViewModel ,
	CProp_Hallucination ,
	CPropCounter ,
	CPropDoorRotating ,
	CPropJeep ,
	CPropVehicleDriveable ,
	CRagdollManager ,
	CRagdollProp ,
	CRagdollPropAttached ,
	CRopeKeyframe ,
	CSCAR17 ,
	CSceneEntity ,
	CSensorGrenade ,
	CSensorGrenadeProjectile ,
	CShadowControl ,
	CSlideshowDisplay ,
	CSmokeGrenade ,
	CSmokeGrenadeProjectile ,
	CSmokeStack ,
	CSnowball ,
	CSnowballPile ,
	CSnowballProjectile ,
	CSpatialEntity ,
	CSpotlightEnd ,
	CSprite ,
	CSpriteOriented ,
	CSpriteTrail ,
	CStatueProp ,
	CSteamJet ,
	CSun ,
	CSunlightShadowControl ,
	CSurvivalSpawnChopper ,
	CTablet ,
	CTeam ,
	CTeamplayRoundBasedRulesProxy ,
	CTEArmorRicochet ,
	CTEBaseBeam ,
	CTEBeamEntPoint ,
	CTEBeamEnts ,
	CTEBeamFollow ,
	CTEBeamLaser ,
	CTEBeamPoints ,
	CTEBeamRing ,
	CTEBeamRingPoint ,
	CTEBeamSpline ,
	CTEBloodSprite ,
	CTEBloodStream ,
	CTEBreakModel ,
	CTEBSPDecal ,
	CTEBubbles ,
	CTEBubbleTrail ,
	CTEClientProjectile ,
	CTEDecal ,
	CTEDust ,
	CTEDynamicLight ,
	CTEEffectDispatch ,
	CTEEnergySplash ,
	CTEExplosion ,
	CTEFireBullets ,
	CTEFizz ,
	CTEFootprintDecal ,
	CTEFoundryHelpers ,
	CTEGaussExplosion ,
	CTEGlowSprite ,
	CTEImpact ,
	CTEKillPlayerAttachments ,
	CTELargeFunnel ,
	CTEMetalSparks ,
	CTEMuzzleFlash ,
	CTEParticleSystem ,
	CTEPhysicsProp ,
	CTEPlantBomb ,
	CTEPlayerAnimEvent ,
	CTEPlayerDecal ,
	CTEProjectedDecal ,
	CTERadioIcon ,
	CTEShatterSurface ,
	CTEShowLine ,
	CTesla ,
	CTESmoke ,
	CTESparks ,
	CTESprite ,
	CTESpriteSpray ,
	CTest_ProxyToggle_Networkable ,
	CTestTraceline ,
	CTEWorldDecal ,
	CTriggerPlayerMovement ,
	CTriggerSoundOperator ,
	CVGuiScreen ,
	CVoteController ,
	CWaterBullet ,
	CWaterLODControl ,
	CWeaponAug ,
	CWeaponAWP ,
	CWeaponBaseItem ,
	CWeaponBizon ,
	CWeaponCSBase ,
	CWeaponCSBaseGun ,
	CWeaponCycler ,
	CWeaponElite ,
	CWeaponFamas ,
	CWeaponFiveSeven ,
	CWeaponG3SG1 ,
	CWeaponGalil ,
	CWeaponGalilAR ,
	CWeaponGlock ,
	CWeaponHKP2000 ,
	CWeaponM249 ,
	CWeaponM3 ,
	CWeaponM4A1 ,
	CWeaponMAC10 ,
	CWeaponMag7 ,
	CWeaponMP5Navy ,
	CWeaponMP7 ,
	CWeaponMP9 ,
	CWeaponNegev ,
	CWeaponNOVA ,
	CWeaponP228 ,
	CWeaponP250 ,
	CWeaponP90 ,
	CWeaponSawedoff ,
	CWeaponSCAR20 ,
	CWeaponScout ,
	CWeaponSG550 ,
	CWeaponSG552 ,
	CWeaponSG556 ,
	CWeaponShield ,
	CWeaponSSG08 ,
	CWeaponTaser ,
	CWeaponTec9 ,
	CWeaponTMP ,
	CWeaponUMP45 ,
	CWeaponUSP ,
	CWeaponXM1014 ,
	CWorld ,
	CWorldVguiText ,
	DustTrail ,
	MovieExplosion ,
	ParticleSmokeGrenade ,
	RocketTrail ,
	SmokeTrail ,
	SporeExplosion ,
	SporeTrail ,
};

enum hitboxes_t : int {
	hitbox_invalid = -1 ,
	hitbox_head ,
	hitbox_neck ,
	hitbox_pelvis ,
	hitbox_stomach ,
	hitbox_lower_chest ,
	hitbox_chest ,
	hitbox_upper_chest ,
	hitbox_right_thigh ,
	hitbox_left_thigh ,
	hitbox_right_calf ,
	hitbox_left_calf ,
	hitbox_right_foot ,
	hitbox_left_foot ,
	hitbox_right_hand ,
	hitbox_left_hand ,
	hitbox_right_upper_arm ,
	hitbox_right_forearm ,
	hitbox_left_upper_arm ,
	hitbox_left_forearm ,
	hitbox_max
};

enum hitgroup_t : int {
	hitgroup_generic = 0 ,
	hitgroup_head = 1 ,
	hitgroup_chest = 2 ,
	hitgroup_stomach = 3 ,
	hitgroup_leftarm = 4 ,
	hitgroup_rightarm = 5 ,
	hitgroup_leftleg = 6 ,
	hitgroup_rightleg = 7 ,
	hitgroup_gear = 10
};

enum dispsurf_t {
	dispsurf_flag_surface = ( 1 << 0 ) ,
	dispsurf_flag_walkable = ( 1 << 1 ) ,
	dispsurf_flag_buildable = ( 1 << 2 ) ,
	dispsurf_flag_surfprop1 = ( 1 << 3 ) ,
	dispsurf_flag_surfprop2 = ( 1 << 4 )
};

enum tracetype_t {
	trace_everything = 0 ,
	trace_world_only ,
	trace_entities_only ,
	trace_everything_filter_props
};

struct plane_t {
	vec3_t m_normal;
	float m_dist;
	std::uint8_t m_type;
	std::uint8_t m_sign_bits;
	PAD( 2 );
};

struct surface_t {
	const char * m_name;
	short m_surface_props;
	std::uint8_t m_flags;
};

class base_trace_t {
public:
	bool is_disp_surface( void ) {
		return ( ( m_disp_flags & dispsurf_flag_surface ) != 0 );
	}
	bool is_disp_surface_walkable( void ) {
		return ( ( m_disp_flags & dispsurf_flag_walkable ) != 0 );
	}
	bool is_disp_surface_buildable( void ) {
		return ( ( m_disp_flags & dispsurf_flag_buildable ) != 0 );
	}
	bool is_disp_surface_prop1( void ) {
		return ( ( m_disp_flags & dispsurf_flag_surfprop1 ) != 0 );
	}
	bool is_disp_surface_prop2( void ) {
		return ( ( m_disp_flags & dispsurf_flag_surfprop2 ) != 0 );
	}

public:
	vec3_t m_startpos;
	vec3_t m_endpos;
	plane_t m_plane;

	float m_fraction;

	std::uint32_t m_contents;
	std::uint16_t m_disp_flags;

	bool m_allsolid;
	bool m_startsolid;

	base_trace_t( ) {
	}
};

class trace_t : public base_trace_t {
public:
	bool did_hit_world( ) const;

	bool did_hit_non_world_ent( ) const;

	int get_entity_index( ) const {
	}

	bool did_hit( ) const {
		return m_fraction < 1 || m_allsolid || m_startsolid;
	}

	bool is_visible( ) const {
		return m_fraction > 0.97f;
	}

public:
	float m_fractionleftsolid;
	surface_t m_surface;
	int m_hitgroup;
	short m_physicsbone;
	std::uint16_t m_world_surface_index;
	entity_t * m_hit_entity;
	int m_hitbox;

	trace_t( ) {
	}

private:
	trace_t( const trace_t & other ) :
		m_fractionleftsolid( other.m_fractionleftsolid ) ,
		m_surface( other.m_surface ) ,
		m_hitgroup( other.m_hitgroup ) ,
		m_physicsbone( other.m_physicsbone ) ,
		m_world_surface_index( other.m_world_surface_index ) ,
		m_hit_entity( other.m_hit_entity ) ,
		m_hitbox( other.m_hitbox ) {
		m_startpos = other.m_startpos;
		m_endpos = other.m_endpos;
		m_plane = other.m_plane;
		m_fraction = other.m_fraction;
		m_contents = other.m_contents;
		m_disp_flags = other.m_disp_flags;
		m_allsolid = other.m_allsolid;
		m_startsolid = other.m_startsolid;
	}
};

class __trace_filter_t {
public:
	virtual bool should_hit_ent( entity_t * ent , std::uint32_t mask ) = 0;
	virtual std::uint32_t get_trace_type( ) const = 0;
};

class _trace_filter_t : public __trace_filter_t {
public:
	void * m_skip;

	bool should_hit_ent( entity_t * ent , std::uint32_t ) override {
		return ent != m_skip;
	}

	std::uint32_t get_trace_type( ) const override {
		return 0;
	}
};

class trace_filter_t : public _trace_filter_t {
public:
	void * m_skip;

	trace_filter_t( ) {
		m_skip = nullptr;
	}

	trace_filter_t( void * ent ) {
		m_skip = ent;
	}

	bool should_hit_ent( entity_t * ent , std::uint32_t ) override {
		return !( ent == m_skip );
	}

	std::uint32_t get_trace_type( ) const override {
		return 0;
	}
};

struct ray_t {
	vec_alligned start;
	vec_alligned delta;
	vec_alligned start_offset;
	vec_alligned extents;
	const matrix3x4_t * world_axis_transform;
	bool is_ray;
	bool is_swept;

	ray_t( ) : world_axis_transform( NULL ) {
	}

	void init( vec3_t const & src , vec3_t const & end ) {
		delta = end - src;
		is_swept = delta.length_sqr( ) != 0.f;
		extents.init( );
		world_axis_transform = NULL;
		is_ray = true;
		start_offset.init( );
		start = src;
	}

	void init( vec3_t const & m_start , vec3_t const & end , vec3_t const & mins , vec3_t const & maxs ) {
		delta = end - m_start;
		world_axis_transform = nullptr;
		is_swept = delta.length_sqr( ) != 0.0f;

		extents = maxs - mins;
		extents *= 0.5f;
		is_ray = extents.length_sqr( ) < 1e-6f;

		start_offset = maxs + mins;
		start_offset *= 0.5f;
		start = m_start + start_offset;
		start_offset *= -1.0f;
	}

	vec3_t inv_delta( ) const {
		vec3_t ret;
		for ( int m_axis = 0; m_axis < 3; ++m_axis ) {
			if ( delta[ m_axis ] != 0.0f ) {
				ret[ m_axis ] = 1.0f / delta[ m_axis ];
			}
			else {
				ret[ m_axis ] = FLT_MAX;
			}
		}
		return ret;
	}

private:
};

class c_engine_trace {
public:
	virtual int get_point_contents( const vec3_t & pos , int mask = mask_all , entity_t ** ent = nullptr ) = 0; // 0
	virtual int get_point_contents_world_only( const vec3_t & pos , int mask = mask_all ) = 0; // 1
	virtual int get_point_contents_collideable( void * collide /* collidable_t */ , const vec3_t & pos ) = 0; // 2
	virtual void clip_ray_to_entity( const ray_t & ray , unsigned int mask , entity_t * ent , trace_t * trace ) = 0; // 3
	virtual void clip_ray_to_collideable( const ray_t & ray , unsigned int mask , void * collide /* collidable_t */ , trace_t * trace ) = 0; // 4
	virtual void trace_ray( const ray_t & ray , unsigned int mask , __trace_filter_t * filter , trace_t * trace ) = 0; // 5
};