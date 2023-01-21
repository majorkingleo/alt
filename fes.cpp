#include "fes.h"
#include "format.h"
#include "db_util.h"
#include <iostream>

using namespace Tools;

extern Database *db;

long FESWindow::button_ok( FXObject* obj, FXSelector sel, void* )
{
  var_from_gui();

  if( !fes.id() )
    {
      if( !StdSqlInsert( *db, fes ) )
	{
	  DBError( this );
	  db->rollback();
	} 
      else 
	{
	  db->commit();
	  feldid->appendItem( fes.feldid().c_str() );
	}
    } 
  else
    {
      if( !StdSqlUpdate( *db, fes, format( "where id = '%s'", fes.id() ) ) )
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

long FESWindow::button_cancel( FXObject* obj, FXSelector sel, void* ptr )
{
  return FXDialogBox::onCmdCancel( obj, sel, ptr );
}

long FESWindow::button_neu( FXObject* obj, FXSelector sel, void* )
{
  fes = FES();

  var_to_gui();

  return 1;
}

long FESWindow::button_combo_changed( FXObject* obj, FXSelector sel, void* ptr )
{
  return button_lesen( obj, sel, ptr );
}

long FESWindow::button_lesen( FXObject* obj, FXSelector sel, void* ptr )
{
  var_from_gui();

  FES f;

  if( !StdSqlSelect( *db, "select %FES from FES where FES.feldid = '" + fes.feldid() + "'",
		     DBInList<DBBindType>() >> f ) )
    {
      DBError( this, "Lesen fehlgeschlagen!" );
    }
  else
    {
      fes = f;
      
      var_to_gui();
    }

  db->rollback();

  return 1;
}

long FESWindow::button_del( FXObject* obj, FXSelector sel, void* ptr )
{
  if( fes.id() )
    {
      DBErg<DBRowList> erg = db->exec( format( "delete from FES where id = '%s'", fes.id() ) );

      if( !erg && !db->get_error().empty() )
	{
	  DBError( this, "Loeschen fehlgeschlagen" );
	  db->rollback();
	}
      else
	{
	  db->commit();
	  feldid->removeItem( feldid->getCurrentItem() );
	  description->setText( "" );
	}
    }

  return 1;
}

void FESWindow::on_load()
{
  autoload_gui();
}
