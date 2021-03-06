/*	========
 *	sleep.cc
 *	========
 */

// Standard C/C++
#include <cmath>
#include <cstdlib>

// POSIX
#include <time.h>
#include <unistd.h>

// Iota
#include "iota/strings.hh"

// gear
#include "gear/parse_float.hh"

// more-posix
#include "more/perror.hh"


int main( int argc, char const *const argv[] )
{
	// Check for correct number of args
	if ( argc != 2 )
	{
		write( STDERR_FILENO, STR_LEN( "Usage: sleep seconds\n" ) );
		
		return 2;
	}
	
	const char* sleep_arg = argv[1];
	
	const float sleep_time = gear::parse_float( &sleep_arg );
	
	if ( const bool parsed = sleep_arg != argv[1] )
	{
		const float int_sleep_time = std::floor( sleep_time );
		
		const unsigned long seconds     = static_cast< unsigned long >( int_sleep_time );
		const unsigned long nanoseconds = static_cast< unsigned long >( (sleep_time - int_sleep_time) * 1000 * 1000 * 1000 );
		
		const timespec time = { seconds, nanoseconds };
		
		nanosleep( &time, NULL );
	}
	else
	{
		more::perror( "sleep: invalid time interval", argv[1], 0 );
		
		return EXIT_FAILURE;
	}
	
	return EXIT_SUCCESS;
}

