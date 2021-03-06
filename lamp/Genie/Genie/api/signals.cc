/*
	signals.cc
	----------
*/

#include "Genie/api/signals.hh"

// Genie
#include "Genie/current_process.hh"
#include "Genie/Process.hh"


namespace Genie
{
	
	bool check_signals( bool may_throw )
	{
		Process& current = current_process();
		
		return current.HandlePendingSignals( may_throw );
	}
	
	void send_signal_to_current_process( int signo )
	{
		current_process().Raise( signo );
	}
	
}

