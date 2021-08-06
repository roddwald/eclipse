#include "d3d9.hpp"
#include "../sdk/sdk.hpp"

struct vtx_t {
	float x, y, z, rhw;
	std::uint32_t color;
};

struct custom_vtx_t {
	float x, y, z, rhw;
	std::uint32_t color;
	float tu, tv;
};

void render::create_font( void** font, const std::wstring_view& family, int size, bool bold ) {
	ID3DXFont* d3d_font = nullptr;
	D3DXCreateFontW( csgo::i::dev, size, 0, bold ? FW_BOLD : FW_LIGHT, 1, false, DEFAULT_CHARSET, OUT_DEFAULT_PRECIS, 0, DEFAULT_PITCH | FF_DONTCARE, family.data( ), &d3d_font );
	*font = d3d_font;
}

void render::screen_size( int& width, int& height ) {
	csgo::i::engine->get_screen_size( width, height );
}

void render::text_size( void* font, const std::wstring_view& text, dim& dimentions ) {
	RECT rect = { 0, 0, 0, 0 };
	reinterpret_cast< ID3DXFont* >( font )->DrawTextW( nullptr, text.data( ), text.length( ), &rect, DT_CALCRECT, D3DCOLOR_RGBA( 0, 0, 0, 0 ) );
	dimentions = dim { rect.right - rect.left, rect.bottom - rect.top };
}

void render::rectangle( int x, int y, int width, int height, std::uint32_t color ) {
	vtx_t vert [ 4 ] = {
		{ x - 0.5f, y - 0.5f, 0.0f, 1.0f, color },
		{ x + width - 0.5f, y - 0.5f, 0.0f, 1.0f, color },
		{ x - 0.5f, y + height - 0.5f, 0.0f, 1.0f, color },
		{ x + width - 0.5f, y + height - 0.5f, 0.0f, 1.0f, color }
	};

	csgo::i::dev->SetRenderState( D3DRS_ALPHABLENDENABLE, true );
	csgo::i::dev->SetTexture( 0, nullptr );
	csgo::i::dev->SetPixelShader( nullptr );
	csgo::i::dev->SetRenderState( D3DRS_SRCBLEND, D3DBLEND_SRCALPHA );
	csgo::i::dev->SetRenderState( D3DRS_DESTBLEND, D3DBLEND_INVSRCALPHA );
	csgo::i::dev->SetFVF( D3DFVF_XYZRHW | D3DFVF_DIFFUSE );
	csgo::i::dev->DrawPrimitiveUP( D3DPT_TRIANGLESTRIP, 2, &vert, sizeof( vtx_t ) );
}

void render::gradient( int x, int y, int width, int height, std::uint32_t color1, std::uint32_t color2, bool is_horizontal ) {
	const auto d3d_clr1 = color1;
	const auto d3d_clr2 = color2;
	std::uint32_t c1, c2, c3, c4;

	if ( is_horizontal ) {
		c1 = d3d_clr1;
		c2 = d3d_clr2;
		c3 = d3d_clr1;
		c4 = d3d_clr2;
	}
	else {
		c1 = d3d_clr1;
		c2 = d3d_clr1;
		c3 = d3d_clr2;
		c4 = d3d_clr2;
	}


	vtx_t vert [ 4 ] = {
		{ x - 0.5f, y - 0.5f, 0.0f, 1.0f, c1 },
		{ x + width - 0.5f, y - 0.5f, 0.0f, 1.0f, c2 },
		{ x - 0.5f, y + height - 0.5f, 0.0f, 1.0f, c3 },
		{ x + width - 0.5f, y + height - 0.5f, 0.0f, 1.0f, c4 }
	};

	csgo::i::dev->SetRenderState( D3DRS_ALPHABLENDENABLE, true );
	csgo::i::dev->SetTexture( 0, nullptr );
	csgo::i::dev->SetPixelShader( nullptr );
	csgo::i::dev->SetRenderState( D3DRS_SRCBLEND, D3DBLEND_SRCALPHA );
	csgo::i::dev->SetRenderState( D3DRS_DESTBLEND, D3DBLEND_INVSRCALPHA );
	csgo::i::dev->SetFVF( D3DFVF_XYZRHW | D3DFVF_DIFFUSE );
	csgo::i::dev->DrawPrimitiveUP( D3DPT_TRIANGLESTRIP, 2, &vert, sizeof( vtx_t ) );
}

void render::outline( int x, int y, int width, int height, std::uint32_t color ) {
	rectangle( x, y, width, 1, color );
	rectangle( x, y + height, width, 1, color );
	rectangle( x, y, 1, height, color );
	rectangle( x + width, y, 1, height + 1, color );
}

void render::line( int x, int y, int x2, int y2, std::uint32_t color ) {
	vtx_t vert [ 2 ] = {
		{ x - 0.5f, y - 0.5f, 0.0f, 1.0f, color },
		{ x2 - 0.5f, y2 - 0.5f, 0.0f, 1.0f, color }
	};

	csgo::i::dev->SetRenderState( D3DRS_ALPHABLENDENABLE, true );
	csgo::i::dev->SetTexture( 0, nullptr );
	csgo::i::dev->SetPixelShader( nullptr );
	csgo::i::dev->SetRenderState( D3DRS_SRCBLEND, D3DBLEND_SRCALPHA );
	csgo::i::dev->SetRenderState( D3DRS_DESTBLEND, D3DBLEND_INVSRCALPHA );
	csgo::i::dev->SetFVF( D3DFVF_XYZRHW | D3DFVF_DIFFUSE );
	csgo::i::dev->DrawPrimitiveUP( D3DPT_LINELIST, 1, &vert, sizeof( vtx_t ) );
}

void render::text( int x, int y, std::uint32_t color, void* font, const std::wstring_view& text ) {
	RECT rect;
	SetRect( &rect, x, y, x, y );
	reinterpret_cast< ID3DXFont* >( font )->DrawTextW( nullptr, text.data( ), text.length( ), &rect, DT_LEFT | DT_NOCLIP, color );
}

void render::circle( int x, int y, int radius, int segments, std::uint32_t color ) {
	std::vector< vtx_t > circle( segments + 2 );

	const auto angle = 360.0f * D3DX_PI / 180.0f;

	circle [ 0 ].x = x - 0.5f;
	circle [ 0 ].y = y - 0.5f;
	circle [ 0 ].z = 0;
	circle [ 0 ].rhw = 1;
	circle [ 0 ].color = color;

	for ( auto i = 1; i < segments + 2; i++ ) {
		circle [ i ].x = ( float ) ( x - radius * std::cosf( D3DX_PI * ( ( i - 1 ) / ( segments / 2.0f ) ) ) ) - 0.5f;
		circle [ i ].y = ( float ) ( y - radius * std::sinf( D3DX_PI * ( ( i - 1 ) / ( segments / 2.0f ) ) ) ) - 0.5f;
		circle [ i ].z = 0;
		circle [ i ].rhw = 1;
		circle [ i ].color = color;
	}

	const auto _res = segments + 2;

	for ( auto i = 0; i < _res; i++ ) {
		circle [ i ].x = x + std::cosf( angle ) * ( circle [ i ].x - x ) - std::sinf( angle ) * ( circle [ i ].y - y ) - 0.5f;
		circle [ i ].y = y + std::sinf( angle ) * ( circle [ i ].x - x ) + std::cosf( angle ) * ( circle [ i ].y - y ) - 0.5f;
	}

	IDirect3DVertexBuffer9* vb = nullptr;

	csgo::i::dev->CreateVertexBuffer( ( segments + 2 ) * sizeof( vtx_t ), D3DUSAGE_WRITEONLY, D3DFVF_XYZRHW | D3DFVF_DIFFUSE, D3DPOOL_DEFAULT, &vb, nullptr );

	void* verticies;
	vb->Lock( 0, ( segments + 2 ) * sizeof( vtx_t ), ( void** ) &verticies, 0 );
	std::memcpy( verticies, &circle [ 0 ], ( segments + 2 ) * sizeof( vtx_t ) );
	vb->Unlock( );

	csgo::i::dev->SetTexture( 0, nullptr );
	csgo::i::dev->SetPixelShader( nullptr );
	csgo::i::dev->SetRenderState( D3DRS_ALPHABLENDENABLE, true );
	csgo::i::dev->SetRenderState( D3DRS_SRCBLEND, D3DBLEND_SRCALPHA );
	csgo::i::dev->SetRenderState( D3DRS_DESTBLEND, D3DBLEND_INVSRCALPHA );
	csgo::i::dev->SetStreamSource( 0, vb, 0, sizeof( vtx_t ) );
	csgo::i::dev->SetFVF( D3DFVF_XYZRHW | D3DFVF_DIFFUSE );
	csgo::i::dev->DrawPrimitive( D3DPT_TRIANGLEFAN, 0, segments );

	if ( vb )
		vb->Release( );
}

void render::texture( unsigned char* data, int x, int y, int width, int height, float scale ) {
	D3DXVECTOR2 center = D3DXVECTOR2( width / 2, height / 2 );
	D3DXVECTOR2 trans = D3DXVECTOR2( x, y );
	D3DXMATRIX mat;
	D3DXVECTOR2 scaling( scale, scale );

	D3DXMatrixTransformation2D( &mat, nullptr, 0.0f, &scaling, &center, 0.0f, &trans );

	ID3DXSprite* sprite = nullptr;
	D3DXCreateSprite( csgo::i::dev, &sprite );

	IDirect3DTexture9* tex = nullptr;
	D3DXCreateTextureFromFileInMemory( csgo::i::dev, data, 4 * width * height, &tex );

	csgo::i::dev->SetRenderState( D3DRS_ALPHABLENDENABLE, true );
	csgo::i::dev->SetRenderState( D3DRS_SRCBLEND, D3DBLEND_SRCALPHA );
	csgo::i::dev->SetRenderState( D3DRS_DESTBLEND, D3DBLEND_INVSRCALPHA );
	csgo::i::dev->SetTextureStageState( 0, D3DTSS_COLOROP, D3DTOP_SELECTARG1 );
	csgo::i::dev->SetTextureStageState( 0, D3DTSS_COLORARG1, D3DTA_TEXTURE );
	csgo::i::dev->SetTextureStageState( 0, D3DTSS_COLORARG2, D3DTA_DIFFUSE );
	csgo::i::dev->SetFVF( D3DFVF_XYZRHW | D3DFVF_DIFFUSE | D3DFVF_TEX1 );
	csgo::i::dev->SetPixelShader( nullptr );

	sprite->Begin( 0 );
	sprite->SetTransform( &mat );
	sprite->Draw( tex, nullptr, nullptr, nullptr, 0xFFFFFFFF );
	sprite->End( );
	sprite->Release( );
}

void render::clip_rect( int x, int y, int width, int height ) {
	RECT rect { x - 0.5f, y - 0.5f, x + width - 0.5f, y + height - 0.5f };
	csgo::i::dev->SetScissorRect( &rect );
}

bool render::key_pressed( const std::uint32_t key ) {
	return GetAsyncKeyState( key );
}

void render::mouse_pos( pos& position ) {
	int x, y;
	csgo::i::surface->get_cursor_pos( x, y );
	position = pos { x, y };
}