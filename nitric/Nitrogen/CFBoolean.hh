// Nitrogen/CFBoolean.hh
// ---------------------

// Part of the Nitrogen project.
//
// Written 2002-2004 by Lisa Lippincott and Marshall Clow.
//
// This code was written entirely by the above contributors, who place it
// in the public domain.


#ifndef NITROGEN_CFBOOLEAN_HH
#define NITROGEN_CFBOOLEAN_HH

#ifndef __CFNUMBER__
#include <CFNumber.h>
#endif
#ifndef NITROGEN_CFBASE_HH
#include "Nitrogen/CFBase.hh"
#endif
#ifndef NUCLEUS_CONVERT_HH
#include "nucleus/convert.hh"
#endif

namespace Nitrogen
  {
   using ::CFBooleanRef;
   
   template <> struct CFType_Traits< CFBooleanRef >: Basic_CFType_Traits< CFBooleanRef, ::CFBooleanGetTypeID > {};
  }

namespace nucleus
  {
   template <> struct disposer_traits< Nitrogen::CFBooleanRef >: disposer_traits<Nitrogen::CFTypeRef>  {};
  }

namespace Nitrogen
  {
   inline bool CFBooleanGetValue( const CFBooleanRef b ) { return ::CFBooleanGetValue( b ); }
   inline void CFShow( const CFBooleanRef b )            { ::CFShow( b ); }
  }
  
namespace nucleus
  {
   template <>
   struct converter< bool, Nitrogen::CFBooleanRef >: public std::unary_function< Nitrogen::CFBooleanRef, bool >
     {
      bool operator()( const Nitrogen::CFBooleanRef& in ) const
        {
         return Nitrogen::CFBooleanGetValue( in );
        }
     };

   template <>
   struct converter< Nitrogen::CFBooleanRef, bool >: public std::unary_function< bool, Nitrogen::CFBooleanRef >
     {
      Nitrogen::CFBooleanRef operator()( const bool& in ) const
        {
         return in ? kCFBooleanTrue : kCFBooleanFalse;
        }
     };
   
   template <>
   struct converter< nucleus::owned<Nitrogen::CFBooleanRef>, bool >: public std::unary_function< bool, nucleus::owned<Nitrogen::CFBooleanRef> >
     {
      nucleus::owned<Nitrogen::CFBooleanRef> operator()( const bool& in ) const
        {
         return Nitrogen::CFRetain( convert<Nitrogen::CFBooleanRef>( in ) );
        }
     };
  }

#endif