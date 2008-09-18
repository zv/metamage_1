/*	===========
 *	ListView.hh
 *	===========
 */

#ifndef PEDESTAL_LISTVIEW_HH
#define PEDESTAL_LISTVIEW_HH

#if PRAGMA_ONCE
#pragma once
#endif

// Nitrogen
#include "Nitrogen/Lists.h"

// Pedestal
#include "Pedestal/View.hh"


namespace Pedestal
{
	
	namespace N = Nitrogen;
	namespace NN = Nucleus;
	
	
	Rect Bounds( ListHandle list );
	Point ViewableRange( ListHandle list );
	Point ScrollableRange( ListHandle list );
	Point ScrollStep( ListHandle list );
	Point ScrollPosition( ListHandle list );
	
	
	class ListView
	{
		private:
			struct TextAttributes
			{
				TextAttributes();
			};
			
			TextAttributes textAttributes;
			NN::Owned< ListHandle > list;
		
		public:
			ListView( const Rect& bounds );
			
			ListHandle Get() const  { return list; }
			
			void MouseDown( const EventRecord& event );
			bool KeyDown  ( const EventRecord& event );
			
			void Activate( bool activating )  { N::LActivate( activating, list ); }
			
			void Update();
			
			void Resize( short width, short height );
	};
	
	inline Rect Bounds( const ListView& view )
	{
		return Bounds( view.Get() );
	}
	
	inline Point ViewableRange( const ListView& view )
	{
		return ViewableRange( view.Get() );
	}
	
	inline Point ScrollableRange( const ListView& view )
	{
		return ScrollableRange( view.Get() );
	}
	
	inline Point ScrollPosition( const ListView& view )
	{
		return ScrollPosition( view.Get() );
	}
	
}

#endif

