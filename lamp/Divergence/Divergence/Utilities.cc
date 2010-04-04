/*
	Divergence/Utilities.cc
	-----------------------
	
	Joshua Juran
*/

#include "Divergence/Utilities.hh"

// poseven
#include "poseven/functions/stat.hh"

#if TARGET_RT_MAC_MACHO
	#include "Nitrogen/Files.hh"
	#include "MacFiles/Classic.hh"
	#include "poseven/Pathnames.hh"
#else
	#include "FSSpec_from_stat.h"
#endif


namespace Divergence
{
	
	namespace p7 = poseven;
	
	
	FSSpec ResolvePathToFSSpec( const char* path )
	{
	#if TARGET_RT_MAC_MACHO
		
		try
		{
			return Nitrogen::FSMakeFSSpec( Nitrogen::FSPathMakeRef( path ).ref );
		}
		catch ( ... )
		{
		}
		
		using namespace io::path_descent_operators;
		
		const std::string parent_path = io::get_preceding_directory( path );
		
		FSRef parent_ref = Nitrogen::FSPathMakeRef( parent_path ).ref;
		
		FSSpec parent_spec = Nitrogen::FSMakeFSSpec( parent_ref );
		
		return parent_spec / io::get_filename( path );
		
	#else
		
		struct ::stat stat_buffer;
		
		const bool exists = p7::stat( path, stat_buffer );
		
		FSSpec spec;	
		
		p7::throw_posix_result( FSSpec_from_stat( stat_buffer, spec ) );
		
		return spec;
		
	#endif
	}
	
}
