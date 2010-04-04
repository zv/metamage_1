/*	================
 *	CommResources.cc
 *	================
 */

#include "ClassicToolbox/CommResources.hh"


namespace Nitrogen
{
	
	// does nothing, but guarantees construction of theRegistration
	NUCLEUS_DEFINE_ERRORS_DEPENDENCY( CommunicationResourceManager )
	
	
	static void RegisterCommunicationResourceManagerErrors();
	
	
#if NUCLEUS_RICH_ERRORCODES
#pragma force_active on
	
	class CommunicationResourceManagerErrorsRegistration
	{
		public:
			CommunicationResourceManagerErrorsRegistration()  { RegisterCommunicationResourceManagerErrors(); }
	};
	
	static CommunicationResourceManagerErrorsRegistration theRegistration;
	
#pragma force_active reset
#endif
	
	
	void CRMAttributesDisposer::Dispose( CRMDeviceType crmDeviceType, CRMAttributes crmAttributes )
	{
		Map::const_iterator found = map.find( crmDeviceType );
		if ( found != map.end() )
		{
			return found->second( crmAttributes );
		}
		// FIXME:  Assert
	}
	
	CRMAttributesDisposer& TheGlobalCRMAttributesDisposer()
	{
		static CRMAttributesDisposer theGlobalCRMAttributesDisposer;
		return theGlobalCRMAttributesDisposer;
	}
	
	void DisposeCRMAttributes( CRMDeviceType crmDeviceType, CRMAttributes crmAttributes )
	{
		TheGlobalCRMAttributesDisposer().Dispose( crmDeviceType, crmAttributes );
	}
	
	void InitCRM()
	{
		ThrowOSStatus( ::InitCRM() );
	}
	
	nucleus::owned< CRMRecPtr, CRMRemover > CRMInstall( nucleus::owned< CRMRecPtr > crmRec )
	{
		// ::CRMInstall() returns void.  There's no means for reporting errors.
		::CRMInstall( crmRec.get() );
		return nucleus::owned< CRMRecPtr, CRMRemover >::seize( crmRec.release() );
	}
	
	nucleus::owned< CRMRecPtr > CRMRemove( nucleus::owned< CRMRecPtr, CRMRemover > crmRec )
	{
		ThrowOSStatus( ::CRMRemove( crmRec.get() ) );
		
		return nucleus::owned< CRMRecPtr >::seize( crmRec.release() );
	}
	
	CRMRecPtr CRMSearch( const CRMRec& crmRec )
	{
		CRMRecPtr result = ::CRMSearch( const_cast< CRMRecPtr >( &crmRec ) );
		
		return result;
	}
	
	CRMRecPtr CRMSearch( CRMDeviceType crmDeviceType, CRMDeviceID crmDeviceID )
	{
		CRMRec crmRec;
		crmRec.crmDeviceType = crmDeviceType;
		crmRec.crmDeviceID   = crmDeviceID;
		
		return CRMSearch( crmRec );
	}
	
	nucleus::owned< CRMRecPtr > New_CRMRecord()
	{
		nucleus::owned< CRMRecPtr > result = nucleus::owned< CRMRecPtr >::seize
		(
			reinterpret_cast< CRMRecPtr >
			(
				NewPtrSysClear( sizeof (CRMRec) ).release().Get()
			)
		);
		
		CRMRecPtr crmRec = result;
		
		crmRec->qType = crmType;
		crmRec->crmDeviceID = 0;
		
		return result;
	}
	
	void RegisterCommunicationResourceManagerErrors()
	{
		// FIXME
	}
	
	namespace Tests
	{
		
		static void Test1( CRMDeviceType crmDeviceType )
		{
			CRMResource_Container crmResources = CRMResources( crmDeviceType );
		}
	}
	
}
