/*	=========
 *	signal.cc
 *	=========
 */

// Standard C
#include <errno.h>

// POSIX
#include "signal.h"

// POSeven
//#include "POSeven/Errno.h"

// Genie
#include "Genie/Process.hh"
#include "Genie/Yield.hh"


namespace Genie
{
	
	//namespace P7 = POSeven;
	
	
	static int kill( pid_t pid, int sig )
	{
		if ( !gProcessTable.Exists( pid ) )
		{
			return -1;  // FIXME:  set errno
		}
		
		if ( sig != 0 )
		{
			gProcessTable[ pid ].Raise( sig );
		}
		
		return 0;
	}
	
	static __sig_handler signal( int sig, __sig_handler func )
	{
		return CurrentProcess().SetSignalAction( sig, func );
	}
	
}

#pragma export on
	
	int kill( pid_t pid, int sig )
	{
		return Genie::kill( pid, sig );
	}
	
	__sig_handler signal( int sig, __sig_handler func )
	{
		return Genie::signal( sig, func );
	}
	
#pragma export reset

