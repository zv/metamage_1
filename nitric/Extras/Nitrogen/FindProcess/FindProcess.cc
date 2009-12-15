/*
	FindProcess.cc
	--------------
	
	Copyright 2009, Joshua Juran
*/

#include "FindProcess.hh"

// Nitrogen
#include "Nitrogen/Files.h"
#include "Nitrogen/Processes.h"


namespace NitrogenExtras
{
	
	namespace N = Nitrogen;
	
	
	class Process_HasSignature
	{
		private:
			N::OSType itsSignature;
		
		public:
			Process_HasSignature( N::OSType signature )
			:
				itsSignature( signature )
			{
			}
			
			bool operator()( const ProcessSerialNumber& psn ) const
			{
				ProcessInfoRec processInfo = N::GetProcessInformation( psn );
				
				return processInfo.processSignature == itsSignature;
			}
	};
	
	class Process_HasAppSpec
	{
		private:
			FSSpec itsAppSpec;
		
		public:
			Process_HasAppSpec( const FSSpec& appSpec )
			:
				itsAppSpec( appSpec )
			{
			}
			
			bool operator()( const ProcessSerialNumber& psn ) const
			{
				FSSpec appSpec = N::GetProcessAppSpec( psn );
				
				return N::FSCompareFSSpecs( appSpec, itsAppSpec );
			}
	};
	
	template < class Predicate >
	static inline ProcessSerialNumber FindProcess( Predicate predicate )
	{
		ProcessSerialNumber psn = N::NoProcess();
		
		while ( true )
		{
			psn = N::GetNextProcess( psn );
			
			if ( predicate( psn ) )
			{
				break;
			}
		}
		
		return psn;
	}
	
	ProcessSerialNumber FindProcess( N::OSType signature )
	{
		return FindProcess( Process_HasSignature( signature ) );
	}
	
	ProcessSerialNumber FindProcess( const FSSpec& appFile )
	{
		return FindProcess( Process_HasAppSpec( appFile ) );
	}
	
}
