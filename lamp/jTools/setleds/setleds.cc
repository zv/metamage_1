/*	==========
 *	setleds.cc
 *	==========
 */

// Standard C++
#include <algorithm>

// Standard C/C++
#include <cstdio>
#include <cstdlib>
#include <cstring>

// Mac OS
#ifndef __MACTYPES__
#include <MacTypes.h>
#endif

// iota
#include "iota/strings.hh"

// gear
#include "gear/parse_decimal.hh"

// plus
#include "plus/var_string.hh"

// Arcana
#if !TARGET_API_MAC_CARBON
#include "ADB/KeyboardLEDs.hh"
#endif

// poseven
#include "poseven/functions/write.hh"

// Orion
#include "Orion/Main.hh"


namespace Nitrogen
{
	// Define namespace Nitrogen for Carbon
}

namespace tool
{
	
	namespace N = Nitrogen;
	namespace p7 = poseven;
	
	
	#if !TARGET_API_MAC_CARBON
	
	
	const UInt8 kLEDValueMask = 7;
	
	static int gChangedBits     = 0;
	static int gChangedBitsMask = 0;
	
	
	static inline bool IsKeyboard( const ADBDataBlock& adbData )
	{
		const SInt8 kADBKeyboardDevice = 2;
		
		return adbData.devType == kADBKeyboardDevice;
	}
	
	
	static void PrintLEDs( UInt8 leds )
	{
		const char* status[] = { "off", "on " };
		
		bool num    = leds & 1;
		bool caps   = leds & 2;
		bool scroll = leds & 4;
		
		plus::var_string message;
		
		// Reserve enough memory to hold everything, avoiding reallocation
		message.reserve( 255 );
		
		/*
		message += "# Device at ADB address ";
		
		message += address + '0';
		message += "\n";
		*/
		
		message += "Current default flags:  NumLock on    CapsLock off   ScrollLock off\n";
		message += "Current flags:          NumLock on    CapsLock off   ScrollLock off\n";
		message += "Current leds:           NumLock ";
		
		message += status[ num    ];
		message += "   " "CapsLock ";
		message += status[ caps   ];
		message += "   " "ScrollLock ";
		message += status[ scroll ];
		message += "\n";
		
		p7::write( p7::stdout_fileno, message );
	}
	
	static void DoLEDs( N::GetIndADB_Result adbDevice, int leds )
	{
		if ( IsKeyboard( adbDevice ) )
		{
			UInt8 leds = GetLEDs( adbDevice );
			
			if ( gChangedBitsMask == 0 )
			{
				PrintLEDs( leds );
			}
			else
			{
				leds &= ~gChangedBitsMask;
				leds |= gChangedBits;
				
				SetLEDs( adbDevice, leds );
			}
		}
	}
	
	static const char* gOptionNames[] = { "num", "caps", "scroll", NULL };
	
	#endif
	
	
	int Main( int argc, char** argv )
	{
	#if TARGET_API_MAC_CARBON
		
		p7::write( p7::stderr_fileno, STR_LEN( "setleds: ADB unsupported in Carbon\n" ) );
		
		return EXIT_FAILURE;
		
	#else
		
		int count = N::CountADBs();
		
		for ( char** argp = argv + 1;  *argp != NULL;  ++argp )
		{
			const char* arg = *argp;
			
			bool on = false;
			
			if ( arg[0] == '+' )
			{
				on = true;
			}
			else if ( arg[0] != '-' )
			{
				std::fprintf( stderr, "setleds: unrecognized argument '%s'\n", arg );
				
				return 2;
			}
			
			for ( const char** pp = gOptionNames;  ;  ++pp )
			{
				if ( std::strcmp( *pp, arg + 1 ) == 0 )
				{
					int bit = pp - gOptionNames;
					
					gChangedBits     |= on << bit;
					gChangedBitsMask |= 1  << bit;
					
					break;
				}
				
				if ( *pp == NULL )
				{
					std::fprintf( stderr, "setleds: unrecognized led '%s'\n", arg );
					
					return 2;
				}
			}
		}
		
		bool setting = argc >= 2;
		
		int leds = setting ? gear::parse_unsigned_decimal( argv[1] ) & kLEDValueMask
		                   : -1;
		
		N::ADBDevice_Container adbs;
		
		std::for_each( adbs.begin(),
		               adbs.end(),
		               std::bind2nd( std::ptr_fun( DoLEDs ),
		                             leds ) );
		
		return 0;
		
	#endif
	}
	
}

