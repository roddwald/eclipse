#pragma once

#define COMBINE( x, y ) x##y
#define COMBINE2( x, y ) COMBINE( x, y )

#define PAD( sz ) \
	private: \
	std::uint8_t COMBINE2( pad_, __COUNTER__ )[ sz ]; \
	public: