#include "texture_handler.hpp"
#include <stdexcept>

std::uint8_t* c_textureholder::decode_resource_image( unsigned short id, unsigned int w, unsigned int h ) {
	const HRSRC m_resource = FindResource( g::module, MAKEINTRESOURCE( id ), "PNG" );
	const HGLOBAL m_loaded = LoadResource( g::module, m_resource );
	const LPVOID m_resource_ptr = LockResource( m_loaded );
	const DWORD m_resource_size = SizeofResource( g::module, m_resource );

	std::vector < std::uint8_t > m_image;

	if ( const auto error = lodepng::decode( m_image, w, h, ( unsigned char* ) m_resource_ptr, m_resource_size ) )
		throw std::runtime_error( "Failed to decode image!" );

	const auto data = new std::uint8_t[m_image.size( )];

	std::copy( m_image.begin( ), m_image.end( ), data );

	return data;
}