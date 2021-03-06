/*
	more/perror.cc
	--------------
	
	Copyright 2009 Joshua Juran
*/

#include "more/perror.hh"

// Standard C
#include <string.h>

// POSIX
#include <unistd.h>
#include <sys/types.h>
#include <sys/uio.h>

// Iota
#include "iota/strings.hh"


#pragma exceptions off


namespace more
{
	
	void perror( int errnum )
	{
		const char* error = strerror( errnum );
		
		struct iovec iov[] =
		{
			{ (void*) error, strlen( error ) },
			{ (void*) STR_LEN( "\n" )        },
		};
		
		(void) ::writev( STDERR_FILENO, iov, sizeof iov / sizeof iov[0] );
	}
	
	void perror( const char* s, int errnum )
	{
		const char* error = strerror( errnum );
		
		struct iovec iov[] =
		{
			{ (void*) s, strlen( s )         },
			{ (void*) STR_LEN( ": " )        },
			{ (void*) error, strlen( error ) },
			{ (void*) STR_LEN( "\n" )        },
		};
		
		if ( errnum == 0 )
		{
			iov[ 1 ].iov_len = 0;
			iov[ 2 ].iov_len = 0;
		}
		
		(void) ::writev( STDERR_FILENO, iov, sizeof iov / sizeof iov[0] );
	}
	
	void perror( const char* s1, const char* s2, int errnum )
	{
		const char* error = strerror( errnum );
		
		struct iovec iov[] =
		{
			{ (void*) s1, strlen( s1 )       },
			{ (void*) STR_LEN( ": " )        },
			{ (void*) s2, strlen( s2 )       },
			{ (void*) STR_LEN( ": " )        },
			{ (void*) error, strlen( error ) },
			{ (void*) STR_LEN( "\n" )        },
		};
		
		if ( errnum == 0 )
		{
			iov[ 3 ].iov_len = 0;
			iov[ 4 ].iov_len = 0;
		}
		
		(void) ::writev( STDERR_FILENO, iov, sizeof iov / sizeof iov[0] );
	}
	
	void perror( const char* s1, const char* s2, const char* s3 )
	{
		struct iovec iov[] =
		{
			{ (void*) s1, strlen( s1 ) },
			{ (void*) STR_LEN( ": " )  },
			{ (void*) s2, strlen( s2 ) },
			{ (void*) STR_LEN( ": " )  },
			{ (void*) s3, strlen( s3 ) },
			{ (void*) STR_LEN( "\n" )  },
		};
		
		(void) ::writev( STDERR_FILENO, iov, sizeof iov / sizeof iov[0] );
	}
	
}

