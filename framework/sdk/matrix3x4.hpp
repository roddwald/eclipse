#pragma once
#include "vec3.hpp"

class matrix3x4_t {
public:
	float m_values[ 3 ][ 4 ];

	matrix3x4_t( void ) {
		m_values[ 0 ][ 0 ] = 0.0f; m_values[ 0 ][ 1 ] = 0.0f; m_values[ 0 ][ 2 ] = 0.0f; m_values[ 0 ][ 3 ] = 0.0f;
		m_values[ 1 ][ 0 ] = 0.0f; m_values[ 1 ][ 1 ] = 0.0f; m_values[ 1 ][ 2 ] = 0.0f; m_values[ 1 ][ 3 ] = 0.0f;
		m_values[ 2 ][ 0 ] = 0.0f; m_values[ 2 ][ 1 ] = 0.0f; m_values[ 2 ][ 2 ] = 0.0f; m_values[ 2 ][ 3 ] = 0.0f;
	}

	matrix3x4_t(
		float m00, float m01, float m02, float m03,
		float m10, float m11, float m12, float m13,
		float m20, float m21, float m22, float m23 ) {
		m_values[ 0 ][ 0 ] = m00; m_values[ 0 ][ 1 ] = m01; m_values[ 0 ][ 2 ] = m02; m_values[ 0 ][ 3 ] = m03;
		m_values[ 1 ][ 0 ] = m10; m_values[ 1 ][ 1 ] = m11; m_values[ 1 ][ 2 ] = m12; m_values[ 1 ][ 3 ] = m13;
		m_values[ 2 ][ 0 ] = m20; m_values[ 2 ][ 1 ] = m21; m_values[ 2 ][ 2 ] = m22; m_values[ 2 ][ 3 ] = m23;
	}

	void init( const vec3_t& x, const vec3_t& y, const vec3_t& z, const vec3_t &origin ) {
		m_values[ 0 ][ 0 ] = x.x; m_values[ 0 ][ 1 ] = y.x; m_values[ 0 ][ 2 ] = z.x; m_values[ 0 ][ 3 ] = origin.x;
		m_values[ 1 ][ 0 ] = x.y; m_values[ 1 ][ 1 ] = y.y; m_values[ 1 ][ 2 ] = z.y; m_values[ 1 ][ 3 ] = origin.y;
		m_values[ 2 ][ 0 ] = x.z; m_values[ 2 ][ 1 ] = y.z; m_values[ 2 ][ 2 ] = z.z; m_values[ 2 ][ 3 ] = origin.z;
	}

	matrix3x4_t( const vec3_t& x, const vec3_t& y, const vec3_t& z, const vec3_t &origin ) {
		init( x, y, z, origin );
	}

	inline void set_origin( vec3_t const& p ) {
		m_values[ 0 ][ 3 ] = p.x;
		m_values[ 1 ][ 3 ] = p.y;
		m_values[ 2 ][ 3 ] = p.z;
	}

	inline void invalidate( void ) {
		for ( int i = 0; i < 3; i++ ) {
			for ( int j = 0; j < 4; j++ ) {
				m_values[ i ][ j ] = std::numeric_limits<float>::infinity( );
			}
		}
	}

	vec3_t get_x_axis( void ) const {
		return at( 0 );
	}

	vec3_t get_y_axis( void ) const {
		return at( 1 );
	}

	vec3_t get_z_axis( void ) const {
		return at( 2 );
	}

	vec3_t origin( void ) const {
		return at( 3 );
	}

	vec3_t at( int i ) const {
		return vec3_t { m_values[ 0 ][ i ], m_values[ 1 ][ i ], m_values[ 2 ][ i ] };
	}

	float* operator[]( int i ) {
		return m_values[ i ];
	}

	const float* operator[]( int i ) const {
		return m_values[ i ];
	}

	float* base( ) {
		return &m_values[ 0 ][ 0 ];
	}

	const float* base( ) const {
		return &m_values[ 0 ][ 0 ];
	}

	const bool operator==( matrix3x4_t matrix ) const {
		return
			m_values[ 0 ][ 0 ] == matrix[ 0 ][ 0 ] && m_values[ 0 ][ 1 ] == matrix[ 0 ][ 1 ] && m_values[ 0 ][ 2 ] == matrix[ 0 ][ 2 ] && m_values[ 0 ][ 3 ] == matrix[ 0 ][ 3 ] &&
			m_values[ 1 ][ 0 ] == matrix[ 1 ][ 0 ] && m_values[ 1 ][ 1 ] == matrix[ 1 ][ 1 ] && m_values[ 1 ][ 2 ] == matrix[ 1 ][ 2 ] && m_values[ 1 ][ 3 ] == matrix[ 1 ][ 3 ] &&
			m_values[ 2 ][ 0 ] == matrix[ 2 ][ 0 ] && m_values[ 2 ][ 1 ] == matrix[ 2 ][ 1 ] && m_values[ 2 ][ 2 ] == matrix[ 2 ][ 2 ] && m_values[ 2 ][ 3 ] == matrix[ 2 ][ 3 ];
	}
};