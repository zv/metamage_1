/*	=============
 *	PPCToolbox.cp
 *	=============
 */

// *** A-line Processing Instructions ***
// API: ClassicMacToolbox (not Carbon)

#include "ClassicToolbox/PPCToolbox.h"

// Nitrogen
#include "Nitrogen/OSStatus.h"


namespace Nitrogen
{
	
	// does nothing, but guarantees construction of theRegistration
	NUCLEUS_DEFINE_ERRORS_DEPENDENCY( PPCToolbox )
	
	
	static void RegisterPPCToolboxErrors();
	
	
#pragma force_active on
	
	class PPCToolboxErrorsRegistration
	{
		public:
			PPCToolboxErrorsRegistration()  { RegisterPPCToolboxErrors(); }
	};
	
	static PPCToolboxErrorsRegistration theRegistration;
	
#pragma force_active reset
	
	
#if CALL_NOT_IN_CARBON
	
	void IPCListPortsSync( IPCListPortsPBRec& pb )
	{
		ThrowOSStatus( ::IPCListPortsSync( &pb ) );
	}
	
	PortInfoRec IPCListPortsSync( const PPCPortRec& name, const LocationNameRec& location )
	{
		PortInfoRec portInfo;
		
		IPCListPortsPBRec pb = Nucleus::Make< IPCListPortsPBRec >( &name, &location, &portInfo );
		
		IPCListPortsSync( pb );
		
		return portInfo;
	}
	
	PortInfoRec IPCListPortsSync( const PPCPortRec& name )
	{
		PortInfoRec portInfo;
		
		IPCListPortsPBRec pb = Nucleus::Make< IPCListPortsPBRec >( &name, &portInfo );
		
		IPCListPortsSync( pb );
		
		return portInfo;
	}
	
#endif  // #if CALL_NOT_IN_CARBON
	
	void RegisterPPCToolboxErrors()
	{
		RegisterOSStatus< paramErr            >();
		RegisterOSStatus< memFullErr          >();
		RegisterOSStatus< userCanceledErr     >();
		RegisterOSStatus< notInitErr          >();
		RegisterOSStatus< nameTypeErr         >();
		RegisterOSStatus< noPortErr           >();
		RegisterOSStatus< noGlobalsErr        >();
		RegisterOSStatus< localOnlyErr        >();
		RegisterOSStatus< destPortErr         >();
		RegisterOSStatus< sessTableErr        >();
		RegisterOSStatus< noSessionErr        >();
		RegisterOSStatus< badReqErr           >();
		RegisterOSStatus< portNameExistsErr   >();
		RegisterOSStatus< noUserNameErr       >();
		RegisterOSStatus< userRejectErr       >();
		RegisterOSStatus< noResponseErr       >();
		RegisterOSStatus< portClosedErr       >();
		RegisterOSStatus< sessClosedErr       >();
		RegisterOSStatus< badPortNameErr      >();
		RegisterOSStatus< noDefaultUserErr    >();
		RegisterOSStatus< notLoggedInErr      >();
		RegisterOSStatus< noUserRefErr        >();
		RegisterOSStatus< networkErr          >();
		RegisterOSStatus< noInformErr         >();
		RegisterOSStatus< authFailErr         >();
		RegisterOSStatus< noUserRecErr        >();
		RegisterOSStatus< badServiceMethodErr >();
		RegisterOSStatus< badLocNameErr       >();
		RegisterOSStatus< guestNotAllowedErr  >();
		RegisterOSStatus< nbpDuplicate        >();
	}
	
}

