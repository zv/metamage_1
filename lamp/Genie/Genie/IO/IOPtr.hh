/*
	IOPtr.hh
	--------
*/

#ifndef GENIE_IO_IOPTR_HH
#define GENIE_IO_IOPTR_HH

// Debug
#include "debug/boost_assert.hh"

// boost
#include <boost/intrusive_ptr.hpp>


namespace Genie
{
	
	class IOHandle;
	
	typedef boost::intrusive_ptr< IOHandle > IOPtr;
	
}

#endif

