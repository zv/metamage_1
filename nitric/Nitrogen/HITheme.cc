// Nitrogen/HITheme.cc
// -------------------
//
// Maintained by Marshall Clow

// Part of the Nitrogen project.
//
// Written 2004-2006 by Marshall Clow and Joshua Juran.
//
// This code was written entirely by the above contributors, who place it
// in the public domain.


#ifdef __MACH__

#include "Nitrogen/HITheme.hh"


namespace Nitrogen {
	
	/*
	HIThemeErrorsRegistrationDependency::HIThemeErrorsRegistrationDependency()
	{
		// does nothing, but guarantees construction of theRegistration
	}
	*/
	
	
	static void RegisterHIThemeErrors();
	
	
	class HIThemeErrorsRegistration
	{
		public:
			HIThemeErrorsRegistration()  { RegisterHIThemeErrors(); }
	};
	
	static HIThemeErrorsRegistration theRegistration;
	
	
	void RegisterHIThemeErrors () {
	//	Apple hasn't documented any yet!
		}
	
	}

#endif

