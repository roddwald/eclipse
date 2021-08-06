#include "sdk.hpp"
#include "netvar.hpp"
//#include "input_system.hpp"

c_entlist* csgo::i::ent_list = nullptr;
c_matsys* csgo::i::mat_sys = nullptr;
c_mdlinfo* csgo::i::mdl_info = nullptr;
c_mdlrender* csgo::i::mdl_render = nullptr;
c_renderview* csgo::i::render_view = nullptr;
c_client* csgo::i::client = nullptr;
c_surface* csgo::i::surface = nullptr;
c_engine* csgo::i::engine = nullptr;
c_globals* csgo::i::globals = nullptr;
c_phys* csgo::i::phys = nullptr;
c_localize* csgo::i::localize = nullptr;
c_engine_trace* csgo::i::trace = nullptr;
c_glow_obj_mgr* csgo::i::glow_obj_mgr = nullptr;
c_clientstate* csgo::i::client_state = nullptr;
c_mem_alloc* csgo::i::mem_alloc = nullptr;
c_prediction* csgo::i::pred = nullptr;
c_move_helper* csgo::i::move_helper = nullptr;
c_movement* csgo::i::move = nullptr;
c_panel* csgo::i::panel = nullptr;
mdl_cache_t* csgo::i::mdl_cache = nullptr;
c_input* csgo::i::input = nullptr;
c_convar* csgo::i::cvar = nullptr;
c_game_event_mgr* csgo::i::events = nullptr;
IDirect3DDevice9* csgo::i::dev = nullptr;
c_hud_chat* csgo::i::hud_chat = nullptr;
c_input_system* csgo::i::input_sys = nullptr;

c_mdl_cache_critical_section::c_mdl_cache_critical_section( ) {
	csgo::i::mdl_cache->begin_lock( );
}

c_mdl_cache_critical_section::~c_mdl_cache_critical_section( ) {
	csgo::i::mdl_cache->end_lock( );
}

bool csgo::render::screen_transform( vec3_t& screen, vec3_t& origin ) {
	static auto view_matrix = pattern::search( client_dll, _( "0F 10 05 ? ? ? ? 8D 85 ? ? ? ? B9" ) ).add( 3 ).deref( ).add( 176 ).get< std::uintptr_t >( );
	const auto& world_matrix = *( matrix3x4_t* ) view_matrix;

	screen.x = world_matrix[0][0] * origin.x + world_matrix[0][1] * origin.y + world_matrix[0][2] * origin.z + world_matrix[0][3];
	screen.y = world_matrix[1][0] * origin.x + world_matrix[1][1] * origin.y + world_matrix[1][2] * origin.z + world_matrix[1][3];
	screen.z = 0.0f;

	const auto w = world_matrix[3][0] * origin.x + world_matrix[3][1] * origin.y + world_matrix[3][2] * origin.z + world_matrix[3][3];

	if ( w < 0.001f ) {
		screen.x *= 100000.0f;
		screen.y *= 100000.0f;

		return true;
	}

	screen.x *= 1.0f / w;
	screen.y *= 1.0f / w;

	return false;
}

bool csgo::render::world_to_screen( vec3_t& screen, vec3_t& origin ) {
	if ( !screen_transform( screen, origin ) ) {
		int width, height;
		csgo::i::engine->get_screen_size( width, height ); // TODO: use the one we calculate in sceneend instead

		screen.x = ( width * 0.5f ) + ( screen.x * width ) * 0.5f;
		screen.y = ( height * 0.5f ) - ( screen.y * height ) * 0.5f;

		return true;
	}

	return false;
}

void csgo::util::trace_line( const vec3_t& start, const vec3_t& end, std::uint32_t mask, const entity_t* ignore, trace_t* ptr ) {
	trace_filter_t filter( ( void* ) ignore );

	ray_t ray;
	ray.init( start, end );

	csgo::i::trace->trace_ray( ray, mask, &filter, ptr );

	/*
	using fn = void( __fastcall* )( const vec3_t&, const vec3_t&, std::uint32_t, const entity_t*, std::uint32_t, trace_t* );
	static auto util_traceline = pattern::search( STR( client_dll ), STR( "55 8B EC 83 E4 F0 83 EC 7C 56 52" ) ).get< fn >( );
	util_traceline( start, end, mask, ignore, 0, ptr );
	*/
}

void csgo::util::clip_trace_to_players( const vec3_t& start, const vec3_t& end, std::uint32_t mask, trace_filter_t* filter, trace_t* trace_ptr ) {
	static auto util_cliptracetoplayers = pattern::search( client_dll, _( "53 8B DC 83 EC 08 83 E4 F0 83 C4 04 55 8B 6B 04 89 6C 24 04 8B EC 81 EC D8 ? ? ? 0F 57 C9" ) ).get< std::uint32_t >( );

	if ( !util_cliptracetoplayers )
		return;

	/* changed from ptr to trace_ptr cuz assmebly */
	__asm {
		mov	eax, filter
		lea ecx, trace_ptr
		push ecx
		push eax
		push mask
		lea edx, end
		lea ecx, start
		call util_cliptracetoplayers
		add esp, 0xC
	}

	/*
	using fn = void( __fastcall* )( const vec3_t&, const vec3_t&, std::uint32_t, trace_filter_t*, trace_t* );
	static auto util_cliptracetoplayers = pattern::search( STR( client_dll ), STR( "53 8B DC 83 EC 08 83 E4 F0 83 C4 04 55 8B 6B 04 89 6C 24 04 8B EC 81 EC D8 ? ? ? 0F 57 C9" ) ).get< fn >( );
	util_cliptracetoplayers( start, end, mask, filter, ptr );
	*/
}

void csgo::for_each_player( std::function< void( player_t* ) > fn ) {
	for ( auto i = 1; i <= i::globals->m_max_clients; i++ ) {
		auto entity = i::ent_list->get< player_t* >( i );

		if ( !entity->valid( ) )
			continue;

		fn( entity );
	}
}

void csgo::for_each_entity( std::function< void( player_t* ) > fn ) {
	for ( auto i = 0; i <= i::ent_list->get_highest_entity_index( ); i++ ) {
		auto entity = i::ent_list->get< player_t* >( i );

		fn( entity );
	}
}

float csgo::normalize( float ang ) {
	return std::remainderf( ang, 360.0f );
}

void csgo::clamp( vec3_t& ang ) {
	auto flt_valid = [] ( float val ) {
		return std::isfinite( val ) && !std::isnan( val );
	};

	for ( auto i = 0; i < 3; i++ )
		if ( !flt_valid( ang[i] ) )
			return;

	ang.x = std::clamp( normalize( ang.x ), -89.0f, 89.0f );
	ang.y = std::clamp( normalize( ang.y ), -180.0f, 180.0f );
	ang.z = 0.0f;
}

float csgo::rad2deg( float rad ) {
	float result = rad * ( 180.0f / pi );
	return result;
}

float csgo::deg2rad( float deg ) {
	float result = deg * ( pi / 180.0f );
	return result;
}

void csgo::sin_cos( float radians, float* sine, float* cosine ) {
	*sine = std::sinf( radians );
	*cosine = std::cosf( radians );
}

vec3_t csgo::calc_angle( const vec3_t& from, const vec3_t& to ) {
	return csgo::vec_angle( to - from );
}

vec3_t csgo::calc_angle( const vec3_t& from, const vec3_t& to, const vec3_t& ang ) {
	vec3_t delta = from - to;
	auto radians_to_degrees = [] ( float radians ) { return radians * 180 / static_cast< float >( pi ); };
	vec3_t angles;
	angles.x = radians_to_degrees( atanf( delta.z / std::hypotf( delta.x, delta.y ) ) ) - ang.x;
	angles.y = radians_to_degrees( atanf( delta.y / delta.x ) ) - ang.y;
	angles.z = 0.0f;

	if ( delta.x >= 0.0 )
		angles.y += 180.0f;

	angles.normalize_angles( );
	return angles;
}

vec3_t csgo::vec_angle( vec3_t vec ) {
	vec3_t ret;

	if ( vec.x == 0.0f && vec.y == 0.0f ) {
		ret.x = ( vec.z > 0.0f ) ? 270.0f : 90.0f;
		ret.y = 0.0f;
	}
	else {
		ret.x = rad2deg( std::atan2f( -vec.z, vec.length_2d( ) ) );
		ret.y = rad2deg( std::atan2f( vec.y, vec.x ) );

		if ( ret.y < 0.0f )
			ret.y += 360.0f;

		if ( ret.x < 0.0f )
			ret.x += 360.0f;
	}

	ret.z = 0.0f;

	return ret;
}

vec3_t csgo::angle_vec( vec3_t angle ) {
	vec3_t ret;

	float sp, sy, cp, cy;

	sin_cos( deg2rad( angle.y ), &sy, &cy );
	sin_cos( deg2rad( angle.x ), &sp, &cp );

	ret.x = cp * cy;
	ret.y = cp * sy;
	ret.z = -sp;

	return ret;
}

void csgo::util_traceline( const vec3_t& start, const vec3_t& end, unsigned int mask, const void* ignore, trace_t* tr ) {
	using fn = void( __fastcall* )( const vec3_t&, const vec3_t&, std::uint32_t, const void*, std::uint32_t, trace_t* );
	static auto utl = pattern::search( client_dll, _( "55 8B EC 83 E4 F0 83 EC 7C 56 52" ) ).get< fn >( );
	utl( start, end, mask, ignore, 0, tr );
}

void csgo::util_tracehull( const vec3_t& start, const vec3_t& end, const vec3_t& mins, const vec3_t& maxs, unsigned int mask, const void* ignore, trace_t* tr ) {
	using fn = void( __fastcall* )( const vec3_t&, const vec3_t&, const vec3_t&, const vec3_t&, unsigned int, const void*, std::uint32_t, trace_t* );
	static auto utl = pattern::search( client_dll, _( "E8 ? ? ? ? 8B 07 83 C4 20" ) ).resolve_rip( ).get< fn >( );
	utl( start, end, mins, maxs, mask, ignore, 0, tr );
}

bool csgo::line_goes_through_smoke( const vec3_t& start, const vec3_t& end, const bool bloat ) {
	using fn = bool( __cdecl* )( vec3_t, vec3_t, bool );
	static auto line_goes_through_smoke = pattern::search( client_dll, _( "55 8B EC 83 EC 08 8B 15 ? ? ? ? 0F 57 C0" ) ).get<fn>( );
	return line_goes_through_smoke( start, end, bloat );
}

template < typename t >
t csgo::create_interface( const char* module, const char* iname ) {
	using createinterface_fn = void* ( __cdecl* )( const char*, int );
	const auto createinterface_export = GetProcAddress( GetModuleHandleA( module ), _( "CreateInterface" ) );
	const auto fn = ( createinterface_fn ) createinterface_export;

	return reinterpret_cast< t >( fn( iname, 0 ) );
}

void csgo::rotate_movement( ucmd_t* ucmd ) {
	vec3_t ang;
	csgo::i::engine->get_viewangles( ang );

	const auto vec_move = vec3_t( ucmd->m_fmove, ucmd->m_smove, ucmd->m_umove );
	const auto ang_move = csgo::vec_angle( vec_move );
	const auto speed = vec_move.length_2d( );
	const auto rotation = deg2rad( ucmd->m_angs.y - ang.y + ang_move.y );

	ucmd->m_fmove = std::cosf( rotation ) * speed;
	ucmd->m_smove = std::sinf( rotation ) * speed;
}

void csgo::fix_movement( ucmd_t* ucmd ) {
	if ( ucmd->m_fmove ) {
		ucmd->m_buttons &= ~( ucmd->m_fmove < 0.0f ? 8 : 16 );
		ucmd->m_buttons |= ( ucmd->m_fmove > 0.0f ? 8 : 16 );
	}
	if ( ucmd->m_smove ) {
		ucmd->m_buttons &= ~( ucmd->m_smove < 0.0f ? 1024 : 512 );
		ucmd->m_buttons |= ( ucmd->m_smove > 0.0f ? 1024 : 512 );
	}
}

template<class T>
static T* csgo::find_hud_elem_t(const char* name)
{
	static auto pThis = pattern::search(client_dll, _("B9 ? ? ? ? 68 ? ? ? ? E8 ? ? ? ? 89 46 24")).add(1).deref().get< std::uintptr_t* >();

	static auto find_hud_element = pattern::search(client_dll, _("55 8B EC 53 8B 5D 08 56 57 8B F9 33 F6 39 77 28")).get< std::uintptr_t* (__thiscall*)(std::uintptr_t*, const char*) >();
	return (T*)find_hud_element(pThis, name);
}

std::uintptr_t* csgo::find_hud_element( const char* name ) {
	static auto hud = pattern::search( client_dll, _( "B9 ? ? ? ? 68 ? ? ? ? E8 ? ? ? ? 89 46 24" ) ).add( 1 ).deref( ).get< std::uintptr_t* >( );
	static auto find_hud_element_func = pattern::search( client_dll, _( "55 8B EC 53 8B 5D 08 56 57 8B F9 33 F6 39 77 28" ) ).get< std::uintptr_t* ( __thiscall* )( std::uintptr_t*, const char* ) >( );

	return ( std::uintptr_t* ) find_hud_element_func( hud, name );
}

void csgo::force_full_update( ) {
	using fn = int( __thiscall* )( void*, int );
	static auto o_clear_hud_weapon_icon = pattern::search( client_dll, _( "55 8B EC 51 53 56 8B 75 08 8B D9 57 6B" ) ).get<fn>( );

	if ( o_clear_hud_weapon_icon != nullptr ) {
		if ( const auto m_hud_weapons = csgo::find_hud_element( _( "CCSGO_HudWeaponSelection" ) ) - 0x28; m_hud_weapons != nullptr ) {
			for ( std::size_t i = 0; i < *( m_hud_weapons + 0x20 ); i++ )
				i = o_clear_hud_weapon_icon( m_hud_weapons, i );
		}
	}

	csgo::i::client_state->delta_tick( ) = -1;
}

bool csgo::init( ) {
	i::globals = pattern::search( client_dll, _( "A1 ? ? ? ? F3 0F 10 8F ? ? ? ? F3 0F 10 05 ? ? ? ? ? ? ? ? ? 0F 2F C1 0F 86" ) ).add( 1 ).deref( ).deref( ).get< c_globals* >( );
	i::ent_list = create_interface< c_entlist* >( client_dll, _( "VClientEntityList003" ) );
	i::mat_sys = create_interface< c_matsys* >( materialsystem_dll, _( "VMaterialSystem080" ) );
	i::mdl_info = create_interface< c_mdlinfo* >( engine_dll, _( "VModelInfoClient004" ) );
	i::mdl_render = create_interface< c_mdlrender* >( engine_dll, _( "VEngineModel016" ) );
	i::render_view = create_interface< c_renderview* >( engine_dll, _( "VEngineRenderView014" ) );
	i::client = create_interface< c_client* >( client_dll, _( "VClient018" ) );
	i::panel = create_interface< c_panel* >( vgui2_dll, _( "VGUI_Panel009" ) );
	i::surface = create_interface< c_surface* >( surface_dll, _( "VGUI_Surface031" ) );
	i::engine = create_interface< c_engine* >( engine_dll, _( "VEngineClient014" ) );
	i::phys = create_interface< c_phys* >( vphysics_dll, _( "VPhysicsSurfaceProps001" ) );
	i::trace = create_interface< c_engine_trace* >( engine_dll, _( "EngineTraceClient004" ) );
	i::pred = create_interface< c_prediction* >( client_dll, _( "VClientPrediction001" ) );
	i::move = create_interface< c_movement* >( client_dll, _( "GameMovement001" ) );
	i::mdl_cache = create_interface< mdl_cache_t* >( client_dll, _( "MDLCache004" ) );
	i::events = create_interface< c_game_event_mgr* >( engine_dll, _( "GAMEEVENTSMANAGER002" ) );
	i::localize = create_interface< c_localize* >(localize_dll, _("Localize_001"));
	i::input_sys = create_interface< c_input_system* >(inputsystem_dll, _("InputSystemVersion001"));
	i::input = pattern::search( client_dll, "B9 ? ? ? ? FF 60 60" ).add( 1 ).deref( ).get< c_input* >( );
	i::cvar = create_interface< c_convar* >( vstdlib_dll, _( "VEngineCvar007" ) );
	i::move_helper = **reinterpret_cast< c_move_helper*** >( pattern::search( client_dll, _( "8B 0D ? ? ? ? 8B 45 ? 51 8B D4 89 02 8B 01" ) ).add( 2 ).get< std::uintptr_t >( ) );
	i::client_state = **reinterpret_cast< c_clientstate*** >( reinterpret_cast< std::uintptr_t >( vfunc< void* >( i::engine, 12 ) ) + 16 );
	i::mem_alloc = *( c_mem_alloc** ) GetProcAddress( GetModuleHandleA( tier0_dll ), _( "g_pMemAlloc" ) );
	i::dev = pattern::search( shaderapidx9_dll, _( "A1 ? ? ? ? 50 8B 08 FF 51 0C" ) ).add( 1 ).deref( ).deref( ).get< IDirect3DDevice9* >( );
	i::glow_obj_mgr = pattern::search(client_dll, _("0F 11 05 ? ? ? ? 83 C8 01")).add(3).deref().get< c_glow_obj_mgr* >();
	i::hud_chat = csgo::find_hud_elem_t<c_hud_chat>("CHudChat");

	return true;
}