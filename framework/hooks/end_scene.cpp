#include "end_scene.hpp"

decltype( &hooks::end_scene ) hooks::original::end_scene = nullptr;

long __fastcall hooks::end_scene( void * ecx , void * edx , IDirect3DDevice9 * device ) {
	static auto ret = _ReturnAddress( );
	if ( ret != _ReturnAddress( ) )
		return original::end_scene( ecx , edx , device );

	IDirect3DStateBlock9 * pixel_state = nullptr;
	IDirect3DVertexDeclaration9 * vertex_decleration = nullptr;
	IDirect3DVertexShader9 * vertex_shader = nullptr;

	device->CreateStateBlock( D3DSBT_PIXELSTATE , &pixel_state );
	device->GetVertexDeclaration( &vertex_decleration );
	device->GetVertexShader( &vertex_shader );

	pixel_state->Apply( );
	pixel_state->Release( );

	device->SetVertexDeclaration( vertex_decleration );
	device->SetVertexShader( vertex_shader );

	return original::end_scene( ecx , edx , device );
}