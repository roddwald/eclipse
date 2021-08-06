#pragma once
#include <cstdint>
#include <string_view>
#include <d3d9.h>
#include <d3dx9.h>

#pragma comment( lib, "d3d9.lib" )
#pragma comment( lib, "d3dx9.lib" )

namespace fonts {
	static void* def = nullptr;
}

constexpr uint32_t rgba( uint8_t r, uint8_t g, uint8_t b, uint8_t a ) {
	return ( ( a & 0xff ) << 24 ) | ( ( r & 0xff ) << 16 ) | ( ( g & 0xff ) << 8 ) | ( b & 0xff );
}

namespace render {
	struct vtx_t {
		float x, y, z, rhw;
		std::uint32_t color;
	};

	struct custom_vtx_t {
		float x, y, z, rhw;
		std::uint32_t color;
		float tu, tv;
	};

	struct pos {
		int x, y;
	};

	struct dim {
		int w, h;
	};

	struct rect {
		int x, y, w, h;
	};

	void create_font( void** font, const std::wstring_view& family, int size, bool bold );
	void screen_size( int& width, int& height );
	void text_size( void* font, const std::wstring_view& text, dim& dimentions );
	void rectangle( int x, int y, int width, int height, std::uint32_t color );
	void gradient( int x, int y, int width, int height, std::uint32_t color1, std::uint32_t color2, bool is_horizontal );
	void outline( int x, int y, int width, int height, std::uint32_t color );
	void line( int x, int y, int x2, int y2, std::uint32_t color );
	void text( int x, int y, std::uint32_t color, void* font, const std::wstring_view& text );
	void circle( int x, int y, int radius, int segments, std::uint32_t color );
	void texture( unsigned char* data, int x, int y, int width, int height, float scale );
	void clip_rect( int x, int y, int width, int height );
	bool key_pressed( const std::uint32_t key );
	void mouse_pos( pos& position );
}