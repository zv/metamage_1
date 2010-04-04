// Nitrogen/CFURL.hh
// -----------------

// Part of the Nitrogen project.
//
// Written 2002-2006 by Lisa Lippincott and Marshall Clow.
//
// This code was written entirely by the above contributors, who place it
// in the public domain.


#ifndef NITROGEN_CFURL_HH
#define NITROGEN_CFURL_HH

#ifndef __CFURL__
#include <CFURL.h>
#endif

#ifndef NITROGEN_CFBASE_HH
#include "Nitrogen/CFBase.hh"
#endif
#ifndef NITROGEN_CFDATA_HH
#include "Nitrogen/CFData.hh"
#endif
#ifndef NITROGEN_FILES_HH
#include "Nitrogen/Files.hh"
#endif

#include <string>

namespace Nitrogen
  {
   using ::CFURLPathStyle;
   using ::CFURLRef;
   
   using ::CFURLGetTypeID;
   
   template <> struct CFType_Traits< CFURLRef >: Basic_CFType_Traits< CFURLRef, ::CFURLGetTypeID > {};

  }

namespace nucleus
  {
   template <> struct disposer_traits< Nitrogen::CFURLRef >: disposer_traits< Nitrogen::CFTypeRef >  {};
  }

namespace Nitrogen
  {
   inline void CFShow( const CFURLRef u )        { ::CFShow( u ); }

   class CFURLCreateWithBytes_Failed {};
   nucleus::owned<CFURLRef> CFURLCreateWithBytes( CFAllocatorRef     allocator,
                                         const UInt8 *      URLBytes,
                                         std::size_t        length,
                                         CFStringEncoding   encoding,
                                         CFURLRef           baseURL = 0 );

   inline nucleus::owned<CFURLRef> CFURLCreateWithBytes( const UInt8 *      URLBytes,
                                                std::size_t        length,
                                                CFStringEncoding   encoding,
                                                CFURLRef           baseURL = 0 )
     {
      return Nitrogen::CFURLCreateWithBytes( kCFAllocatorDefault, URLBytes, length, encoding, baseURL );
     }

   nucleus::owned<CFURLRef> CFURLCreateWithBytes( CFAllocatorRef     allocator,
                                         const std::string& URLBytes,
                                         CFStringEncoding   encoding,
                                         CFURLRef           baseURL = 0 );

   inline nucleus::owned<CFURLRef> CFURLCreateWithBytes( const std::string& URLBytes,
                                                CFStringEncoding   encoding,
                                                CFURLRef           baseURL = 0 )
     {
      return Nitrogen::CFURLCreateWithBytes( kCFAllocatorDefault, URLBytes, encoding, baseURL );
     }

   class CFURLCreateData_Failed {};
   nucleus::owned<CFDataRef> CFURLCreateData( CFAllocatorRef     allocator,
                                     CFURLRef           url,
                                     CFStringEncoding   encoding,
                                     bool               escapeWhitespace );
   
   inline nucleus::owned<CFDataRef> CFURLCreateData( CFURLRef           url,
                                            CFStringEncoding   encoding,
                                            Boolean            escapeWhitespace )
     {
      return Nitrogen::CFURLCreateData( kCFAllocatorDefault, url, encoding, escapeWhitespace );
     }

   class CFURLCreateWithString_Failed {};
   nucleus::owned<CFURLRef> CFURLCreateWithString( CFAllocatorRef   allocator,
                                          CFStringRef      URLString,
                                          CFURLRef         baseURL = 0 );

   inline nucleus::owned<CFURLRef> CFURLCreateWithString( CFStringRef      URLString,
                                                 CFURLRef         baseURL = 0 )
     {
      return Nitrogen::CFURLCreateWithString( kCFAllocatorDefault, URLString, baseURL );
     }

   class CFURLCreateWithFileSystemPath_Failed {};
   nucleus::owned<CFURLRef> CFURLCreateWithFileSystemPath( CFAllocatorRef   allocator,
                                                  CFStringRef      filePath,
                                                  CFURLPathStyle   pathStyle,
                                                  bool             isDirectory );

   inline nucleus::owned<CFURLRef> CFURLCreateWithFileSystemPath( CFStringRef      filePath,
                                                         CFURLPathStyle   pathStyle,
                                                         bool             isDirectory )
     {
      return Nitrogen::CFURLCreateWithFileSystemPath( kCFAllocatorDefault, filePath, pathStyle, isDirectory );
     }


   class CFURLCreateFromFileSystemRepresentation_Failed {};
   nucleus::owned<CFURLRef> CFURLCreateFromFileSystemRepresentation( CFAllocatorRef   allocator,
                                                            const UInt8 *    buffer,
                                                            CFIndex          bufLen,
                                                            bool             isDirectory );

   inline nucleus::owned<CFURLRef> CFURLCreateFromFileSystemRepresentation( const UInt8 *    buffer,
                                                                   CFIndex          bufLen,
                                                                   bool             isDirectory )
     {
      return Nitrogen::CFURLCreateFromFileSystemRepresentation( kCFAllocatorDefault, buffer, bufLen, isDirectory );
     }

   class CFURLCreateWithFileSystemPathRelativeToBase_Failed {};
   nucleus::owned<CFURLRef> CFURLCreateWithFileSystemPathRelativeToBase( CFAllocatorRef   allocator,
                                                                CFStringRef      filePath,
                                                                CFURLPathStyle   pathStyle,
                                                                bool             isDirectory,
                                                                CFURLRef         baseURL );

   inline nucleus::owned<CFURLRef> CFURLCreateWithFileSystemPathRelativeToBase( CFStringRef      filePath,
                                                                       CFURLPathStyle   pathStyle,
                                                                       bool             isDirectory,
                                                                       CFURLRef         baseURL )
     {
      return Nitrogen::CFURLCreateWithFileSystemPathRelativeToBase( kCFAllocatorDefault, filePath, pathStyle, isDirectory, baseURL );
     }

   class CFURLCreateFromFileSystemRepresentationRelativeToBase_Failed {};
   nucleus::owned<CFURLRef> CFURLCreateFromFileSystemRepresentationRelativeToBase( CFAllocatorRef   allocator,
                                                                          const UInt8 *    buffer,
                                                                          CFIndex          bufLen,
                                                                          bool             isDirectory,
                                                                          CFURLRef         baseURL );

   inline nucleus::owned<CFURLRef> CFURLCreateFromFileSystemRepresentationRelativeToBase( const UInt8 *    buffer,
                                                                                 CFIndex          bufLen,
                                                                                 bool             isDirectory,
                                                                                 CFURLRef         baseURL )
     {
      return Nitrogen::CFURLCreateFromFileSystemRepresentationRelativeToBase( kCFAllocatorDefault, buffer, bufLen, isDirectory, baseURL );
     }

   class CFURLGetFileSystemRepresentation_Failed {};
   void CFURLGetFileSystemRepresentation( CFURLRef   url,
                                          bool       resolveAgainstBase,
                                          UInt8 *    buffer,
                                          CFIndex    maxBufLen );

   template < CFIndex maxBufLen >
   inline void CFURLGetFileSystemRepresentation( CFURLRef   url,
                                                 bool       resolveAgainstBase,
                                                 UInt8      (&buffer)[maxBufLen] )
     {
      Nitrogen::CFURLGetFileSystemRepresentation( url, resolveAgainstBase, buffer, maxBufLen );
     }

	class CFURLCopyAbsoluteURL_Failed {};
	nucleus::owned<CFURLRef> CFURLCopyAbsoluteURL( CFURLRef relativeURL );
	
	class CFURLGetString_Failed {};
	CFStringRef CFURLGetString( CFURLRef anURL );
	
	using ::CFURLGetBaseURL;
	
	class CFURLCopyFileSystemPath_Failed {};
	inline nucleus::owned<CFStringRef> CFURLCopyFileSystemPath ( CFURLRef anURL, CFURLPathStyle pathStyle = kCFURLPOSIXPathStyle ) {
		CFStringRef result = ::CFURLCopyFileSystemPath ( anURL, pathStyle );
		if ( NULL == result ) throw CFURLCopyFileSystemPath_Failed ();
		return nucleus::owned<CFStringRef>::seize ( result );
		}
	
	class CFURLCreateCopyDeletingLastPathComponent_Failed {};
	inline nucleus::owned<CFURLRef> CFURLCreateCopyDeletingLastPathComponent ( CFAllocatorRef allocator, CFURLRef anURL ) {
		CFURLRef result = ::CFURLCreateCopyDeletingLastPathComponent ( allocator, anURL );
		if ( NULL == result ) throw CFURLCreateCopyDeletingLastPathComponent_Failed ();
		return nucleus::owned<CFURLRef>::seize ( result );
		}
	
	
	class CFURLCopyLastPathComponent_Failed {};
	inline nucleus::owned<CFStringRef> CFURLCopyLastPathComponent ( CFURLRef anURL ) {
		CFStringRef result = ::CFURLCopyLastPathComponent ( anURL );
		if ( NULL == result ) throw CFURLCopyLastPathComponent_Failed ();
		return nucleus::owned<CFStringRef>::seize ( result );
		}
	
	//	CFMutableStringRef CFStringCreateMutableCopy(CFAllocatorRef alloc, CFIndex maxLength, CFStringRef theString);
	class CFStringCreateMutableCopy_Failed {};
	inline nucleus::owned<CFMutableStringRef> CFStringCreateMutableCopy ( CFAllocatorRef alloc, CFIndex maxLength, CFStringRef theString ) {
		CFMutableStringRef result = ::CFStringCreateMutableCopy ( alloc, maxLength, theString );
		if ( NULL == result ) throw CFStringCreateMutableCopy_Failed ();
		return nucleus::owned<CFMutableStringRef>::seize ( result );
		}
	

   /*...*/

   class CFURLCreateFromFSRef_Failed {};
   nucleus::owned<CFURLRef> CFURLCreateFromFSRef( CFAllocatorRef   allocator,
                                         const FSRef&     fsRef );
   
   inline nucleus::owned<CFURLRef> CFURLCreateFromFSRef( const FSRef& fsRef )
     {
      return Nitrogen::CFURLCreateFromFSRef( kCFAllocatorDefault, fsRef );
     }

   class CFURLGetFSRef_Failed {};
   FSRef CFURLGetFSRef( CFURLRef url );
   
  }

namespace nucleus
  {
  template <> struct converter< Nitrogen::FSRef, Nitrogen::CFURLRef >: public std::unary_function< Nitrogen::CFURLRef, Nitrogen::FSRef >
     {
      Nitrogen::FSRef operator()( Nitrogen::CFURLRef url ) const;
     };

   template <> struct converter< nucleus::owned<Nitrogen::CFURLRef>, Nitrogen::FSRef >: public std::unary_function< Nitrogen::FSRef, nucleus::owned<Nitrogen::CFURLRef> >
     {
      nucleus::owned<Nitrogen::CFURLRef> operator()( const Nitrogen::FSRef& ref ) const
        {
         return Nitrogen::CFURLCreateFromFSRef( ref );
        }
     };
  }

#endif