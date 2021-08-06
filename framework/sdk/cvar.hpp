#pragma once
#include "..\utils\vfunc.hpp"

class c_cvar {
public:
	void set_value( const char* str ) {
		typedef void( __thiscall* fn )( void*, const char* );
		return vfunc<fn>( this, 13 )( this, str );
	}

	void set_value( float value ) {
		typedef void( __thiscall* fn )( void*, float );
		return vfunc<fn>( this, 15 )( this, value );
	}

	void set_value_char( const char* name ) {
		typedef void( __thiscall* fn )( void*, const char* );
		return vfunc<fn>( this, 14 )( this, name );
	}

	void set_value( int value ) {
		typedef void( __thiscall* fn )( void*, int );
		return vfunc<fn>( this, 16 )( this, value );
	}

	void internal_set_string( const char* str ) {
		typedef void( __thiscall* fn )( void*, const char* );
		return vfunc<fn>( this, 17 )( this, str );
	}

	char* get_base_name( ) {
		typedef char* ( __thiscall* fn )( void* );
		return vfunc<fn>( this, 6 )( this );
	}

	float get_float( ) {
		typedef float( __thiscall* fn )( void* );
		return vfunc< fn >( this, 12 )( this );
	}

	int get_int( void ) const {
		return m_parent->m_value;
	}

	const char* get_string( void ) const {
		return m_parent->m_default_value;
	}

	void no_callback( ) {
		*reinterpret_cast< int* >( reinterpret_cast< uintptr_t >( &m_change_callback ) + 0xc ) = 0;
	}

	char pad_0x0000[0x4];
	c_cvar* m_next;
	__int32 m_registered;
	char* m_name;
	char* m_help_string;
	__int32 m_flags;
	char pad_0x0018[0x4];
	c_cvar* m_parent;
	char* m_default_value;
	char* m_string;
	__int32 m_str_length;
	float m_float_value;
	__int32 m_value;
	__int32 m_has_min;
	float m_min_val;
	__int32 m_has_max;
	float m_max_val;
	void* m_change_callback;
};

class c_appsystem {
public:
	virtual ~c_appsystem( ) {
	}

	virtual void func0( ) = 0;
	virtual void func1( ) = 0;
	virtual void func2( ) = 0;
	virtual void func3( ) = 0;
	virtual void func4( ) = 0;
	virtual void func5( ) = 0;
	virtual void func6( ) = 0;
	virtual void func7( ) = 0;
	virtual void func8( ) = 0;
	virtual void func9( ) = 0;
};

struct cvar_dll_identifier_t;
class c_convar : public c_appsystem {
public:
	virtual void			func10( ) = 0;
	virtual void			register_con_commands( c_cvar* m_cmd_base ) = 0;
	virtual void			unregister_con_commands( c_cvar* m_cmd_base ) = 0;
	virtual void			func13( ) = 0;
	virtual c_cvar* find_var( const char* var_name ) = 0;
	virtual void			func15( ) = 0;
	virtual void			func16( ) = 0;
	virtual void			func17( ) = 0;
	virtual void			func18( ) = 0;
	virtual void			func19( ) = 0;
	virtual void			func20( ) = 0;
};