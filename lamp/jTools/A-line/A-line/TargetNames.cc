/*	==============
 *	TargetNames.cc
 *	==============
 */

#include <cstdio>

// plus
#include "plus/var_string.hh"

#include "A-line/TargetNames.hh"


namespace tool
{
	
	class InvalidPlatform {};
	
	static const char* ArchName( Platform arch )
	{
		switch ( arch )
		{
			case arch68K:  return "68k";
			case archPPC:  return "ppc";
			case archX86:  return "x86";
		};
		
		std::fprintf( stderr, "Invalid arch: %x\n", arch );
		
		throw InvalidPlatform();
	}
	
	static const char* RuntimeName( Platform rt )
	{
		switch ( rt )
		{
			case runtimeA4CodeResource:  return "a4";
			case runtimeA5CodeSegments:  return "a5";
			case runtimeCodeFragments:   return "cfm";
			case runtimeMachO:           return "mach";
		};
		
		return "";
	}
	
	static const char* APIName( Platform api )
	{
		switch ( api )
		{
			case apiMacBlue:    return "blue";
			case apiMacCarbon:  return "carb";
		};
		
		return "";
	}
	
	static const char* BuildName( BuildVariety build )
	{
		switch ( build )
		{
			case buildSymbolics:  return "sym";
			case buildDebug:      return "dbg";
			case buildRelease:    return "opt";
		};
		
		return "";
	}
	
	plus::string MakeTargetName( Platform      arch,
	                             Platform      runtime,
	                             Platform      api,
	                             BuildVariety  build )
	{
		plus::var_string name;
		
	#if ALINE_CROSS_DEVELOPMENT
		
		name += ArchName   ( arch    );
		name += "-";
		name += RuntimeName( runtime );
		name += "-";
		name += APIName    ( api     );
		name += "-";
		
	#endif
		
		name += BuildName( build   );
		
		return name.move();
	}
	
}

