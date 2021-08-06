#pragma once
#include <memory>
#include <windows.h>
#include <psapi.h>

#define in_range( x, a, b ) ( x >= a && x <= b )
#define get_bits( x ) ( in_range( ( x & ( ~0x20 ) ), 'A', 'F' ) ? ( ( x & ( ~0x20 ) ) - 'A' + 0xA ) : ( in_range( x, '0', '9' ) ? x - '0' : 0 ) )
#define get_byte( x ) ( get_bits( x[ 0 ] ) << 4 | get_bits( x[ 1 ] ) )

class pattern {
	std::uintptr_t m_addr;

public:
	__forceinline pattern( std::uintptr_t addr ) {
		this->m_addr = addr;
	}

	template < typename t >
	__forceinline t get( ) {
		return t( this->m_addr );
	}

	__forceinline pattern sub( std::uintptr_t bytes ) {
		return pattern( this->m_addr - bytes );
	}

	__forceinline pattern add( std::uintptr_t bytes ) {
		return pattern( this->m_addr + bytes );
	}

	__forceinline pattern deref( ) {
		return pattern( *( std::uintptr_t* ) this->m_addr );
	}

	__forceinline pattern resolve_rip( ) {
		return pattern( this->m_addr + *( int* ) ( this->m_addr + 1 ) + 5 );
	}

	static pattern search( const char* mod, const char* pat ) {
		auto pat1 = const_cast< char* >( pat );
		auto range_start = reinterpret_cast< std::uintptr_t >( GetModuleHandleA( mod ) );

		MODULEINFO mi;
		K32GetModuleInformation( GetCurrentProcess( ), reinterpret_cast< HMODULE >( range_start ), &mi, sizeof( MODULEINFO ) );

		auto end = range_start + mi.SizeOfImage;

		std::uintptr_t first_match = 0;

		for ( std::uintptr_t current_address = range_start; current_address < end; current_address++ ) {
			if ( !*pat1 )
				return pattern( first_match );

			if ( *reinterpret_cast< std::uint8_t* >( pat1 ) == '\?' || *reinterpret_cast< std::uint8_t* >( current_address ) == get_byte( pat1 ) ) {
				if ( !first_match )
					first_match = current_address;

				if ( !pat1 [ 2 ] )
					return pattern( first_match );

				if ( *reinterpret_cast< std::uint16_t* >( pat1 ) == '\?\?' || *reinterpret_cast< std::uint8_t* >( pat1 ) != '\?' )
					pat1 += 3;
				else
					pat1 += 2;
			}
			else {
				pat1 = const_cast< char* >( pat );
				first_match = 0;
			}
		}

		return pattern( 0 );
	}

	///* yoinked from csgosimple */
	//static std::uint8_t* scan(void* module, const char* signature)
	//{
	//	static auto pattern_to_byte = [](const char* pattern) {
	//		auto bytes = std::vector<int>{};
	//		auto start = const_cast<char*>(pattern);
	//		auto end = const_cast<char*>(pattern) + strlen(pattern);

	//		for (auto current = start; current < end; ++current) {
	//			if (*current == '?') {
	//				++current;
	//				if (*current == '?')
	//					++current;
	//				bytes.push_back(-1);
	//			}
	//			else {
	//				bytes.push_back(strtoul(current, &current, 16));
	//			}
	//		}
	//		return bytes;
	//	};

	//	auto dosHeader = (PIMAGE_DOS_HEADER)module;
	//	auto ntHeaders = (PIMAGE_NT_HEADERS)((std::uint8_t*)module + dosHeader->e_lfanew);

	//	auto sizeOfImage = ntHeaders->OptionalHeader.SizeOfImage;
	//	auto patternBytes = pattern_to_byte(signature);
	//	auto scanBytes = reinterpret_cast<std::uint8_t*>(module);

	//	auto s = patternBytes.size();
	//	auto d = patternBytes.data();

	//	for (auto i = 0ul; i < sizeOfImage - s; ++i) {
	//		bool found = true;
	//		for (auto j = 0ul; j < s; ++j) {
	//			if (scanBytes[i + j] != d[j] && d[j] != -1) {
	//				found = false;
	//				break;
	//			}
	//		}
	//		if (found) {
	//			return &scanBytes[i];
	//		}
	//	}
	//	return nullptr;
	//}
};