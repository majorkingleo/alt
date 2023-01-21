#include "book.h"
#include "format.h"
#include "db_util.h"

using namespace Tools;

extern Database *db;

long BOOKWindow::button_cancel( FXObject* obj, FXSelector sel, void* ptr )
{
  return FXDialogBox::onCmdCancel( obj, sel, ptr );
}

long BOOKWindow::button_ok( FXObject* obj, FXSelector sel, void* ptr )
{
  var_from_gui();

  if( !book.id() )
    {
      if( !StdSqlInsert( *db, book ) )
	{
	  DBError( this );
	  db->rollback();
	}
      else
	{
	  int id = db->get_insert_id();
	  db->commit();
	  autoload_gui();
 	  book.id = id;
	  var_to_gui();
	  button_lesen( obj, sel, ptr );
	}
    }
  else
    {
      if( !StdSqlUpdate( *db, book, format( "where id = '%s'", book.id() ) ) )
	{
	  DBError( this );
	  db->rollback();
	}
      else
	{
	  db->commit();
	}
    }

  return 1;
}

long BOOKWindow::button_del( FXObject* obj, FXSelector sel, void* ptr )
{
  if( book.id() )
    {
      DBErg<DBRowList> erg = db->exec( format( "delete from BOOKS where id = '%s'", book.id() ) );

      if( !erg && !db->get_error().empty() )
	{
	  DBError( this, "Loeschen fehlgeschlagen" );
	  db->rollback();
	}
      else
	{
	  button_neu( obj, sel, ptr );
	  db->commit();
	  autoload_gui();	 
	}
    }  

  return 1;
}

long BOOKWindow::button_neu( FXObject* obj, FXSelector sel, void* ptr )
{
  book = BOOKS();

  f_ref->setFocus();

  var_to_gui();

  return 1;
}

long BOOKWindow::onBarcode( FXObject* obj, FXSelector sel, void* ptr )
{
  var_from_gui();

  if( book.identifier.str().empty() )
    return 1;

  BOOKS b;

  if( !StdSqlSelect( *db, "select %BOOKS from BOOKS where identifier = '" + book.identifier.str() + "'",
		     DBInList<DBBindType>() >> b ) )
    {
      DBError( this, "Lesen fehlgeschlagen!" );
    }
  else
    {
      book = b;

      var_to_gui();
    }

  db->rollback();

  return 1;
}

long BOOKWindow::button_lesen( FXObject* obj, FXSelector sel, void* ptr )
{
  var_from_gui();

  BOOKS b;

  if( !StdSqlSelect( *db, "select %BOOKS from BOOKS where id = '" + book.id.str() + "'",
		     DBInList<DBBindType>() >> b ) )
    {
      DBError( this, "Lesen fehlgeschlagen!" );
    }
  else
    {
      book = b;

      var_to_gui();
    }

  db->rollback();

  return 1;
}

void BOOKWindow::on_load()
{
  for( int i = 0; i < book.type.data.LAST__; i++ )
    {
      f_type->appendItem( book.type.data.STYPES[i] );
    }

  f_ref->setFocus();

  autoload_gui();
}

