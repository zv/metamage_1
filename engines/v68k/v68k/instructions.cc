/*
	instructions.cc
	---------------
*/

#include "v68k/instructions.hh"

// v68k
#include "v68k/fetches.hh"
#include "v68k/microcode.hh"


namespace v68k
{
	
	instruction decoded_MOVEP_to =
	{
		fetches_MOVEP,
		microcode_MOVEP_to
	};
	
	instruction decoded_MOVEP_from =
	{
		fetches_MOVEP,
		microcode_MOVEP_from
	};
	
	instruction decoded_LINK =
	{
		fetches_LINK,
		microcode_LINK
	};
	
	instruction decoded_UNLK =
	{
		fetches_ANY_minor_register_num,
		microcode_UNLK
	};
	
	instruction decoded_MOVE_to_USP =
	{
		fetches_ANY_minor_register_num,
		microcode_MOVE_to_USP,
		privileged
	};
	
	instruction decoded_MOVE_from_USP =
	{
		fetches_ANY_minor_register_num,
		microcode_MOVE_from_USP,
		privileged
	};
	
	instruction decoded_NOP =
	{
		fetches_none,
		microcode_NOP
	};
	
	instruction decoded_MOVEQ =
	{
		fetches_MOVEQ,
		microcode_MOVEQ
	};
	
	instruction decoded_EXG =
	{
		fetches_EXG,
		microcode_EXG
	};
	
}
