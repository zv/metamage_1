/*
	Mac/Files/Types/FSDirID.hh
	--------------------------
*/

#ifndef MAC_FILES_TYPES_FSDIRID_HH
#define MAC_FILES_TYPES_FSDIRID_HH

#ifndef __FILES__
#include <Files.h>
#endif

// nucleus
#ifndef NUCLEUS_ENUMERATIONTRAITS_HH
#include "nucleus/enumeration_traits.hh"
#endif


namespace Mac
{
	
	enum FSDirID
	{
		fsRtParID = ::fsRtParID,
		fsRtDirID = ::fsRtDirID,
		
		kFSDirID_Max = nucleus::enumeration_traits< UInt32 >::max
	};
	
}

#endif

