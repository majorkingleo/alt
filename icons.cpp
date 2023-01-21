#include "icons.h"
#include "FXPNGIcon.h"

#include "i_button_ok.h"
#include "i_button_cancel.h"
#include "i_edittrash.h"
#include "i_filenew.h"
#include "i_button_read.h"
#include "i_wizard.h"
#include "i_fileexport.h"
#include "i_fileclose.h"
#include "i_exit.h"

using namespace Tools;

FXIcon * IconManager::get_icon( const std::string &name )
{
  for( unsigned i = 0; i < icons.size(); i++ )
    {
      if( icase_cmp( icons[i].name, name ) )
	return new FXPNGIcon( app, icons[i].data );
    }
  return 0;
}

void IconManager::create()
{
  for( unsigned i = 0; i < icons.size(); i++ )
    if( icons[i].icon )
      return icons[i].icon->create();
}


void IconManager::add( const std::string &name, const unsigned char *icon )
{
  //  icons.push_back( Icon( name, new FXGIFIcon( app, icon ), const_cast<unsigned char*>(icon) ) );
  icons.push_back( Icon( name, 0, const_cast<unsigned char*>(icon) ) );
}

void IconManager::implement()
{
  Icons->add( "button_ok", button_ok );
  Icons->add( "button_cancel", button_cancel );
  Icons->add( "edittrash", edittrash );
  Icons->add( "filenew", filenew );
  Icons->add( "button_read", button_read );
  Icons->add( "wizard", wizard );
  Icons->add( "save", fileexport );
  Icons->add( "close", fileclose );
  Icons->add( "exit", bexit );
}
