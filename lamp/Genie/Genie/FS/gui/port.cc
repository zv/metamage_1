/*
	Genie/FS/gui/port.cc
	--------------------
*/

#include "Genie/FS/gui/port.hh"

// Standard C++
#include <set>

// Iota
#include "iota/strings.hh"

// plus
#include "plus/hexidecimal.hh"

// poseven
#include "poseven/types/errno_t.hh"

// Genie
#include "Genie/FS/basic_directory.hh"
#include "Genie/FS/FSTreeCache.hh"
#include "Genie/FS/ResolvePathname.hh"
#include "Genie/FS/gui/port/ADDR.hh"
#include "Genie/Utilities/canonical_32_bit_hex.hh"


namespace Genie
{
	
	namespace p7 = poseven;
	
	
	typedef std::set< const FSTree* > WindowMap;
	
	static WindowMap gWindowMap;
	
	
	static FSTreePtr window_lookup( const FSTreePtr& parent, const plus::string& name )
	{
		WindowMap::const_iterator it;
		
		const bool canonical = canonical_32_bit_hex::applies( name );
		
		if ( canonical )
		{
			const FSTree* key = (const FSTree*) plus::decode_32_bit_hex( name );
			
			it = gWindowMap.find( key );
		}
		
		if ( !canonical  ||  it == gWindowMap.end() )
		{
			p7::throw_errno( ENOENT );
		}
		
		return FSTreePtr( *it );
	}
	
	static void window_iterate( const FSTreePtr& parent, FSTreeCache& cache )
	{
		WindowMap::const_iterator end = gWindowMap.end();
		
		for ( WindowMap::const_iterator it = gWindowMap.begin();  it != end;  ++it )
		{
			const FSTree* window = *it;
			
			ino_t inode = (ino_t) window;  // coerce pointer to integer
			
			FSNode node( inode, window->Name() );
			
			cache.push_back( node );
		}
	}
	
	void remove_port( const FSTree* port )
	{
		remove_window_and_views_from_port( port );
		
		gWindowMap.erase( port );
	}
	
	static const FSTreePtr& SysWindow()
	{
		static FSTreePtr sys_window = ResolveAbsolutePath( STR_LEN( "/gui/port" ) );
		
		return sys_window;
	}
	
	FSTreePtr new_port()
	{
		const FSTreePtr& parent = SysWindow();
		
		FSTreePtr window = Premapped_Factory( parent, "/", sys_port_ADDR_Mappings, &remove_port );
		
		gWindowMap.insert( window.get() );
		
		return window;
	}
	
	FSTreePtr New_sys_port( const FSTreePtr&     parent,
	                        const plus::string&  name,
	                        const void*          args )
	{
		return new_basic_directory( parent, name, window_lookup, window_iterate );
	}
	
}

