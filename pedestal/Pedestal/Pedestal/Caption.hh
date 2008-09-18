/*	==========
 *	Caption.hh
 *	==========
 */

#ifndef PEDESTAL_CAPTION_HH
#define PEDESTAL_CAPTION_HH

#if PRAGMA_ONCE
#pragma once
#endif

// Standard C++
#include <string>

// Pedestal
#include "Pedestal/Graphic.hh"


namespace Pedestal
{
	
	class Caption : public Graphic
	{
		private:
			std::string text;
		
		public:
			typedef std::string Initializer;
			
			Caption( const std::string& text ) : text( text )  {}
			
			std::string Text() const  { return text; }
			
			void SetText( const std::string& newText )  { text = newText; }
			
			void Plot( const Rect& area );
	};
	
}

#endif

