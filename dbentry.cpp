#include "dbentry.h"
#include "fxkeys.h"
#include <cctype>
#include <iostream>

using namespace Tools;

FXDEFMAP( DBEntry ) DBEntryMap[] = {
  FXMAPFUNC( SEL_VERIFY, 0, DBEntry::onVerify )
};

FXIMPLEMENT( DBEntry, FXTextField, DBEntryMap, ARRAYNUMBER( DBEntryMap ) );

DBEntry::DBEntry( Tools::DBType *dbtype, FXComposite* p,FXint ncols,FXObject* tgt,FXSelector sel,FXuint opts )
  : FXTextField( p, ncols, tgt, sel, opts ),
    dbtype( dbtype )
{
  len = dbtype->get_size();
  type = dbtype->type;

  switch( type )
    {
    case DBType::TYPE::INT:
      is_signed = false;
      break;    
    }  
}


long DBEntry::onVerify(FXObject* o,FXSelector sel,void* p)
{
  std::string s( (char*)p );
  
  switch( type )
    {
    case DBType::TYPE::INT:
      for( unsigned i = 0; i < s.size(); i++ )
	{
	  if( !std::isdigit( s[i] ) )
	    {
	      if( !( is_signed && ( s[i] == '-' || s[i] == '+' ) ) )
		return 1;
	    }
	}
    }

  return FXTextField::onVerify( o, sel, p );
}
