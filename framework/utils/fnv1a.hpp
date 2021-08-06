#pragma once
#include "../sdk/sdk.hpp"

using FNV1A_t = std::uint32_t;

namespace fnv1a {
	constexpr std::uint32_t ull_basis = 0x811C9DC5;
	constexpr std::uint32_t ull_prime = 0x1000193;

	constexpr FNV1A_t hash_const( const char * str , const FNV1A_t val = ull_basis ) noexcept {
		return ( str[ 0 ] == '\0' ) ? val : hash_const( &str[ 1 ] , ( val ^ FNV1A_t( str[ 0 ] ) ) * ull_prime );
	}

	inline const FNV1A_t hash( const char * str ) {
		FNV1A_t hashed = ull_basis;
		for ( std::size_t i = 0U; i < strlen( str ); ++i ) {
			hashed ^= str[ i ];
			hashed *= ull_prime;
		}
		return hashed;
	}
}