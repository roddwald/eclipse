#pragma once
#include <cmath>

struct color {
	color( ) = default;
	color( int r, int g, int b, int a ) {
		this->r = r;
		this->g = g;
		this->b = b;
		this->a = a;
	}

	color( int r, int g, int b ) {
		this->r = r;
		this->g = g;
		this->b = b;
		this->a = 255;
	}

	color( color col, int a ) {
		this->r = col.r;
		this->g = col.g;
		this->b = col.b;
		this->a = a;
	}

	static color hsl_to_rgb( float h, float s, float l ) {
		float q;

		if ( l < 0.5f )
			q = l * ( s + 1.f );

		else
			q = l + s - ( l * s );

		float p = 2 * l - q;

		float rgb[3];
		rgb[0] = h + ( 1.f / 3.f );
		rgb[1] = h;
		rgb[2] = h - ( 1.f / 3.f );

		for ( int i = 0; i < 3; ++i ) {
			if ( rgb[i] < 0 )
				rgb[i] += 1.f;

			if ( rgb[i] > 1 )
				rgb[i] -= 1.f;

			if ( rgb[i] < ( 1.f / 6.f ) )
				rgb[i] = p + ( ( q - p ) * 6 * rgb[i] );
			else if ( rgb[i] < 0.5f )
				rgb[i] = q;
			else if ( rgb[i] < ( 2.f / 3.f ) )
				rgb[i] = p + ( ( q - p ) * 6 * ( ( 2.f / 3.f ) - rgb[i] ) );
			else
				rgb[i] = p;
		}

		return {
			int( rgb[0] * 255.f ),
			int( rgb[1] * 255.f ),
			int( rgb[2] * 255.f ),
			int( 1.0f )
		};
	}

	int r, g, b, a;
};