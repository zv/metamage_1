/*	============
 *	backtrace.cc
 *	============
 */

#include "recall/backtrace.hh"

// Standard C++
#include <algorithm>
#include <functional>
#include <string>

// Standard C/C++
#include <cstdio>

#ifdef __GNUC__
#include <cxxabi.h>
#endif

// Recall
#include "recall/demangle.hh"
#include "recall/mach_o.hh"
#include "recall/macsbug_symbols.hh"
#include "recall/name_filter.hh"
#include "recall/traceback_tables.hh"


#ifndef TARGET_CPU_PPC
	#ifdef __POWERPC__
		#define TARGET_CPU_PPC 1
	#else
		#define TARGET_CPU_PPC 0
	#endif
#endif


namespace recall
{
	
	template < class ReturnAddr > struct demangler_traits;
	
#ifdef __MACOS__
	
	template <> struct demangler_traits< return_address_68k >
	{
		static std::string demangle( const std::string& name )
		{
			return demangle_MWC68K( name );
		}
	};
	
	template <> struct demangler_traits< return_address_cfm >
	{
		static std::string demangle( const std::string& name )
		{
			return demangle_MWCPPC( name );
		}
	};
	
#endif
	
#ifdef __GNUC__
	
	template <> struct demangler_traits< return_address_native >
	{
		static std::string demangle( const std::string& name )
		{
			std::string result = name;
			
			// s/:.*//;
			result.resize( std::find( result.begin(), result.end(), ':' ) - result.begin() );
			
			const char* mangled_name = result.c_str();
			
			if ( std::equal( mangled_name, mangled_name + 3, "__Z" ) )
			{
				++mangled_name;
			}
			
			if ( char* unmangled = abi::__cxa_demangle( mangled_name, NULL, NULL, NULL ) )
			{
				result = unmangled;
				
				free( unmangled );
				
				return result;
			}
			
			return result;
		}
	};
	
#endif
	
#ifdef __ELF__
	
	static const char* find_symbol_name( return_address_native )
	{
		return NULL;
	}
	
#endif
	
	template < class SymbolPtr >
	inline std::string get_name_from_symbol_pointer( SymbolPtr symbol )
	{
		return symbol != NULL ? get_symbol_string( symbol ) : "???";
	}
	
	template < class ReturnAddr >
	inline std::string find_symbol_string( ReturnAddr addr )
	{
		return get_name_from_symbol_pointer( find_symbol_name( addr ) );
	}
	
	template < class ReturnAddr >
	inline std::string get_symbol_name( ReturnAddr addr )
	{
		return find_symbol_string( addr );
	}
	
	template < class ReturnAddr >
	inline std::string get_demangled_symbol_name( ReturnAddr addr )
	{
		std::string name = get_symbol_name( addr );
		
		try
		{
			return demangler_traits< ReturnAddr >::demangle( name );
		}
		catch ( ... )
		{
			return name;
		}
	}
	
	
	struct call_info
	{
		const void*  frame_pointer;
		const void*  return_address;
		const char*  arch;
		std::string  demangled_name;
	};
	
	static call_info get_call_info_from_return_address( const frame_data& call )
	{
		call_info result;
		
		result.frame_pointer  = call.frame_pointer;
		result.return_address = call.addr_native;
		
	#ifdef __MACOS__
		
		result.arch           = call.is_cfm ? "ppc" : "68k";
		result.demangled_name = call.is_cfm ? get_demangled_symbol_name( call.addr_cfm    )
		                                    : get_demangled_symbol_name( call.addr_native );
		
		result.demangled_name = filter_symbol( result.demangled_name );
		
	#else
		
		result.arch           = TARGET_CPU_PPC ? "ppc" : "x86";
		result.demangled_name = get_demangled_symbol_name( call.addr_native );
		
	#endif
		
		return result;
	}
	
	static std::string make_report_for_call( unsigned            offset,
	                                         const void*         frame,
	                                         const void*         addr,
	                                         const char*         arch,
	                                         const std::string&  name )
	{
		char buffer[ sizeof "1234567890: [<0x12345678|xyz>] \0" ];
		
		std::sprintf( buffer, "%2d: [%#.8x <%#.8x|%s>] \0", offset, frame, addr, arch );
		
		std::string result = buffer;
		
		result += name;
		result += "\n";
		
		return result;
	}
	
	static std::string make_report_from_call_chain( std::vector< call_info >::const_iterator  begin,
	                                                std::vector< call_info >::const_iterator  end )
	{
		unsigned offset = 0;
		
		std::string result;
		
		// It's important to use < instead of != if we might skip past the end
		for ( std::vector< call_info >::const_iterator it = begin;  it < end;  ++it, ++offset )
		{
			const call_info& info = *it;
			
			result += make_report_for_call( offset, info.frame_pointer,
			                                        info.return_address,
			                                        info.arch,
			                                        info.demangled_name );
		}
		
		result += "\n";
		
		return result;
	}
	
	std::string make_report_from_stack_crawl( std::vector< frame_data >::const_iterator  begin,
	                                          std::vector< frame_data >::const_iterator  end )
	{
		std::vector< call_info > call_chain;
		
		call_chain.resize( end - begin );
		
		std::transform( begin,
		                end,
		                call_chain.begin(),
		                std::ptr_fun( get_call_info_from_return_address ) );
		
		return make_report_from_call_chain( call_chain.begin(), call_chain.end() );
	}
	
	debugging_context::debugging_context()
	:
		its_stack_crawl( make_stack_crawl() )
	{
	}
	
}

