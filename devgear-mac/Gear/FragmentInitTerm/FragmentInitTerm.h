/*	==================
 *	FragmentInitTerm.h
 *	==================
 */


#ifndef FRAGMENTINITTERM_H
#define FRAGMENTINITTERM_H

#include "FragmentInitTermProcs.h"
	
	typedef pascal OSErr (*FragmentInitializer)( const CFragInitBlock* initBlock );
	typedef pascal void  (*FragmentTerminator )();
	
	struct FragmentProcs
	{
		FragmentInitializer fInit;
		FragmentTerminator  fTerm;
	};
	typedef struct FragmentProcs FragmentProcs;
	
	static FragmentProcs gFragmentProcs[] = FRAGMENT_INIT_TERM_PROCS_ARRAY;
	
	enum { kFragmentProcsCount = sizeof gFragmentProcs / sizeof (FragmentProcs) };
	
	static int gInitCount = 0;
	
	static void Muxed_Terminate();
	
	static OSErr Muxed_Initialize( const CFragInitBlock* initBlock )
	{
		for ( gInitCount = 0;  gInitCount < kFragmentProcsCount;  ++gInitCount )
		{
			FragmentInitializer init = gFragmentProcs[ gInitCount ].fInit;
			
			OSErr err = init ? init( initBlock ) : noErr;
			
			if ( err != noErr )
			{
				Muxed_Terminate();
				
				return err;
			}
		}
		
		return noErr;
	}
	
	static void Muxed_Terminate()
	{
		while ( gInitCount > 0 )
		{
			FragmentTerminator term = gFragmentProcs[ --gInitCount ].fTerm;
			
			if ( term ) term();
		}
	}
	
	extern pascal OSErr InitializeFragment( const CFragInitBlock* initBlock );
	extern pascal void TerminateFragment();
	
#pragma export on
	
	pascal OSErr InitializeFragment( const CFragInitBlock* initBlock )
	{
		return Muxed_Initialize( initBlock );
	}
	
	pascal void TerminateFragment()
	{
		Muxed_Terminate();
	}
	
#pragma export reset

#endif

