/*
	gui/new/scrollframe.cc
	----------------------
*/

#include "Genie/FS/gui/new/scrollframe.hh"

// Standard C++
#include <algorithm>

// plus
#include "plus/serialize.hh"

// Nitrogen
#include "Nitrogen/Controls.hh"

// Pedestal
#include "Pedestal/EmptyView.hh"
#include "Pedestal/Scrollbar.hh"
#include "Pedestal/Scroller_beta.hh"

// Genie
#include "Genie/FS/FSTree_Directory.hh"
#include "Genie/FS/FSTree_Property.hh"
#include "Genie/FS/node_method_set.hh"
#include "Genie/FS/ResolvePathname.hh"
#include "Genie/FS/ScrollerBase.hh"
#include "Genie/FS/subview.hh"
#include "Genie/FS/TrackScrollbar.hh"
#include "Genie/FS/Views.hh"
#include "Genie/FS/gui/new/scroller.hh"
#include "Genie/Utilities/simple_map.hh"


namespace Genie
{
	
	namespace N = Nitrogen;
	namespace Ped = Pedestal;
	
	
	struct ScrollFrameParameters
	{
		bool  itHasVertical;
		bool  itHasHorizontal;
		
		boost::intrusive_ptr< Ped::View >  itsSubview;
		
		plus::string  itsTargetPath;
		
		ScrollerProxy  itsTargetProxy;
		
		ScrollFrameParameters() : itHasVertical(),
		                          itHasHorizontal(),
		                          itsSubview( Ped::EmptyView::Get() )
		{
		}
	};
	
	typedef simple_map< const FSTree*, ScrollFrameParameters > ScrollFrameParametersMap;
	
	static ScrollFrameParametersMap gScrollFrameParametersMap;
	
	
	
	class Scrollbar : public Ped::Scrollbar
	{
		private:
			Scrollbar_UserData itsUserData;
		
		public:
			void Update( const Rect& bounds, const FSTree* viewKey );
	};
	
	static Ped::ScrollerAPI* RecoverScrollerFromControl( ControlRef control )
	{
		Scrollbar_UserData* userData = N::GetControlReference( control );
		
		ASSERT( userData      != NULL );
		ASSERT( userData->key != NULL );
		
		const FSTree* scrollFrame = userData->key;
		
		if ( ScrollFrameParameters* it = gScrollFrameParametersMap.find( scrollFrame ) )
		{
			Ped::ScrollerAPI* proxy = &it->itsTargetProxy;
			
			return proxy;
		}
		
		return NULL;
	}
	
	static void DebriefAfterTrack( ControlRef control )
	{
		Scrollbar_UserData* userData = N::GetControlReference( control );
		
		ASSERT( userData      != NULL );
		ASSERT( userData->key != NULL );
		
		const bool vertical = userData->vertical;
		
		const FSTree* scrollFrame = userData->key;
		
		if ( ScrollFrameParameters* it = gScrollFrameParametersMap.find( scrollFrame ) )
		{
			if ( Ped::ScrollerAPI* proxy = &it->itsTargetProxy )
			{
				int clientLength = vertical ? proxy->ClientHeight() : proxy->ClientWidth();
				int viewLength   = vertical ? proxy->ViewHeight  () : proxy->ViewWidth  ();
				int offset       = vertical ? proxy->GetVOffset  () : proxy->GetHOffset ();
				
				short max_offset = std::max( clientLength - viewLength, offset );
				
				N::SetControlMaximum( control, max_offset );
			}
		}
	}
	
	void Scrollbar::Update( const Rect& bounds, const FSTree* viewKey )
	{
		if ( Get() == NULL )
		{
			itsUserData.trackHook = &Ped::TrackScrollbar;
			itsUserData.fetchHook = &RecoverScrollerFromControl;
			itsUserData.afterHook = &DebriefAfterTrack;
			itsUserData.vertical  = bounds.bottom - bounds.top > bounds.right - bounds.left;
			itsUserData.key       = viewKey;
			
			Create( bounds, &itsUserData );
		}
		else
		{
			UpdateBounds( bounds );
		}
	}
	
	
	class ScrollFrame : public Ped::ScrollFrame
	{
		private:
			typedef const FSTree* Key;
			
			Key itsKey;
			
			Scrollbar  itsVertical;
			Scrollbar  itsHorizontal;
			
			Rect itsLastBounds;
		
		public:
			ScrollFrame( Key key ) : itsKey( key )
			{
			}
			
			const Rect& Bounds() const  { return itsLastBounds; }
			
			void SetBounds( const Rect& bounds );
			
			Scrollbar& GetHorizontal()  { return itsHorizontal; }
			Scrollbar& GetVertical  ()  { return itsVertical;   }
			
			void UpdateScrollbars();
			
			void Draw( const Rect& bounds, bool erasing );
			
			Ped::View& Subview();
	};
	
	
	void ScrollFrame::SetBounds( const Rect& bounds )
	{
		itsLastBounds = bounds;
		
		Rect aperture = bounds;
		
		const short kScrollbarThickness = 16;
		
		const short kOverlap = 1;
		
		const short kFootprint = kScrollbarThickness - kOverlap;
		
		const ScrollFrameParameters& params = gScrollFrameParametersMap[ itsKey ];
		
		if ( params.itHasHorizontal )
		{
			aperture.bottom -= kFootprint;
			
			Rect scrollbarBounds = itsLastBounds;
			
			scrollbarBounds.top = aperture.bottom;
			
			scrollbarBounds.left   -= kOverlap;
			scrollbarBounds.bottom += kOverlap;
			scrollbarBounds.right  += kOverlap - kFootprint;
			
			itsHorizontal.Update( scrollbarBounds, itsKey );
		}
		else
		{
			itsHorizontal.Clear();
		}
		
		if ( params.itHasVertical )
		{
			aperture.right -= kFootprint;
			
			Rect scrollbarBounds = itsLastBounds;
			
			scrollbarBounds.left = aperture.right;
			
			scrollbarBounds.top    -= kOverlap;
			scrollbarBounds.right  += kOverlap;
			scrollbarBounds.bottom += kOverlap - kFootprint;
			
			itsVertical.Update( scrollbarBounds, itsKey );
		}
		else
		{
			itsVertical.Clear();
		}
		
		Subview().SetBounds( aperture );
		
		UpdateScrollbars();
	}
	
	void ScrollFrame::UpdateScrollbars()
	{
		const ScrollFrameParameters& params = gScrollFrameParametersMap[ itsKey ];
		
		const FSTree* target = params.itsTargetProxy.Get();
		
		const Rect aperture = ApertureFromBounds( itsLastBounds );
		
		itsHorizontal.Adjust( GetScrollerClientWidth( target ),
		                      GetScrollerHOffset    ( target ),
		                      aperture.right - aperture.left );
		
		itsVertical.Adjust( GetScrollerClientHeight( target ),
		                    GetScrollerVOffset     ( target ),
		                    aperture.bottom - aperture.top );
	}
	
	
	void ScrollFrame::Draw( const Rect& bounds, bool erasing )
	{
		const ScrollFrameParameters& params = gScrollFrameParametersMap[ itsKey ];
		
		const bool rebind =    params.itHasHorizontal != !!itsHorizontal.Get()
		                    || params.itHasVertical   != !!itsVertical  .Get();
		
		if ( rebind )
		{
			SetBounds( bounds );
		}
		
		Subview().Draw( ApertureFromBounds( bounds ), erasing );
		
		UpdateScrollbars();
	}
	
	Ped::View& ScrollFrame::Subview()
	{
		boost::intrusive_ptr< Ped::View >& subview = gScrollFrameParametersMap[ itsKey ].itsSubview;
		
		if ( subview.get() == NULL )
		{
			subview = Ped::EmptyView::Get();
		}
		
		return *subview;
	}
	
	static boost::intrusive_ptr< Ped::View > CreateView( const FSTree* delegate )
	{
		return new ScrollFrame( delegate );
	}
	
	
	static void DestroyDelegate( const FSTree* delegate )
	{
		gScrollFrameParametersMap.erase( delegate );
	}
	
	
	static void scrollframe_target_symlink( const FSTree*        node,
	                                        const plus::string&  target_path );
	
	static node_method_set scrollframe_target_methods =
	{
		NULL,
		NULL,
		NULL,
		NULL,
		NULL,
		NULL,
		NULL,
		NULL,
		NULL,
		NULL,
		NULL,
		NULL,
		NULL,
		NULL,
		&scrollframe_target_symlink
	};
	
	class FSTree_ScrollFrame_target : public FSTree
	{
		public:
			FSTree_ScrollFrame_target( const FSTreePtr&     parent,
			                           const plus::string&  name )
			:
				FSTree( parent, name, S_IFLNK | 0777, &scrollframe_target_methods )
			{
			}
			
			void Delete() const;
			
			plus::string ReadLink() const;
	};
	
	static bool scrollframe_target_exists( const FSTree* view )
	{
		return gScrollFrameParametersMap[ view ].itsTargetProxy.Get() != NULL;
	}
	
	void FSTree_ScrollFrame_target::Delete() const
	{
		const FSTree* view = GetViewKey( this );
		
		ScrollFrameParameters& params = gScrollFrameParametersMap[ view ];
		
		params.itsTargetPath.reset();
		
		params.itsTargetProxy = ScrollerProxy( NULL );
		
		InvalidateWindowForView( view );
	}
	
	static void scrollframe_target_symlink( const FSTree*        node,
	                                        const plus::string&  target_path )
	{
		const FSTree* view = node->ParentRef().get();
		
		FSTreePtr target = ResolvePathname( target_path, view );
		
		FSTreePtr delegate = target->Lookup( plus::string::null );
		
		ScrollFrameParameters& params = gScrollFrameParametersMap[ view ];
		
		params.itsTargetPath  = target_path;
		params.itsTargetProxy = ScrollerProxy( delegate.get() );
		
		InvalidateWindowForView( view );
	}
	
	plus::string FSTree_ScrollFrame_target::ReadLink() const
	{
		const FSTree* view = GetViewKey( this );
		
		return gScrollFrameParametersMap[ view ].itsTargetPath;
	}
	
	
	namespace
	{
		
		boost::intrusive_ptr< Ped::View >& GetView( const FSTree* key, const plus::string& name )
		{
			return gScrollFrameParametersMap[ key ].itsSubview;
		}
		
		bool& Vertical( const FSTree* view )
		{
			return gScrollFrameParametersMap[ view ].itHasVertical;
		}
		
		bool& Horizontal( const FSTree* view )
		{
			return gScrollFrameParametersMap[ view ].itHasHorizontal;
		}
		
	}
	
	
	#define PROPERTY( prop )  &new_property, &property_params_factory< prop >::value
	
	typedef View_Property< plus::serialize_bool, Horizontal >  Horizontal_Property;
	typedef View_Property< plus::serialize_bool, Vertical   >  Vertical_Property;
	
	static FSTreePtr target_factory( const FSTreePtr&     parent,
	                                 const plus::string&  name,
	                                 const void*          args )
	{
		if ( const bool exists = scrollframe_target_exists( parent.get() ) )
		{
			return new FSTree_ScrollFrame_target( parent, name );
		}
		
		return new FSTree( parent, name, 0, &scrollframe_target_methods );
	}
	
	static const FSTree_Premapped::Mapping local_mappings[] =
	{
		{ "horizontal", PROPERTY( Horizontal_Property ) },
		{ "vertical",   PROPERTY( Vertical_Property   ) },
		
		{ "target", &target_factory },
		
		{ "v", &subview_factory, (const void*) static_cast< ViewGetter >( &GetView ) },
		
		{ NULL, NULL }
	};
	
	FSTreePtr New_FSTree_new_scrollframe( const FSTreePtr&     parent,
	                                      const plus::string&  name,
	                                      const void*          args )
	{
		return new FSTree_new_View( parent,
		                            name,
		                            &CreateView,
		                            local_mappings,
		                            &DestroyDelegate );
	}
	
}

