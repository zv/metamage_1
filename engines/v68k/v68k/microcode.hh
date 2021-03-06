/*
	microcode.hh
	------------
*/

#ifndef V68K_MICROCODE_HH
#define V68K_MICROCODE_HH

// v68k
#include "v68k/op_params.hh"


namespace v68k
{
	
	struct processor_state;
	
	
	typedef void (*microcode)( processor_state&, op_params& );
	
	
	#pragma mark -
	#pragma mark Line 0
	
	void microcode_ORI_to_CCR( processor_state& state, op_params& pb );
	
	void microcode_ORI_to_SR( processor_state& state, op_params& pb );
	
	void microcode_BCHG( processor_state& state, op_params& pb );
	void microcode_BCLR( processor_state& state, op_params& pb );
	void microcode_BSET( processor_state& state, op_params& pb );
	
	void microcode_MOVEP_to  ( processor_state& state, op_params& pb );
	void microcode_MOVEP_from( processor_state& state, op_params& pb );
	
	void microcode_ANDI_to_CCR( processor_state& state, op_params& pb );
	
	void microcode_ANDI_to_SR( processor_state& state, op_params& pb );
	
	void microcode_EORI_to_CCR( processor_state& state, op_params& pb );
	
	void microcode_EORI_to_SR( processor_state& state, op_params& pb );
	
	void microcode_MOVES( processor_state& state, op_params& pb );
	
	#pragma mark -
	#pragma mark Lines 1-3
	
	void microcode_MOVE( processor_state& state, op_params& pb );
	
	#pragma mark -
	#pragma mark Line 4
	
	void microcode_CHK( processor_state& state, op_params& pb );
	
	void microcode_LEA( processor_state& state, op_params& pb );
	
	void microcode_MOVE_from_CCR( processor_state& state, op_params& pb );
	
	void microcode_MOVE_from_SR( processor_state& state, op_params& pb );
	
	void microcode_MOVE_to_CCR( processor_state& state, op_params& pb );
	
	void microcode_MOVE_to_SR( processor_state& state, op_params& pb );
	
	void microcode_SWAP( processor_state& state, op_params& pb );
	
	void microcode_BKPT( processor_state& state, op_params& pb );
	
	void microcode_PEA( processor_state& state, op_params& pb );
	
	void microcode_EXT_W( processor_state& state, op_params& pb );
	void microcode_EXT_L( processor_state& state, op_params& pb );
	
	void microcode_EXTB( processor_state& state, op_params& pb );
	
	void microcode_TAS( processor_state& state, op_params& pb );
	
	void microcode_MOVEM_to  ( processor_state& state, op_params& pb );
	void microcode_MOVEM_from( processor_state& state, op_params& pb );
	
	void microcode_TRAP( processor_state& state, op_params& pb );
	
	void microcode_LINK( processor_state& state, op_params& pb );
	
	void microcode_UNLK( processor_state& state, op_params& pb );
	
	void microcode_MOVE_to_USP  ( processor_state& state, op_params& pb );
	void microcode_MOVE_from_USP( processor_state& state, op_params& pb );
	
	void microcode_NOP( processor_state& state, op_params& pb );
	
	void microcode_STOP( processor_state& state, op_params& pb );
	
	void microcode_RTE( processor_state& state, op_params& pb );
	
	void microcode_RTD( processor_state& state, op_params& pb );
	
	void microcode_RTS( processor_state& state, op_params& pb );
	
	void microcode_TRAPV( processor_state& state, op_params& pb );
	
	void microcode_RTR( processor_state& state, op_params& pb );
	
	void microcode_MOVEC( processor_state& state, op_params& pb );
	
	#pragma mark -
	#pragma mark Line 5
	
	void microcode_DBcc( processor_state& state, op_params& pb );
	
	void microcode_Scc( processor_state& state, op_params& pb );
	
	#pragma mark -
	#pragma mark Line 6
	
	void microcode_BRA( processor_state& state, op_params& pb );
	void microcode_BSR( processor_state& state, op_params& pb );
	void microcode_Bcc( processor_state& state, op_params& pb );
	
	#pragma mark -
	#pragma mark Line 8
	
	void microcode_OR( processor_state& state, op_params& pb );
	
	void microcode_SBCD( processor_state& state, op_params& pb );
	
	void microcode_DIVS( processor_state& state, op_params& pb );
	
	void microcode_DIVU( processor_state& state, op_params& pb );
	
	#pragma mark -
	#pragma mark Line 9
	
	void microcode_SUB( processor_state& state, op_params& pb );
	
	#pragma mark -
	#pragma mark Line B
	
	void microcode_CMP( processor_state& state, op_params& pb );
	
	void microcode_EOR( processor_state& state, op_params& pb );
	
	#pragma mark -
	#pragma mark Line C
	
	void microcode_AND( processor_state& state, op_params& pb );
	
	void microcode_EXG( processor_state& state, op_params& pb );
	
	void microcode_ABCD( processor_state& state, op_params& pb );
	
	void microcode_MULS( processor_state& state, op_params& pb );
	
	void microcode_MULU( processor_state& state, op_params& pb );
	
	#pragma mark -
	#pragma mark Line D
	
	void microcode_ADD( processor_state& state, op_params& pb );
	
	#pragma mark -
	#pragma mark Line E
	
	void microcode_ASR( processor_state& state, op_params& pb );
	void microcode_ASL( processor_state& state, op_params& pb );
	
	void microcode_LSR( processor_state& state, op_params& pb );
	void microcode_LSL( processor_state& state, op_params& pb );
	
	void microcode_ROXR( processor_state& state, op_params& pb );
	void microcode_ROXL( processor_state& state, op_params& pb );
	
	void microcode_ROR( processor_state& state, op_params& pb );
	void microcode_ROL( processor_state& state, op_params& pb );
	
}

#endif

