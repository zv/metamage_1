// in_port_t.hh
// ------------
//
// Maintained by Joshua Juran

// Part of the Nitrogen project.
//
// Written 2008 by Joshua Juran.
//
// This code was written entirely by the above contributor, who places it
// in the public domain.


#ifndef POSEVEN_TYPES_IN_PORT_T_HH
#define POSEVEN_TYPES_IN_PORT_T_HH

// POSIX
#include <netinet/in.h>

// nucleus
#ifndef NUCLEUS_ENUMERATIONTRAITS_HH
#include "nucleus/enumeration_traits.hh"
#endif


namespace poseven
{
	
	enum in_port_t
	{
		/*
		ipport_echo       = IPPORT_ECHO,
		ipport_discard    = IPPORT_DISCARD,
		ipport_systat     = IPPORT_SYSTAT,
		ipport_daytime    = IPPORT_DAYTIME,
		ipport_netstat    = IPPORT_NETSTAT,
		ipport_ftp        = IPPORT_FTP,
		ipport_telnet     = IPPORT_TELNET,
		ipport_smtp       = IPPORT_SMTP,
		ipport_timeserver = IPPORT_TIMESERVER,
		ipport_nameserver = IPPORT_NAMESERVER,
		ipport_whois      = IPPORT_WHOIS,
		ipport_mtp        = IPPORT_MTP,
		*/
		
		// ...
		
		in_port_t_max = nucleus::enumeration_traits< ::in_port_t >::max
	};
	
	namespace big_endian
	{
		
	#ifdef __LITTLE_ENDIAN__
		
		enum in_port_t
		{
			in_port_t_max = nucleus::enumeration_traits< ::in_port_t >::max
		};
		
	#else
		
		using poseven::in_port_t;
		
	#endif
		
	}
	
}

#endif

