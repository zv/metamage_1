/*	===================
 *	FSTree_IconSuite.cc
 *	===================
 */

#include "Genie/FS/FSTree_IconSuite.hh"

// POSIX
#include <sys/stat.h>

// Genie
#include "Genie/FS/FSTree.hh"


namespace Genie
{
	
	namespace n = nucleus;
	namespace N = Nitrogen;
	
	
	class FSTree_IconSuite : public FSTree
	{
		private:
			typedef n::shared< N::IconSuiteRef > Value;
			
			Value itsIconSuite;
		
		public:
			FSTree_IconSuite( const FSTreePtr&     parent,
			                  const plus::string&  name,
			                  const Value&         iconSuite )
			:
				FSTree( parent, name, S_IFREG | 0400 ),
				itsIconSuite( iconSuite )
			{
			}
			
			void CopyFile( const FSTreePtr& destination ) const;
	};
	
	
	static n::shared< N::IconSuiteRef > gStoredIconSuite;
	
	void FSTree_IconSuite::CopyFile( const FSTreePtr& destination ) const
	{
		gStoredIconSuite = itsIconSuite;
		
		try
		{
			destination->Attach( FSTreePtr( this ) );
		}
		catch ( ... )
		{
			gStoredIconSuite.reset();
			
			throw;
		}
		
		gStoredIconSuite.reset();
	}
	
	
	FSTreePtr
	//
	New_FSTree_IconSuite( const FSTreePtr&                     parent,
			              const plus::string&                  name,
			              const n::shared< N::IconSuiteRef >&  iconSuite )
	{
		return new FSTree_IconSuite( parent, name, iconSuite );
	}
	
	const n::shared< N::IconSuiteRef >& Fetch_IconSuite()
	{
		return gStoredIconSuite;
	}
	
}

