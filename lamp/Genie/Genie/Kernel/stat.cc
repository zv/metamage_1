/*	=======
 *	stat.cc
 *	=======
 */

// Standard C/C++
#include <cstring>

// Standard C++
#include <string>

// POSIX
//#include "stdlib.h"
#include "sys/stat.h"

// Genie
#include "Genie/FileDescriptors.hh"
#include "Genie/FileSystem/ResolvePathAt.hh"
#include "Genie/FileSystem/ResolvePathname.hh"
#include "Genie/FileSystem/StatFile.hh"
#include "Genie/IO/File.hh"
#include "Genie/Process.hh"
#include "Genie/SystemCallRegistry.hh"
#include "Genie/SystemCalls.hh"


namespace Genie
{
	
	static int faccessat( int dirfd, const char* path, int mode, int flags )
	{
		SystemCallFrame frame( "faccessat" );
		
		try
		{
			FSTreePtr file = ResolvePathAt( dirfd, path );
			
			if ( const bool following_links = !(flags & AT_SYMLINK_NOFOLLOW) )
			{
				ResolveLinks_InPlace( file );
			}
			
			if ( !file->Exists() )
			{
				return frame.SetErrno( EACCES );
			}
			
			// FIXME: check permissions
		}
		catch ( ... )
		{
			return frame.SetErrnoFromException();
		}
		
		return 0;
	}
	
	
	static int fchmodat( int dirfd, const char* path, mode_t mode, int flags )
	{
		SystemCallFrame frame( "fchmodat" );
		
		try
		{
			FSTreePtr file = ResolvePathAt( dirfd, path );
			
			if ( const bool following_links = !(flags & AT_SYMLINK_NOFOLLOW) )
			{
				ResolveLinks_InPlace( file );
			}
			
			file->ChangeMode( mode );
		}
		catch ( ... )
		{
			return frame.SetErrnoFromException();
		}
		
		return 0;
	}
	
	static int fchmod( int fd, mode_t mode )
	{
		SystemCallFrame frame( "fchmod" );
		
		try
		{
			GetFileHandle( fd )->GetFile()->ChangeMode( mode );
		}
		catch ( ... )
		{
			return frame.SetErrnoFromException();
		}
		
		return 0;
	}
	
	
	static int fstatat( int dirfd, const char* path, struct stat* sb, int flags )
	{
		SystemCallFrame frame( "fstatat" );
		
		std::memset( (void*) sb, '\0', sizeof (struct stat) );
		
		try
		{
			FSTreePtr file = ResolvePathAt( dirfd, path );
			
			if ( const bool following_links = !(flags & AT_SYMLINK_NOFOLLOW) )
			{
				ResolveLinks_InPlace( file );
			}
			
			file->Stat( *sb );
		}
		catch ( ... )
		{
			return frame.SetErrnoFromException();
		}
		
		return 0;
	}
	
	
	static int fstat( int fd, struct stat* sb )
	{
		SystemCallFrame frame( "fstat" );
		
		std::memset( (void*) sb, '\0', sizeof (struct stat) );
		
		try
		{
			GetFileHandle( fd )->GetFile()->Stat( *sb );
		}
		catch ( ... )
		{
			return frame.SetErrnoFromException();
		}
		
		return 0;
	}
	
	#pragma force_active on
	
	REGISTER_SYSTEM_CALL( faccessat );
	REGISTER_SYSTEM_CALL( fchmodat  );
	REGISTER_SYSTEM_CALL( fchmod    );
	REGISTER_SYSTEM_CALL( fstatat   );
	REGISTER_SYSTEM_CALL( fstat     );
	
	#pragma force_active reset
	
}

