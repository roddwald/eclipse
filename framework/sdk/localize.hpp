#pragma once
#include "../utils/vfunc.hpp"

class c_localize {
public:
	wchar_t* find( const char* token_name ) {
		return call_vfunc< wchar_t*>( this, 11, token_name );
	}

	const wchar_t* find_safe( const char* token_name ) {
		return call_vfunc< wchar_t*>( this, 12, token_name );
	}
};