/*
	vfork_context.cc
	----------------
	
	Joshua Juran
*/

#include "Genie/Process/vfork_context.hh"

// Debug
#include "debug/assert.hh"

// Relix
#include "relix/vfork.h"


namespace Genie
{
	
	vfork_context::~vfork_context()
	{
		delete its_pad;
	}
	
	recall::stack_frame_pointer vfork_context::get_vfork_frame_pointer() const
	{
		if ( its_pad != NULL )
		{
			return (recall::stack_frame_pointer) its_pad->fp;
		}
		
		return NULL;
	}
	
	void vfork_context::resume_vfork( int second_result )
	{
		ASSERT( its_pad != NULL );
		
		_vfork_pad pad = *its_pad;
		
		delete its_pad;
		
		its_pad = NULL;
		
		const _resume_handler_t resume = pad.resume_handler;
		
		ASSERT( resume != NULL );
		
		pad.pid = second_result;
		
		resume( &pad );
	}
	
	void vfork_context::set_resume_handler( const _vfork_pad* pad )
	{
		if ( its_pad == NULL )
		{
			its_pad = new _vfork_pad;
		}
		
		*its_pad = *pad;
	}
	
}

