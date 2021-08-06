#include "../sdk/sdk.hpp"

namespace fonts {
	extern unsigned long menu;
	extern unsigned long menu_bold;
	extern unsigned long visual_big;
	extern unsigned long visual_small;

	/* functions */
	void initialize( );
};

namespace draw {
	void rectangle( int x, int y, int w, int h, color color );

	void outline( int x, int y, int w, int h, color color );

	void outlined_vec_rect( int x, int y, int x2, int y2, color color );

	void line( int x, int y, int x2, int y2, color color );

	void string( int x, int y, color color, unsigned long font, bool center, const char* input, ... );

	RECT get_text_size( unsigned long font, const char* text, ... );

	void gradient( int x, int y, int w, int h, color c1, color c2, bool horizontal );

	vec2_t get_text_size_vec(unsigned long font, std::string text);
}