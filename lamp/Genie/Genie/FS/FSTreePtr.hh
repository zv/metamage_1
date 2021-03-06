/*
	FSTreePtr.hh
	------------
*/

#ifndef GENIE_FS_FSTREEPTR_HH
#define GENIE_FS_FSTREEPTR_HH

// Debug
#include "debug/boost_assert.hh"

// boost
#include <boost/intrusive_ptr.hpp>


namespace Genie
{
	
	class FSTree;
	
	typedef boost::intrusive_ptr< FSTree const > FSTreePtr;
	
}

#endif

