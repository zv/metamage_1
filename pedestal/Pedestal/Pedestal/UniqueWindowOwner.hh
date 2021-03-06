/*	====================
 *	UniqueWindowOwner.hh
 *	====================
 */

#ifndef PEDESTAL_UNIQUEWINDOWOWNER_HH
#define PEDESTAL_UNIQUEWINDOWOWNER_HH

// Standard C++
#include <memory>

// Pedestal
#ifndef PEDESTAL_WINDOW_HH
#include "Pedestal/Window.hh"
#endif


namespace Pedestal
{
	
	// A unique window such as a modeless about box that's sometimes open and sometimes not.
	class UniqueWindowOwner
	{
		typedef std::auto_ptr< Window > (*Factory)();
		
		class CloseHandler : public WindowCloseHandler
		{
			private:
				std::auto_ptr< Window >& itsStorage;
			
			public:
				CloseHandler( std::auto_ptr< Window >& storage ) : itsStorage( storage )
				{
				}
				
				void operator()( WindowRef window ) const
				{
					// assert( itsStorage.get() );
					// assert( itsStorage->Get() == window );
					
					itsStorage.reset( NULL );
				}
		};
		
		private:
			std::auto_ptr< Window >                     itsWindow;
			boost::intrusive_ptr< WindowCloseHandler >  itsCloseHandler;
			Factory                                     itsFactory;
		
		public:
			UniqueWindowOwner( Factory f )
			:
				itsCloseHandler( new CloseHandler( itsWindow ) ),
				itsFactory( f )
			{
			}
			
			Window& Get()  { return *itsWindow.get(); }
			
			void Show();
	};
	
}

#endif

