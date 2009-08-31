/*	===============
 *	OpenTransport.h
 *	===============
 */

#ifndef CLASSICTOOLBOX_OPENTRANSPORT_H
#define CLASSICTOOLBOX_OPENTRANSPORT_H

// Nostalgia
#include "Nostalgia/OpenTransport.hh"

// Nitrogen
#include "Nitrogen/OpenTransport.h"


namespace Nitrogen
{
	
	inline void InitOpenTransport()
	{
		ThrowOSStatus( ::InitOpenTransport() );
	}
	
	using ::CloseOpenTransport;
	
	inline Nucleus::Owned< EndpointRef >
	//
	OTOpenEndpoint( Nucleus::Owned< OTConfigurationRef >  config,
	                TEndpointInfo*                        info = NULL )
	{
		::OSStatus err;
		
		EndpointRef result = ::OTOpenEndpoint( config.Release(),
		                                       OTOpenFlags( 0 ),
		                                       info,
		                                       &err );
		
		ThrowOSStatus( err );
		
		return Nucleus::Owned< EndpointRef >::Seize( result );
	}
	
}

#endif

