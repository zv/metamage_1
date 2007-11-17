/*	=============
 *	ConsoleTTY.cc
 *	=============
 */

#include "Genie/IO/ConsoleTTY.hh"

// Standard C
#include <errno.h>

// Lamp
#include <lamp/winio.h>

// Nucleus
#include "Nucleus/Convert.h"

// POSeve
#include "POSeven/Errno.hh"

// Genie
#include "Genie/IO/ConsoleWindow.hh"
#include "Genie/Yield.hh"


namespace Genie
{
	
	namespace N = Nitrogen;
	namespace NN = Nucleus;
	namespace p7 = poseven;
	
	
	static ConsoleMap gConsoleMap;
	
	const ConsoleMap& GetConsoleMap()
	{
		return gConsoleMap;
	}
	
	
	boost::shared_ptr< IOHandle > NewConsoleDevice()
	{
		static ConsoleID gLastID = 0;
		
		boost::shared_ptr< IOHandle > consoleDevice( new ConsoleTTYHandle( ++gLastID ) );
		
		gConsoleMap[ gLastID ] = consoleDevice;
		
		return consoleDevice;
	}
	
	TTYHandle& GetConsoleByID( ConsoleID id )
	{
		ConsoleMap::const_iterator it = gConsoleMap.find( id );
		
		if ( it == gConsoleMap.end() )
		{
			p7::throw_errno( ENOENT );
		}
		
		ASSERT( !it->second.expired() );
		
		return IOHandle_Cast< TTYHandle >( *it->second.lock() );
	}
	
	static std::string MakeConsoleName( ConsoleID id )
	{
		return "/dev/con/" + NN::Convert< std::string >( id );
	}
	
	static bool ShouldSalvageConsoleWindow( int salvagePolicy, int leaderWaitStatus )
	{
		switch ( salvagePolicy )
		{
			default:
			case kLampSalvageWindowOnExitNever:
				//return false;
			
			case kLampSalvageWindowOnExitForSignal:
				// FIXME
			
			case kLampSalvageWindowOnExitForFailure:
				return leaderWaitStatus != 0;
			
			case kLampSalvageWindowOnExitAlways:
				return true;
		}
	}
	
	
	std::map< ::WindowRef, boost::shared_ptr< ConsoleWindow > > gSalvagedConsoles;
	
	
	class SalvagedWindowCloseHandler : public Ped::WindowCloseHandler
	{
		public:
			void operator()( N::WindowRef window ) const;
	};
	
	static void SalvageConsole( const boost::shared_ptr< ConsoleWindow >& console )
	{
		ASSERT( console.get() != NULL );
		
		boost::shared_ptr< Ped::WindowCloseHandler > handler( new SalvagedWindowCloseHandler() );
		
		console->SetCloseHandler( handler );
		
		gSalvagedConsoles[ console->Get() ] = console;
	}
	
	void SalvagedWindowCloseHandler::operator()( N::WindowRef window ) const
	{
		gSalvagedConsoles.erase( window );
	}
	
	
	ConsoleTTYHandle::ConsoleTTYHandle( ConsoleID id ) : TTYHandle( MakeConsoleName( id ) ),
	                                                     id( id ),
	                                                     itsWindowSalvagePolicy( kLampSalvageWindowOnExitNever ),
	                                                     itsLeaderWaitStatus()
	{
	}
	
	ConsoleTTYHandle::~ConsoleTTYHandle()
	{
		if ( console.get()  &&  !IsDisconnected()  &&  ShouldSalvageConsoleWindow( itsWindowSalvagePolicy, itsLeaderWaitStatus ) )
		{
			std::string exCon = "(" + NN::Convert< std::string >( itsLeaderWaitStatus ) + ")";
			
			N::SetWTitle( console->Get(), N::Str255( exCon ) );
			
			SalvageConsole( console );
		}
		
		gConsoleMap.erase( id );
	}
	
	IOHandle* ConsoleTTYHandle::Next() const
	{
		return console.get();
	}
	
	unsigned int ConsoleTTYHandle::SysPoll() const
	{
		bool readable = true;
		
		try
		{
			readable = !itsCurrentInput.empty()  ||  console.get() && console->IsReadyForInput();
		}
		catch ( const io::end_of_input& )
		{
			// IsReadyForInput() throws on EOF, in which case readable is true
		}
		
		int readability = readable ? kPollRead : 0;
		
		return readability | kPollWrite;
	}
	
	int ConsoleTTYHandle::SysRead( char* data, std::size_t byteCount )
	{
		Open();
		
		// Zero byteCount always begets zero result
		if ( byteCount == 0 )
		{
			return 0;
		}
		
		while ( true )
		{
			if ( itsCurrentInput.empty() )
			{
				try
				{
					if ( console->IsReadyForInput() )
					{
						itsCurrentInput = console->ReadInput();
					}
				}
				catch ( const io::end_of_input& )
				{
					return 0;
				}
			}
			
			if ( !itsCurrentInput.empty() )
			{
				// Actual byteCount is lesser of requested size and available size
				std::size_t bytesCopied = std::min( byteCount, itsCurrentInput.size() );
				
				// Copy data from input to buffer
				std::copy( itsCurrentInput.begin(),
				           itsCurrentInput.begin() + bytesCopied,
				           data );
				
				// Slide remaining data to beginning
				std::copy( itsCurrentInput.begin() + bytesCopied,
				           itsCurrentInput.end(),
				           itsCurrentInput.begin() );
				
				// and take up the slack
				itsCurrentInput.resize( itsCurrentInput.size() - bytesCopied );
				
				return bytesCopied;
			}
			
			TryAgainLater();
		}
	}
	
	int ConsoleTTYHandle::SysWrite( const char* data, std::size_t byteCount )
	{
		Open();
		
		return console->Write( data, byteCount );
	}
	
	void ConsoleTTYHandle::IOCtl( unsigned long request, int* argp )
	{
		switch ( request )
		{
			case WIOCGEXIT:
				if ( argp != NULL )
				{
					*argp = itsWindowSalvagePolicy;
				}
				
				break;
			
			case WIOCSEXIT:
				if ( argp == NULL )
				{
					p7::throw_errno( EFAULT );
				}
				
				itsWindowSalvagePolicy = *argp;
				break;
			
			case WIOCGTITLE:
			case WIOCSTITLE:
				Open();
				
				// fall through
			
			default:
				TTYHandle::IOCtl( request, argp );
				break;
		};
	}
	
	static N::Str255 DefaultConsoleTitle( ConsoleID id )
	{
		return N::Str255( "/dev/con/" + NN::Convert< std::string >( id ) );
	}
	
	static boost::shared_ptr< ConsoleWindow > NewConsole( ConsoleID id, ConstStr255Param title )
	{
		boost::shared_ptr< ConsoleWindow > console( new ConsoleWindow( id, title ) );
		
		return console;
	}
	
	void ConsoleTTYHandle::Open()
	{
		if ( console.get() == NULL )
		{
			console = NewConsole( id, DefaultConsoleTitle( id ) );
		}
	}
	
}

