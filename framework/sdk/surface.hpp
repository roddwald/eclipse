#pragma once
#include "../utils/vfunc.hpp"
#include "color.hpp"

struct vertex_t {
	vertex_t( ) {
	}

	vertex_t( const vec2_t & pos , const vec2_t & coord = vec2_t( 0 , 0 ) ) {
		m_pos = pos;
		m_tex_coord = coord;
	}

	void init( const vec2_t & pos , const vec2_t & coord = vec2_t( 0 , 0 ) ) {
		m_pos = pos;
		m_tex_coord = coord;
	}

	vec2_t	m_pos;
	vec2_t	m_tex_coord;
};

typedef vertex_t font_vertex_t;
enum FontDrawType_t {
	FONT_DRAW_DEFAULT = 0 ,

	FONT_DRAW_NONADDITIVE ,
	FONT_DRAW_ADDITIVE ,

	FONT_DRAW_TYPE_COUNT = 2 ,
};

struct CharRenderInfo {
	FontDrawType_t	drawType;
	wchar_t			ch;

	bool			valid;

	bool			shouldclip;
	int				x , y;
	vertex_t		verts[ 2 ];
	int				textureId;
	int				abcA;
	int				abcB;
	int				abcC;
	int				fontTall;
	unsigned int	currentFont;
};

enum SurfaceFeature_e {
	ANTIALIASED_FONTS = 1 ,
	DROPSHADOW_FONTS = 2 ,
	ESCAPE_KEY = 3 ,
	OPENING_NEW_HTML_WINDOWS = 4 ,
	FRAME_MINIMIZE_MAXIMIZE = 5 ,
	OUTLINE_FONTS = 6 ,
	DIRECT_HWND_RENDER = 7 ,
};

struct IntRect {
	int x0;
	int y0;
	int x1;
	int y1;
};

enum EFontFlags {
	FONTFLAG_NONE ,
	FONTFLAG_ITALIC = 0x001 ,
	FONTFLAG_UNDERLINE = 0x002 ,
	FONTFLAG_STRIKEOUT = 0x004 ,
	FONTFLAG_SYMBOL = 0x008 ,
	FONTFLAG_ANTIALIAS = 0x010 ,
	FONTFLAG_GAUSSIANBLUR = 0x020 ,
	FONTFLAG_ROTARY = 0x040 ,
	FONTFLAG_DROPSHADOW = 0x080 ,
	FONTFLAG_ADDITIVE = 0x100 ,
	FONTFLAG_OUTLINE = 0x200 ,
	FONTFLAG_CUSTOM = 0x400 ,
	FONTFLAG_BITMAP = 0x800 ,
};

class c_surface {
public:
	void draw_set_color( color c ) {
		typedef void( __thiscall * fn )( void * , color );
		return vfunc<fn>( this , 14 )( this , c );
	}

	void draw_set_color( int r , int g , int b , int a ) {
		typedef void( __thiscall * fn )( void * , int , int , int , int );
		return vfunc<fn>( this , 15 )( this , r , g , b , a );
	}

	void draw_filled_rect( int x0 , int y0 , int x1 , int y1 ) {
		call_vfunc<void>( this , 16 , x0 , y0 , x1 , y1 );
	}

	void draw_filled_rect_fade( int x0 , int y0 , int x1 , int y1 , uint32_t uAlpha0 , uint32_t uAlpha1 , bool bHorizontal ) {
		call_vfunc<void>( this , 123 , x0 , y0 , x1 , y1 , uAlpha0 , uAlpha1 , bHorizontal );
	}

	void draw_outlined_rect( int x0 , int y0 , int x1 , int y1 ) {
		call_vfunc<void>( this , 18 , x0 , y0 , x1 , y1 );
	}

	void draw_line( int x0 , int y0 , int x1 , int y1 ) {
		call_vfunc<void>( this , 19 , x0 , y0 , x1 , y1 );
	}

	void draw_textured_rect( int x , int y , int width , int height ) {
		using fn = void( __thiscall * )( c_surface * , int , int , int , int );
		return ( *( fn ** ) this )[ 41 ]( this , x , y , width , height );
	}

	void draw_poly_line( int * x , int * y , int nPoints ) {
		call_vfunc<void>( this , 20 , x , y , nPoints );
	}

	void draw_set_text_font( unsigned int hFont ) {
		call_vfunc<void>( this , 23 , hFont );
	}

	void draw_set_text_color( color c ) {
		typedef void( __thiscall * fn )( void * , color );
		return vfunc<fn>( this , 24 )( this , c );
	}

	void draw_set_text_color( int r , int g , int b , int a ) {
		typedef void( __thiscall * fn )( void * , int , int , int , int );
		return vfunc<fn>( this , 25 )( this , r , g , b , a );
	}

	void draw_set_text_pos( int x , int y ) {
		call_vfunc<void>( this , 26 , x , y );
	}

	void draw_print_text( const wchar_t * text , int text_length , int draw_type = 0 ) {
		typedef void( __thiscall * fn )( void * , const wchar_t * , int , int );
		return vfunc<fn>( this , 28 )( this , text , text_length , draw_type );
	}

	void draw_set_texture_rgba( int nIndex , const unsigned char * rgba , int iWide , int iTall ) {
		call_vfunc<void>( this , 37 , nIndex , rgba , iWide , iTall );
	}

	void draw_set_texture( int nIndex ) {
		call_vfunc<void>( this , 38 , nIndex );
	}

	bool is_texture_id_valid( int texture_id ) {
		using fn = bool( __thiscall * )( c_surface * , int );
		return ( *( fn ** ) this )[ 42 ]( this , texture_id );
	}

	int create_new_texture_id( bool bProcedural = false ) {
		return call_vfunc<int>( this , 43 , bProcedural );
	}

	unsigned int font_create( ) {
		return call_vfunc<unsigned int>( this , 71 );
	}

	bool set_font_glyph( unsigned int hFont , const char * szWindowsFontName , int iTall , int iWeight , int iBlur , int nScanLines , int iFlags , int nRangeMin = 0 , int nRangeMax = 0 ) {
		return call_vfunc<bool>( this , 72 , hFont , szWindowsFontName , iTall , iWeight , iBlur , nScanLines , iFlags , nRangeMin , nRangeMax );
	}

	void get_text_size( DWORD font , const wchar_t * text , int & wide , int & tall ) {
		using original_fn = void( __thiscall * )( void * , unsigned long , const wchar_t * , int & , int & );
		return ( *( original_fn ** ) this )[ 79 ]( this , font , text , wide , tall );
	}

	void play_sound( const char * szFileName ) {
		call_vfunc<void>( this , 82 , szFileName );
	}

	void draw_outlined_circle( int x , int y , int iRadius , int nSegments ) {
		call_vfunc<void>( this , 103 , x , y , iRadius , nSegments );
	}

	void unlock_cursor( void ) {
		using unlockcusor_fn = void( __thiscall * )( void * );
		vfunc< unlockcusor_fn >( this , 66 )( this );
	}

	void lock_cursor( void ) {
		using lockcusor_fn = void( __thiscall * )( void * );
		vfunc< lockcusor_fn >( this , 67 )( this );
	}

	void get_cursor_pos( int & x , int & y ) {
		using get_cursor_pos_fn = void( __thiscall * )( void * , int & , int & );
		vfunc< get_cursor_pos_fn >( this , 100 )( this , x , y );
	}
};