/*	=======
 *	Pipe.hh
 *	=======
 */

#ifndef GENIE_IO_PIPE_HH
#define GENIE_IO_PIPE_HH

// POSIX
#include <fcntl.h>

// Debug
#include "debug/boost_assert.hh"

// Boost
#include <boost/intrusive_ptr.hpp>

// Genie
#include "Genie/IO/Conduit.hh"
#include "Genie/IO/Stream.hh"


namespace Genie
{
	
	class PipeInHandle : public StreamHandle
	{
		private:
			boost::intrusive_ptr< Conduit > itsConduit;
		
		public:
			PipeInHandle( const boost::intrusive_ptr< Conduit >&  conduit,
			              bool                                    nonblocking )
			:
				StreamHandle( nonblocking ? O_WRONLY | O_NONBLOCK
				                          : O_WRONLY ),
				itsConduit( conduit )
			{
			}
			
			~PipeInHandle();
			
			unsigned int SysPoll()
			{
				return   kPollRead
				       | kPollWrite * itsConduit->IsWritable();
			}
			
			ssize_t SysWrite( const char* data, std::size_t byteCount )
			{
				return itsConduit->Write( data, byteCount, IsNonblocking() );
			}
			
			//void IOCtl( unsigned long request, int* argp );
	};
	
	class PipeOutHandle : public StreamHandle
	{
		private:
			boost::intrusive_ptr< Conduit > itsConduit;
		
		public:
			PipeOutHandle( const boost::intrusive_ptr< Conduit >&  conduit,
			               bool                                    nonblocking )
			:
				StreamHandle( nonblocking ? O_RDONLY | O_NONBLOCK
				                          : O_RDONLY ),
				itsConduit( conduit )
			{
			}
			
			~PipeOutHandle();
			
			unsigned int SysPoll()
			{
				return   kPollRead * itsConduit->IsReadable()
				       | kPollWrite;
			}
			
			ssize_t SysRead( char* data, std::size_t byteCount )
			{
				return itsConduit->Read( data, byteCount, IsNonblocking() );
			}
			
			//void IOCtl( unsigned long request, int* argp );
	};
	
}

#endif

