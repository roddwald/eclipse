#include <windows.h>
#include <cstdint>
#include <iostream>
#include <thread>

#include "minhook/minhook.h"
#include "sdk/sdk.hpp"
#include "hooks/hooks.hpp"
#include "globals.hpp"

#include "utils/utils.hpp"
#include "utils/lazy_importer.hpp"

void init( HMODULE mod ) {
	/* wait for all modules to load */
	while ( !GetModuleHandleA( _( "serverbrowser.dll" ) ) )
		std::this_thread::sleep_for( std::chrono::milliseconds( 100 ) );

	g::module = mod;

	/* initialize hack */
	csgo::init( );
	netvars::init( );
	hooks::init( );

	/* wait for unload key */
	while ( !GetAsyncKeyState( VK_END ) )
		std::this_thread::sleep_for( std::chrono::milliseconds( 500 ) );

	/* unload */
	MH_RemoveHook( MH_ALL_HOOKS );
	MH_Uninitialize( );

	FreeLibraryAndExitThread( mod, 0 );
}

int __stdcall DllMain( HINSTANCE inst, std::uint32_t reason, void* reserved ) {
	if ( reason == DLL_PROCESS_ATTACH ) {
		( CreateThread ) ( nullptr, 0, LPTHREAD_START_ROUTINE( init ), HMODULE( inst ), 0, nullptr );
	}

	return 1;
}