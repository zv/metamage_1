/*
	current_process.cc
	------------------
*/

#include "Genie/current_process.hh"

// poseven
#include "poseven/types/errno_t.hh"

// Nitrogen
#include "Mac/Toolbox/Types/OSStatus.hh"

// OSErrno
#include "OSErrno/OSErrno.hh"

// Genie
#include "Genie/caught_signal.hh"
#include "Genie/Faults.hh"
#include "Genie/Process.hh"


namespace Genie
{
	
	namespace p7 = poseven;
	
	
	static p7::errno_t GetErrnoFromException()
	{
		try
		{
			throw;
		}
		catch ( const p7::errno_t& errnum )
		{
			return errnum;
		}
		catch ( const caught_signal& signal )
		{
			the_caught_signal = signal;
			
			return EINTR;
		}
		catch ( const Mac::OSStatus& err )
		{
			return OSErrno::ErrnoFromOSStatus( err );
		}
		catch ( ... )
		{
		}
		
		return EINVAL;
	}
	
	
	int set_errno( int errnum )
	{
		return gCurrentProcess->SetErrno( errnum );
	}
	
	int set_errno_from_exception()
	{
		return gCurrentProcess->SetErrno( GetErrnoFromException() );
	}
	
}

