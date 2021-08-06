#include "draw.hpp"

unsigned long fonts::menu = NULL;
unsigned long fonts::menu_bold = NULL;
unsigned long fonts::visual_big = NULL;
unsigned long fonts::visual_small = NULL;

void fonts::initialize( ) {
	csgo::i::surface->set_font_glyph( fonts::menu_bold = csgo::i::surface->font_create( ), "Tahoma", 12, FW_BOLD, 0, 0, FONTFLAG_DROPSHADOW );
	csgo::i::surface->set_font_glyph( fonts::menu = csgo::i::surface->font_create( ), "Tahoma", 12, FW_NORMAL, 0, 0, FONTFLAG_DROPSHADOW );
	csgo::i::surface->set_font_glyph( fonts::visual_big = csgo::i::surface->font_create( ), "Tahoma", 12, FW_BOLD, 0, 0, FONTFLAG_DROPSHADOW );
	csgo::i::surface->set_font_glyph( fonts::visual_small = csgo::i::surface->font_create( ), "hooge 05_55 Cyr2", 10, FW_NORMAL, 0, 0, FONTFLAG_OUTLINE );
}

void draw::rectangle( int x, int y, int w, int h, color color ) {
	csgo::i::surface->draw_set_color( color.r, color.g, color.b, color.a );
	csgo::i::surface->draw_filled_rect( x, y, x + w, y + h );
}

void draw::outline( int x, int y, int w, int h, color color ) {
	csgo::i::surface->draw_set_color( color.r, color.g, color.b, color.a );
	csgo::i::surface->draw_outlined_rect( x, y, x + w, y + h );
}

void draw::outlined_vec_rect( int x, int y, int x2, int y2, color color ) {
	csgo::i::surface->draw_set_color( color.r, color.g, color.b, color.a );
	csgo::i::surface->draw_line( x, y, x2, y );
	csgo::i::surface->draw_line( x2, y, x2, y2 );
	csgo::i::surface->draw_line( x2, y2, x, y2 );
	csgo::i::surface->draw_line( x, y2, x, y );
}

void draw::line( int x, int y, int x2, int y2, color color ) {
	csgo::i::surface->draw_set_color( color.r, color.g, color.b, color.a );
	csgo::i::surface->draw_line( x, y, x2, y2 );
}

void draw::string( int x, int y, color color, unsigned long font, bool center, const char* input, ... ) {
	if ( input == NULL ) return;

	va_list			va_alist;
	char			buffer[1024];
	wchar_t			string[1024];

	va_start( va_alist, input );
	_vsnprintf( buffer, sizeof( buffer ), input, va_alist );
	va_end( va_alist );

	MultiByteToWideChar( CP_UTF8, 0, buffer, 256, string, 256 );

	if ( center )
		csgo::i::surface->draw_set_text_pos( x - ( draw::get_text_size( font, buffer ).right / 2 ), y );
	else
		csgo::i::surface->draw_set_text_pos( x, y );

	csgo::i::surface->draw_set_text_font( font );
	csgo::i::surface->draw_set_text_color( color.r, color.g, color.b, color.a );
	csgo::i::surface->draw_print_text( string, wcslen( string ) );
}

RECT draw::get_text_size( unsigned long font, const char* text, ... ) {
	size_t		origsize = strlen( text ) + 1;
	size_t		converted_chars = 0;

	wchar_t		wcstring[255];
	mbstowcs_s( &converted_chars, wcstring, origsize, text, _TRUNCATE );

	RECT		rect;
	int			x, y;

	csgo::i::surface->get_text_size( font, wcstring, x, y );

	rect.left = x; rect.bottom = y;
	rect.right = x;
	return rect;
}

void draw::gradient( int x, int y, int w, int h, color c1, color c2, bool horizontal ) {
	csgo::i::surface->draw_set_color( c1.r, c1.g, c1.b, c1.a );
	csgo::i::surface->draw_filled_rect_fade( x, y, x + w, y + h, 255, 255, horizontal );

	csgo::i::surface->draw_set_color( c2.r, c2.g, c2.b, c2.a );
	csgo::i::surface->draw_filled_rect_fade( x, y, x + w, y + h, 0, 255, horizontal );
}
vec2_t draw::get_text_size_vec(unsigned long font, std::string text) {
	std::wstring a(text.begin(), text.end());
	const wchar_t* wstr = a.c_str();
	static int width, height;

	csgo::i::surface->get_text_size(font, wstr, width, height);
	return { static_cast<float>(width), static_cast<float>(height) };
}
