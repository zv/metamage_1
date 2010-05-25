/*
	Mac/Script/Types/ScriptCode.hh
	-------------------------------
*/

#ifndef MAC_SCRIPT_TYPES_SCRIPTCODE_HH
#define MAC_SCRIPT_TYPES_SCRIPTCODE_HH

#ifndef __MACTYPES__
#include <MacTypes.h>
#endif
#ifndef __SCRIPT__
#include <Script.h>
#endif

// nucleus
#include "nucleus/enumeration_traits.hh"


namespace Mac
{
	
	enum ScriptCode
	{
		smSystemScript = ::smSystemScript,
		
		kScriptCode_Max = nucleus::enumeration_traits< ::ScriptCode >::max
	};
	
}

#endif
