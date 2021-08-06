#pragma once
#include <cmath>
#include <algorithm>
#include "angles.hpp"

#define DOT_PROD( a, b ) ( a.x * b.x + a.y * b.y + a.z * b.z )

#define VEC_TRANSFORM( in1, in2, out )																	\
out.x = DOT_PROD( in1, vec3_t( in2 [ 0 ] [ 0 ], in2 [ 0 ] [ 1 ], in2 [ 0 ] [ 2 ] ) ) + in2 [ 0 ] [ 3 ];	\
out.y = DOT_PROD( in1, vec3_t( in2 [ 1 ] [ 0 ], in2 [ 1 ] [ 1 ], in2 [ 1 ] [ 2 ] ) ) + in2 [ 1 ] [ 3 ];	\
out.z = DOT_PROD( in1, vec3_t( in2 [ 2 ] [ 0 ], in2 [ 2 ] [ 1 ], in2 [ 2 ] [ 2 ] ) ) + in2 [ 2 ] [ 3 ];

#define CHECK_VALID( _v ) 0

class vec2_t {
public:
	vec2_t( ) = default;
	vec2_t( float x, float y ) {
		this->x = x;
		this->y = y;
	}
	float x, y;
};

class vec3_t {
public:
	float x, y, z;

	vec3_t( ) {
		init( );
	}

	vec3_t( float x, float y, float z = 0.0f ) {
		this->x = x;
		this->y = y;
		this->z = z;
	}

	void init( ) {
		this->x = this->y = this->z = 0.0f;
	}

	void init( float x, float y, float z ) {
		this->x = x;
		this->y = y;
		this->z = z;
	}
	inline float VectorNormalize( vec3_t& v ) {
		Assert( v.IsValid( ) );
		float l = v.Length( );
		if ( l != 0.0f ) {
			v /= l;
		}
		else {
			// FIXME:
			// Just copying the existing implemenation; shouldn't res.z == 0?
			v.x = v.y = 0.0f; v.z = 1.0f;
		}
		return l;
	}
	//===============================================
	inline float VectorNormalize( float* v ) {
		return VectorNormalize( *( reinterpret_cast< vec3_t* >( v ) ) );
	}

	float dot( const vec3_t& other ) const {
		const vec3_t& a = *this;
		return( a.x * other.x + a.y * other.y + a.z * other.z );
	}

	bool is_valid( ) {
		return std::isfinite( this->x ) && std::isfinite( this->y ) && std::isfinite( this->z );
	}

	void normalize_angles( ) {
		x = std::isfinite( x ) ? std::remainderf( x, 360.0f ) : 0.0f;
		y = std::isfinite( y ) ? std::remainderf( y, 360.0f ) : 0.0f;
		z = 0.0f;
	}

	bool is_zero( ) {
		return vec3_t( this->x, this->y, this->z ) == vec3_t( 0.0f, 0.0f, 0.0f );
	}

	void invalidate( ) {
		this->x = this->y = this->z = std::numeric_limits< float >::infinity( );
	}

	void clear( ) {
		this->x = this->y = this->z = 0.0f;
	}

	float& operator[]( int i ) {
		return ( ( float* ) this )[i];
	}

	float operator[]( int i ) const {
		return ( ( float* ) this )[i];
	}

	void zero( ) {
		this->x = this->y = this->z = 0.0f;
	}

	bool operator==( const vec3_t& src ) const {
		return ( src.x == this->x ) && ( src.y == y ) && ( src.z == z );
	}

	bool operator!=( const vec3_t& src ) const {
		return ( src.x != this->x ) || ( src.y != y ) || ( src.z != z );
	}

	vec3_t& operator+=( const vec3_t& v ) {
		this->x += v.x; this->y += v.y; this->z += v.z;

		return *this;
	}

	vec3_t& operator-=( const vec3_t& v ) {
		this->x -= v.x; this->y -= v.y; this->z -= v.z;

		return *this;
	}

	vec3_t& operator*=( float fl ) {
		this->x *= fl;
		this->y *= fl;
		this->z *= fl;

		return *this;
	}

	vec3_t& operator*=( const vec3_t& v ) {
		this->x *= v.x;
		this->y *= v.y;
		this->z *= v.z;

		return *this;
	}

	vec3_t& operator/=( const vec3_t& v ) {
		this->x /= v.x;
		this->y /= v.y;
		this->z /= v.z;

		return *this;
	}

	vec3_t& operator+=( float fl ) {
		this->x += fl;
		this->y += fl;
		this->z += fl;

		return *this;
	}

	vec3_t& operator/=( float fl ) {
		this->x /= fl;
		this->y /= fl;
		this->z /= fl;

		return *this;
	}

	vec3_t& operator-=( float fl ) {
		this->x -= fl;
		this->y -= fl;
		this->z -= fl;

		return *this;
	}

	void clamp( ) {
		while ( this->x < -89.0f )
			this->x += 89.0f;

		if ( this->x > 89.0f )
			this->x = 89.0f;

		while ( this->y < -180.0f )
			this->y += 360.0f;

		while ( this->y > 180.0f )
			this->y -= 360.0f;

		this->z = 0.0f;
	}

	void normalize( ) {
		*this = normalized( );
	}

	vec3_t normalized( ) const {
		auto res = *this;
		auto l = res.length( );

		if ( l != 0.0f )
			res /= l;
		else
			res.x = res.y = res.z = 0.0f;

		return res;
	}

	float normalize_place( ) {
		vec3_t vecOut = *this;

		float flLength = vecOut.length( );
		float flRadius = 1.0f / ( flLength + std::numeric_limits<float>::epsilon( ) );

		vecOut.x *= flRadius;
		vecOut.y *= flRadius;
		vecOut.z *= flRadius;

		return flLength;
	}

	float dist_to( const vec3_t& vec ) const {
		vec3_t delta;

		delta.x = this->x - vec.x;
		delta.y = this->y - vec.y;
		delta.z = this->z - vec.z;

		return delta.length( );
	}

	float dist_to_sqr( const vec3_t& vec ) const {
		vec3_t delta;

		delta.x = this->x - vec.x;
		delta.y = this->y - vec.y;
		delta.z = this->z - vec.z;

		return delta.length_sqr( );
	}

	float dot_product( const vec3_t& vec ) const {
		return this->x * vec.x + this->y * vec.y + this->z * vec.z;
	}

	vec3_t cross_product( const vec3_t& vec ) const {
		return vec3_t( this->y * vec.z - this->z * vec.y, this->z * vec.x - this->x * vec.z, this->x * vec.y - this->y * vec.x );
	}

	float length( ) const {
		return std::sqrtf( this->x * this->x + this->y * this->y + this->z * this->z );
	}

	float length_sqr( ) const {
		return this->x * this->x + this->y * this->y + this->z * this->z;
	}

	float length_2d_sqr( ) const {
		return this->x * this->x + this->y * this->y;
	}

	float length_2d( ) const {
		return std::sqrtf( this->x * this->x + this->y * this->y );
	}

	vec3_t& operator=( const vec3_t& vec ) {
		this->x = vec.x; this->y = vec.y; this->z = vec.z;

		return *this;
	}

	vec3_t operator-( ) const {
		return vec3_t( -this->x, -this->y, -this->z );
	}

	vec3_t operator+( const vec3_t& v ) const {
		return vec3_t( this->x + v.x, this->y + v.y, this->z + v.z );
	}

	vec3_t operator-( const vec3_t& v ) const {
		return vec3_t( this->x - v.x, this->y - v.y, this->z - v.z );
	}

	vec3_t operator*( float fl ) const {
		return vec3_t( this->x * fl, this->y * fl, this->z * fl );
	}

	vec3_t operator*( const vec3_t& v ) const {
		return vec3_t( this->x * v.x, this->y * v.y, this->z * v.z );
	}

	vec3_t operator/( float fl ) const {
		return vec3_t( this->x / fl, this->y / fl, this->z / fl );
	}

	vec3_t operator/( const vec3_t& v ) const {
		return vec3_t( this->x / v.x, this->y / v.y, this->z / v.z );
	}
	float Length( void ) const;
	vec3_t Normalize( );
};

__forceinline vec3_t operator*( float lhs, const vec3_t& rhs ) {
	return rhs * lhs;
}

__forceinline vec3_t operator/( float lhs, const vec3_t& rhs ) {
	return rhs / lhs;
}

inline float vector_length( const vec3_t& v ) {
	return ( float ) sqrt( v.x * v.x + v.y * v.y + v.z * v.z );
}

inline vec3_t vector_approach( vec3_t target, vec3_t value, float speed ) {
	vec3_t diff = ( target - value );
	float delta = diff.length( );

	if ( delta > speed )
		value += diff.normalized( ) * speed;
	else if ( delta < -speed )
		value -= diff.normalized( ) * speed;
	else
		value = target;

	return value;
}

class __declspec( align( 16 ) ) vec_alligned : public vec3_t {
public:

	inline vec_alligned( void ) { };
	inline vec_alligned( float x, float y, float z ) {
		init( x, y, z );
	}

public:

	explicit vec_alligned( const vec3_t& other ) {
		init( other.x, other.y, other.z );
	}

	vec_alligned& operator=( const vec3_t& other ) {
		init( other.x, other.y, other.z );
		return *this;
	}

	vec_alligned& operator=( const vec_alligned& other ) {
		init( other.x, other.y, other.z );
		return *this;
	}

	float w;
};