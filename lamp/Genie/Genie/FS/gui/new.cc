/*
	gui/new.cc
	----------
*/

#include "Genie/FS/gui/new.hh"

// Genie
#include "Genie/FS/gui/new/bitmap.hh"
#include "Genie/FS/gui/new/button.hh"
#include "Genie/FS/gui/new/caption.hh"
#include "Genie/FS/gui/new/console.hh"
#include "Genie/FS/gui/new/defaultkeys.hh"
#include "Genie/FS/gui/new/focuser.hh"
#include "Genie/FS/gui/new/frame.hh"
#include "Genie/FS/gui/new/gworld.hh"
#include "Genie/FS/gui/new/icon.hh"
#include "Genie/FS/gui/new/list.hh"
#include "Genie/FS/gui/new/port.hh"
#include "Genie/FS/gui/new/scroller.hh"
#include "Genie/FS/gui/new/scrollframe.hh"
#include "Genie/FS/gui/new/stack.hh"
#include "Genie/FS/gui/new/textedit.hh"


namespace Genie
{
	
	const FSTree_Premapped::Mapping new_Mappings[] =
	{
		{ "bitmap",       &New_FSTree_new_bitmap              },
		{ "button",       &New_FSTree_new_button              },
		{ "caption",      &New_FSTree_new_caption             },
		{ "console",      &New_FSTree_new_console             },
		{ "defaultkeys",  &New_defaultkeys                    },
		{ "focuser",      &New_focuser                        },
		{ "frame",        &New_FSTree_new_frame               },
		{ "gworld",       &New_FSTree_new_gworld              },
		{ "icon",         &New_FSTree_new_icon                },
		{ "list",         &New_FSTree_new_list                },
		{ "scroller",     &New_FSTree_new_scroller            },
		{ "scrollframe",  &New_FSTree_new_scrollframe         },
		{ "stack",        &New_stack                          },
		{ "textedit",     &New_FSTree_new_textedit            },
		{ "port",         &Basic_Factory< FSTree_new_port   > },
		
		{ NULL, NULL }
	};
	
}

