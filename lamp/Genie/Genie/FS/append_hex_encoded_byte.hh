/*
	append_hex_encoded_byte.hh
	--------------------------
	
	Copyright 2009, Joshua Juran
*/

#ifndef GENIE_FS_APPENDHEXENCODEDBYTE_HH
#define GENIE_FS_APPENDHEXENCODEDBYTE_HH


namespace plus
{
	
	class var_string;
	
}

namespace Genie
{
	
	void append_hex_encoded_byte( plus::var_string& s, unsigned char c );
	
}

#endif

