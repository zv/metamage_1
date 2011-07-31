/*
	instruction.hh
	--------------
*/

#ifndef V68K_INSTRUCTION_HH
#define V68K_INSTRUCTION_HH

// v68k
#include "v68k/fetcher.hh"
#include "v68k/microcode.hh"


namespace v68k
{
	
	enum instruction_flags_t
	{
		privileged_except_on_68000 = 0x01,
		privileged                 = 0x02,
		privilege_mask             = 0x03
	};
	
	struct instruction
	{
		typedef instruction_flags_t flags_t;
		
		fetcher*   fetch;
		microcode  code;
		flags_t    flags;
	};
	
}

#endif
