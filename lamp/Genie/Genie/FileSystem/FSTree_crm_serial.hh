/*	====================
 *	FSTree_crm_serial.hh
 *	====================
 */

#ifndef GENIE_FILESYSTEM_FSTREESYSMACCRMSERIAL_HH
#define GENIE_FILESYSTEM_FSTREESYSMACCRMSERIAL_HH

// Nucleus
#include "Nucleus/AdvanceUntilFailureContainer.h"
#include "Nucleus/IndexedContainer.h"

// ClassicToolbox
#include "ClassicToolbox/CRMSerialDevices.h"

// Genie
#include "Genie/FileSystem/FSTree_sys_mac_crm.hh"


namespace Genie
{
	
	struct CRMDeviceID_KeyName_Traits
	{
		typedef Nitrogen::CRMDeviceID Key;
		
		static std::string NameFromKey( const Key& key );
		
		static Key KeyFromName( const std::string& name );
	};
	
	class FSTree_sys_mac_crm_serial_N;
	
	struct sys_mac_crm_serial_Details : public CRMDeviceID_KeyName_Traits
	{
		typedef Nitrogen::CRMSerialDevice_Container Sequence;
		
		typedef FSTree_sys_mac_crm_serial_N ChildNode;
		
		static std::string Name()  { return "serial"; }
		
		static FSTreePtr Parent();
		
		static const Sequence& ItemSequence()  { return Nitrogen::CRMSerialDevices(); }
		
		static Key KeyFromValue( Sequence::const_reference ref )  { return Key( ref->crmDeviceID ); }
		
		static bool KeyIsValid( const Key& key );
	};
	
	typedef FSTree_Sequence< sys_mac_crm_serial_Details > FSTree_sys_mac_crm_serial;
	
	class FSTree_sys_mac_crm_serial_N : public FSTree_Virtual,
	                                    public CRMDeviceID_KeyName_Traits
	{
		private:
			Key itsKey;
		
		public:
			FSTree_sys_mac_crm_serial_N( const Key& key );
			
			std::string Name() const  { return NameFromKey( itsKey ); }
			
			FSTreePtr Parent() const  { return GetSingleton< FSTree_sys_mac_crm_serial >(); }
	};
	
}

#endif

