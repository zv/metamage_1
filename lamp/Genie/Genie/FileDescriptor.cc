/*	=================
 *	FileDescriptor.cc
 *	=================
 */

#include "Genie/FileDescriptor.hh"

// iota
#include "iota/swap.hh"

// Genie
#include "Genie/IO/Base.hh"


namespace Genie
{
	
	FileDescriptor::FileDescriptor()
	:
		closeOnExec( false )
	{
	}
	
	FileDescriptor::FileDescriptor( const IOPtr& handle )
	:
		handle     ( handle ),
		closeOnExec( false  )
	{
	}
	
	FileDescriptor::FileDescriptor( const FileDescriptor& other )
	:
		handle     ( other.handle      ),
		closeOnExec( other.closeOnExec )
	{
	}
	
	FileDescriptor::~FileDescriptor()
	{
	}
	
	FileDescriptor& FileDescriptor::operator=( const FileDescriptor& other )
	{
		handle      = other.handle;
		closeOnExec = other.closeOnExec;
		
		return *this;
	}
	
	void FileDescriptor::swap( FileDescriptor& other )
	{
		using iota::swap;
		
		swap( handle,      other.handle      );
		swap( closeOnExec, other.closeOnExec );
	}
	
}

