/*
	node_method_set.hh
	------------------
*/

#ifndef GENIE_FS_NODEMETHODSET_HH
#define GENIE_FS_NODEMETHODSET_HH

// POSIX
#include <sys/types.h>

// plus
#include "plus/string.hh"

// Genie
#include "Genie/FS/FSTreePtr.hh"


// #include <time.h>
struct timespec;

// #include <sys/stat.h>
struct stat;


namespace Genie
{
	
	class FSTree;
	
	typedef FSTreePtr (*parent_method)( const FSTree* );
	
	typedef ino_t (*inode_method)( const FSTree* );
	
	typedef void (*stat_method)( const FSTree*, struct ::stat& );
	
	typedef void (*chmod_method)( const FSTree*, mode_t );
	
	typedef void (*touch_method)( const FSTree* );
	typedef void (*utime_method)( const FSTree*, const struct timespec times[2] );
	
	typedef void (*remove_method)( const FSTree* );
	
	typedef void (*rename_method)( const FSTree*, const FSTreePtr& );
	
	typedef void (*link_method)( const FSTree*, const FSTreePtr& );
	
	typedef void (*copyfile_method)( const FSTree*, const FSTreePtr& );
	
	typedef off_t (*geteof_method)( const FSTree* );
	typedef void  (*seteof_method)( const FSTree*, off_t );
	
	typedef plus::string (*readlink_method)( const FSTree* );
	
	typedef FSTreePtr (*resolve_method)( const FSTree* );
	
	typedef void (*symlink_method)( const FSTree*, const plus::string& );
	
	
	struct node_method_set
	{
		parent_method   parent;
		inode_method    parent_inode;
		stat_method     stat;
		chmod_method    chmod;
		touch_method    touch;
		utime_method    utime;
		remove_method   remove;
		rename_method   rename;
		link_method     link;
		copyfile_method copyfile;
		geteof_method   geteof;
		seteof_method   seteof;
		readlink_method readlink;
		resolve_method  resolve;
		symlink_method  symlink;
	};
	
}

#endif

