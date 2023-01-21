#include "db_util.h"
#include "database.h"
#include "format.h"
#include <fx.h>

using namespace Tools;

extern Database *db;

void DBError( FXWindow *owner, const std::string &what )
{
  FXMessageBox::error( owner, MBOX_OK, "Datenbankfehler", 
		       format( "%s\n%s", what, db->get_error() ).c_str() );  
}
