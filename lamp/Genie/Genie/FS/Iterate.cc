/*
	Iterate.cc
	----------
*/

#include "Genie/FS/Iterate.hh"

// Genie
#include "Genie/FS/FSTree.hh"
#include "Genie/FS/FSTreeCache_Impl.hh"


namespace Genie
{
	
	typedef boost::intrusive_ptr< const FSTreeCache > FSTreeCachePtr;
	
	class FSIterator_Cache : public FSIterator
	{
		private:
			FSTreeCachePtr  contents;
			std::size_t     nextOffset;
		
		public:
			FSIterator_Cache( const FSTreeCachePtr& cache ) : contents( cache ), nextOffset( 0 )  {}
			
			FSNode Get() const
			{
				const FSTreeCache_Impl* impl = static_cast< const FSTreeCache_Impl* >( contents.get() );
				
				return nextOffset < impl->size() ? impl->at( nextOffset )
				                                 : FSNode();
			}
			
			void Advance()  { ++nextOffset; }
			
			void Rewind()  { nextOffset = 0; }
			
			void Seek( off_t index )  { nextOffset = index; }
			
			off_t Tell() const  { return nextOffset; }
	};
	
	
	FSIterator::~FSIterator()
	{
	}
	
	FSIteratorPtr Iterate( const FSTreePtr& dir )
	{
		FSTreeCache_Impl cache;
		
		cache.push_back( FSNode( dir->Inode(),       "."  ) );
		cache.push_back( FSNode( dir->ParentInode(), ".." ) );
		
		dir->IterateIntoCache( cache );
		
		FSTreeCache_Impl* newCache = new FSTreeCache_Impl();
		
		FSTreeCachePtr cachePtr( newCache );
		
		swap( cache, *newCache );
		
		return FSIteratorPtr( new FSIterator_Cache( cachePtr ) );
	}
	
}

