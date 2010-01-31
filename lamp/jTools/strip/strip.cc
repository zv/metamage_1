/*
	strip.cc
	--------
	
	Copyright 2010, Joshua Juran
*/

// Standard C
#include <string.h>

// POSIX
#include <fcntl.h>
#include <unistd.h>
#include <sys/stat.h>

// libc
#include <alloca.h>

// iota
#include "iota/strings.hh"

// more-posix
#include "more/perror.hh"


#pragma exceptions off


static int strip( const char* path )
{
	struct stat stat_buffer;
	
	int status = stat( path, &stat_buffer );
	
	if ( status == -1 )
	{
		more::perror( "strip", path );
		
		return 1;
	}
	
	if ( !S_ISREG( stat_buffer.st_mode ) )
	{
		more::perror( "strip", path, "not a regular file" );
		
		return 1;
	}
	
	size_t length = strlen( path );
	
	char* res_path = (char*) alloca( length + STRLEN( "/res" ) + 1 );
	
	memcpy( res_path, path, length );
	
	memcpy( res_path + length, STR_LEN( "/res" ) + 1 );
	
	int res_fd = open( res_path, O_RDONLY | O_DIRECTORY );
	
	if ( res_fd == -1 )
	{
		more::perror( "strip", path, "file has no resource fork" );
		
		return 1;
	}
	
	unlinkat( res_fd, "SymC/0", 0 );
	unlinkat( res_fd, "SymN/0", 0 );
	unlinkat( res_fd, "Name/0", 0 );
	
	close( res_fd );
	
	return 0;
}

int main( int argc, char **argv )
{
	int exit_status = 0;
	
	for ( int i = 1;  i < argc;  ++i )
	{
		const char* path = argv[ i ];
		
		int result = strip( path );
		
		if ( result != 0 )
		{
			exit_status = 1;
		}
	}
	
	return exit_status;
}
