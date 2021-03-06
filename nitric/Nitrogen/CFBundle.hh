// Nitrogen/CFBundle.hh
// --------------------

// Part of the Nitrogen project.
//
// Written 2002-2004 by Lisa Lippincott and Marshall Clow.
//
// This code was written entirely by the above contributors, who place it
// in the public domain.


#ifndef NITROGEN_CFBUNDLE_HH
#define NITROGEN_CFBUNDLE_HH

#ifndef __CFBUNDLE__
#include <CFBundle.h>
#endif
#ifndef NITROGEN_CFBASE_HH
#include "Nitrogen/CFBase.hh"
#endif
#ifndef NITROGEN_CFARRAY_HH
#include "Nitrogen/CFArray.hh"
#endif
#ifndef NITROGEN_CFDICTIONARY_HH
#include "Nitrogen/CFDictionary.hh"
#endif
#ifndef NITROGEN_CFURL_HH
#include "Nitrogen/CFURL.hh"
#endif

namespace Nitrogen
  {
   template <> struct CFType_Traits< CFBundleRef >: Basic_CFType_Traits< CFBundleRef, ::CFBundleGetTypeID > {};

  }

namespace nucleus
  {
   template <> struct disposer_class< CFBundleRef >: disposer_class< Nitrogen::CFTypeRef >  {};
  }

namespace Nitrogen
  {
   
   class CFBundleGetMainBundle_Failed {};
   CFBundleRef CFBundleGetMainBundle();

   class CFBundleGetBundleWithIdentifier_Failed {};
   CFBundleRef CFBundleGetBundleWithIdentifier( CFStringRef bundleID );

   using ::CFBundleGetAllBundles;  // Is this really a "Get", not a "Copy"?
   
   class CFBundleCreate_Failed {};
   nucleus::owned<CFBundleRef> CFBundleCreate( CFAllocatorRef allocator, CFURLRef bundleURL );
   
   inline nucleus::owned<CFBundleRef> CFBundleCreate( CFURLRef bundleURL )
     {
      return Nitrogen::CFBundleCreate( kCFAllocatorDefault, bundleURL );
     }

   class CFBundleCreateBundlesFromDirectory_Failed {};
   nucleus::owned<CFArrayRef> CFBundleCreateBundlesFromDirectory( CFAllocatorRef allocator,
                                                         CFURLRef       directoryURL,
                                                         CFStringRef    bundleType );
   
   inline nucleus::owned<CFArrayRef> CFBundleCreateBundlesFromDirectory( CFURLRef    directoryURL,
                                                                CFStringRef bundleType )
     {
      return Nitrogen::CFBundleCreateBundlesFromDirectory( kCFAllocatorDefault, directoryURL, bundleType );
     }

   class CFBundleCopyBundleURL_Failed {};
   nucleus::owned<CFURLRef> CFBundleCopyBundleURL( CFBundleRef bundle );

   class CFBundleGetValueForInfoDictionaryKey_Failed {};
   CFTypeRef CFBundleGetValueForInfoDictionaryKey( CFBundleRef bundle,
                                                   CFStringRef key );

   class CFBundleGetInfoDictionary_Failed {};
   CFDictionaryRef CFBundleGetInfoDictionary( CFBundleRef bundle );

   class CFBundleGetLocalInfoDictionary_Failed {};
   CFDictionaryRef CFBundleGetLocalInfoDictionary( CFBundleRef bundle );

   /* ... */
   
   class CFBundleCopyResourceURL_Failed {};
   nucleus::owned<CFURLRef> CFBundleCopyResourceURL( CFBundleRef bundle,
                                            CFStringRef resourceName,
                                            CFStringRef resourceType,
                                            CFStringRef subDirName );
   
   /* ... */
   
   class CFBundleGetFunctionPointerForName_Failed {};
   void *CFBundleGetFunctionPointerForName( CFBundleRef bundle, CFStringRef functionName );

   template < class DesiredType >
   DesiredType CFBundleGetFunctionPointerForName( CFBundleRef bundle, CFStringRef functionName )
     {
      return reinterpret_cast< DesiredType >( Nitrogen::CFBundleGetFunctionPointerForName( bundle, functionName ) );
     }
  }

#endif
