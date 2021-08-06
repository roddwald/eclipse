#include "reset.hpp"

decltype( &hooks::reset ) hooks::original::reset = nullptr;

long __fastcall hooks::reset( void * ecx , void * edx , IDirect3DDevice9 * device , D3DPRESENT_PARAMETERS * presentation_params ) {
	auto hr = original::reset( ecx , edx , device , presentation_params );

	if ( SUCCEEDED( hr ) ) {
	}

	return hr;
}