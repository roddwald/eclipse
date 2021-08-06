#pragma once
#include "../utils/vfunc.hpp"
#include "../utils/padding.hpp"

#define TEXTURE_GROUP_LIGHTMAP						        "Lightmaps"
#define TEXTURE_GROUP_WORLD							          "World textures"
#define TEXTURE_GROUP_MODEL							          "Model textures"
#define TEXTURE_GROUP_VGUI							          "VGUI textures"
#define TEXTURE_GROUP_PARTICLE						        "Particle textures"
#define TEXTURE_GROUP_DECAL							          "Decal textures"
#define TEXTURE_GROUP_SKYBOX						          "SkyBox textures"
#define TEXTURE_GROUP_CLIENT_EFFECTS				      "ClientEffect textures"
#define TEXTURE_GROUP_OTHER							          "Other textures"
#define TEXTURE_GROUP_PRECACHED						        "Precached"
#define TEXTURE_GROUP_CUBE_MAP						        "CubeMap textures"
#define TEXTURE_GROUP_RENDER_TARGET					      "RenderTargets"
#define TEXTURE_GROUP_UNACCOUNTED					        "Unaccounted textures"
//#define TEXTURE_GROUP_STATIC_VERTEX_BUFFER		  "Static Vertex"
#define TEXTURE_GROUP_STATIC_INDEX_BUFFER			    "Static Indices"
#define TEXTURE_GROUP_STATIC_VERTEX_BUFFER_DISP		"Displacement Verts"
#define TEXTURE_GROUP_STATIC_VERTEX_BUFFER_COLOR	"Lighting Verts"
#define TEXTURE_GROUP_STATIC_VERTEX_BUFFER_WORLD	"World Verts"
#define TEXTURE_GROUP_STATIC_VERTEX_BUFFER_MODELS	"Model Verts"
#define TEXTURE_GROUP_STATIC_VERTEX_BUFFER_OTHER	"Other Verts"
#define TEXTURE_GROUP_DYNAMIC_INDEX_BUFFER			  "Dynamic Indices"
#define TEXTURE_GROUP_DYNAMIC_VERTEX_BUFFER			  "Dynamic Verts"
#define TEXTURE_GROUP_DEPTH_BUFFER					      "DepthBuffer"
#define TEXTURE_GROUP_VIEW_MODEL					        "ViewModel"
#define TEXTURE_GROUP_PIXEL_SHADERS					      "Pixel Shaders"
#define TEXTURE_GROUP_VERTEX_SHADERS				      "Vertex Shaders"
#define TEXTURE_GROUP_RENDER_TARGET_SURFACE			  "RenderTarget Surfaces"
#define TEXTURE_GROUP_MORPH_TARGETS					      "Morph Targets"

using material_handle_t = std::uint16_t;

enum MaterialVarFlags_t {
	MATERIAL_VAR_DEBUG = ( 1 << 0 ),
	MATERIAL_VAR_NO_DEBUG_OVERRIDE = ( 1 << 1 ),
	MATERIAL_VAR_NO_DRAW = ( 1 << 2 ),
	MATERIAL_VAR_USE_IN_FILLRATE_MODE = ( 1 << 3 ),
	MATERIAL_VAR_VERTEXCOLOR = ( 1 << 4 ),
	MATERIAL_VAR_VERTEXALPHA = ( 1 << 5 ),
	MATERIAL_VAR_SELFILLUM = ( 1 << 6 ),
	MATERIAL_VAR_ADDITIVE = ( 1 << 7 ),
	MATERIAL_VAR_ZNEARER = ( 1 << 10 ),
	MATERIAL_VAR_MODEL = ( 1 << 11 ),
	MATERIAL_VAR_FLAT = ( 1 << 12 ),
	MATERIAL_VAR_NOCULL = ( 1 << 13 ),
	MATERIAL_VAR_NOFOG = ( 1 << 14 ),
	MATERIAL_VAR_IGNOREZ = ( 1 << 15 ),
	MATERIAL_VAR_DECAL = ( 1 << 16 ),
	MATERIAL_VAR_ENVMAPSPHERE = ( 1 << 17 ), // OBSOLETE
	MATERIAL_VAR_ENVMAPCAMERASPACE = ( 1 << 19 ), // OBSOLETE
	MATERIAL_VAR_BASEALPHAENVMAPMASK = ( 1 << 20 ),
	MATERIAL_VAR_TRANSLUCENT = ( 1 << 21 ),
	MATERIAL_VAR_NORMALMAPALPHAENVMAPMASK = ( 1 << 22 ),
	MATERIAL_VAR_NEEDS_SOFTWARE_SKINNING = ( 1 << 23 ), // OBSOLETE
	MATERIAL_VAR_OPAQUETEXTURE = ( 1 << 24 ),
	MATERIAL_VAR_ENVMAPMODE = ( 1 << 25 ), // OBSOLETE
	MATERIAL_VAR_SUPPRESS_DECALS = ( 1 << 26 ),
	MATERIAL_VAR_HALFLAMBERT = ( 1 << 27 ),
	MATERIAL_VAR_WIREFRAME = ( 1 << 28 ),
	MATERIAL_VAR_ALLOWALPHATOCOVERAGE = ( 1 << 29 ),
	MATERIAL_VAR_ALPHA_MODIFIED_BY_PROXY = ( 1 << 30 ),
	MATERIAL_VAR_VERTEXFOG = ( 1 << 31 ),
};

class material_var_t {
public:
	void set_float( const float val ) {
		using o_fn = void( __thiscall* )( void*, float );
		return vfunc< o_fn >( this, 4 )( this, val );
	}

	void set_int( const int val ) {
		using o_fn = void( __thiscall* )( void*, int );
		return vfunc< o_fn >( this, 5 )( this, val );
	}

	void set_int( const char* val ) {
		using o_fn = void( __thiscall* )( void*, const char* );
		return vfunc< o_fn >( this, 6 )( this, val );
	}

	void set_vector( const float x, const float y, const float z ) {
		using o_fn = void( __thiscall* )( void*, float, float, float );
		return vfunc< o_fn >( this, 11 )( this, x, y, z );
	}
};

class material_t {
public:
	const char* get_name( void ) {
		using getname_fn = const char* ( __thiscall* )( void* );
		return vfunc< getname_fn >( this, 0 )( this );
	}

	const char* get_texture_group_name( void ) {
		using gettexturegroupname_fn = const char* ( __thiscall* )( void* );
		return vfunc< gettexturegroupname_fn >( this, 1 )( this );
	}

	void increment_reference_count( void ) {
		using increferencecount_fn = void( __thiscall* )( void* );
		vfunc< increferencecount_fn >( this, 12 )( this );
	}

	void* find_var( const char* var, bool* found, bool complain = true ) {
		using findvar_fn = void* ( __thiscall* )( void*, const char*, bool*, bool );
		return vfunc< findvar_fn >( this, 11 )( this, var, found, complain );
	}

	void color_modulate( int r, int g, int b ) {
		using colormodulate_fn = void( __thiscall* )( void*, float, float, float );
		vfunc< colormodulate_fn >( this, 28 )( this, static_cast< float >( r ) / 255.0f, static_cast< float >( g ) / 255.0f, static_cast< float >( b ) / 255.0f );
	}

	void alpha_modulate( int a ) {
		using colormodulate_fn = void( __thiscall* )( void*, float );
		vfunc< colormodulate_fn >( this, 27 )( this, static_cast< float >( a ) / 255.0f );
	}

	void set_material_var_flag( std::uint32_t flag, bool state ) {
		using setmaterialvarflag_fn = void( __thiscall* )( void*, std::uint32_t, bool );
		vfunc< setmaterialvarflag_fn >( this, 29 )( this, flag, state );
	}

	bool is_error_material( ) {
		using is_error_material_fn = bool( __thiscall* )( void* );
		return vfunc< is_error_material_fn >( this, 42 )( this );
	}
};

class c_matsys {
public:
	material_t* createmat( const char* name, void* kv ) {
		using creatematerial_fn = material_t * ( __thiscall* )( void*, const char*, void* );
		return vfunc< creatematerial_fn >( this, 83 )( this, name, kv );
	}

	material_t* findmat( char const* name, const char* group_name, bool complain = true, const char* complain_prefix = nullptr ) {
		using findmaterial_fn = material_t * ( __thiscall* )( void*, char const*, const char*, bool, const char* );
		return vfunc< findmaterial_fn >( this, 84 )( this, name, group_name, complain, complain_prefix );
	}

	material_t* get_material( int handle ) {
		using get_material_fn = material_t * ( __thiscall* )( void*, int );
		return vfunc< get_material_fn >( this, 89 )( this, handle );
	}

	int first_material( ) {
		using first_material_fn = int( __thiscall* )( void* );
		return vfunc< first_material_fn >( this, 86 )( this );
	}

	int next_material( int handle ) {
		using next_material_fn = int( __thiscall* )( void*, int );
		return vfunc< next_material_fn >( this, 87 )( this, handle );
	}

	int invalid_material( ) {
		using invalid_material_fn = int( __thiscall* )( void* );
		return vfunc< invalid_material_fn >( this, 88 )( this );
	}
};