#pragma once

#include "../sdk/sdk.hpp"
#include "lodepng/lodepng.h"
#include "../globals.hpp"

/* thanks to dusty for this modified texture holder class */
class c_textureholder {
public:
	c_textureholder( ) : m_surface( csgo::i::surface ), h( 0 ), w( 0 ), valid( false ) { };

	c_textureholder( const unsigned char* pRawRGBAData, int W, int H ) : m_surface( csgo::i::surface ), h( H ), w( W ), valid( false ) {
		tex = m_surface->create_new_texture_id( true );

		if ( !tex )
			return;

		m_surface->draw_set_texture_rgba( tex, pRawRGBAData, W, H );
		valid = true;
	};

	bool is_valid( ) const {
		return valid;
	};

	int get_texture_id( ) const {
		return tex;
	};

	bool draw( int x, int y, color col, float scale = 1.0 ) {
		if ( !m_surface->is_texture_id_valid( tex ) )
			return false;

		m_surface->draw_set_color( col.r, col.g, col.b, col.a );
		m_surface->draw_set_texture( tex );
		m_surface->draw_textured_rect( x, y, x + w * scale, y + h * scale );
		return true;
	};

	static std::uint8_t* decode_resource_image( unsigned short id, unsigned int w, unsigned int h );

protected:
	int tex;
	int w, h;
	bool valid;
	c_surface* m_surface;
};