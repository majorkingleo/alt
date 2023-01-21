#include "notes.h"
#include "format.h"
#include "db_util.h"
#include "mynotes.h"

using namespace Tools;

extern Database *db;

void NotesWindow::on_load()
{ 
  positionen = new FXTable( pos_parent, 20, 5, 0, 0, 
			    TABLE_COL_SIZABLE|TABLE_ROW_SIZABLE|LAYOUT_FILL_X|LAYOUT_FILL_Y); 

  positionen->setTableSize( 2,6 );

  positionen->setLeadingRows(1);
  // positionen->setLeadingCols(1);
  positionen->setItemText(0,0, "Id" );

  positionen->setItemText( 0, 1, "Status" );
  positionen->setItemText( 0, 2, "Type" );
  positionen->setItemText( 0, 3, "Note" );
  positionen->setItemText( 0, 4, "Anzeit" );
  positionen->setItemText( 0, 5, "AeZeit" );
  

  positionen->getItem(0,0)->setButton(true);
  positionen->getItem(0,1)->setButton(true);
  positionen->getItem(0,2)->setButton(true);
  positionen->getItem(0,3)->setButton(true);
  positionen->getItem(0,4)->setButton(true);
  positionen->getItem(0,5)->setButton(true);
  
  for( int i = 0; i < notes_heads.status.data.LAST__; i++ )
    {
      head_status->appendItem( notes_heads.status.data.STYPES[i] );
    }

  autoload_gui();
}

long NotesWindow::button_del( FXObject *obj, FXSelector sel, void *ptr)
{
  return 1;
}

long NotesWindow::button_ok( FXObject *obj, FXSelector sel, void *ptr)
{
  var_from_gui();

  if( !notes_heads.id() )
    {
      if( !StdSqlInsert( *db, notes_heads ) )
	{
	  DBError( this );
	  db->rollback();
	}
      else
	{
	  db->commit();	 
	  head_title->appendItem( notes_heads.title().c_str() );
	}
    }
  else
    {
      if( !StdSqlUpdate( *db, notes_heads, format( "where id = '%s'", notes_heads.id() ) ) )
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

long NotesWindow::button_head_neu( FXObject *obj, FXSelector sel, void *ptr)
{
  notes_heads = NOTES_HEADS();

  var_to_gui();
  
  return 1;
}

long NotesWindow::button_combo_changed( FXObject* obj, FXSelector sel, void* ptr )
{
  return 1;
}

long MyNotes::button_combo_changed( FXObject* obj, FXSelector sel, void* ptr )
{
  return head_lesen( true );
}

long MyNotes::head_lesen( bool from_combo )
{
  var_from_gui();

  NOTES_HEADS f;

  if( !StdSqlSelect( *db, "select %NOTES_HEADS from NOTES_HEADS where NOTES_HEADS.title = '" + 
		     notes_heads.title() + "'",
		     DBInList<DBBindType>() >> f ) )
    {
      if( !from_combo )
	DBError( this, "Lesen fehlgeschlagen!" );      
    }
  else
    {
      notes_heads = f;    
      var_to_gui();
    }

  db->rollback();
}

long NotesWindow::button_head_lesen( FXObject *obj, FXSelector sel, void *ptr)
{
  return 1;
}

long MyNotes::button_head_lesen( FXObject *obj, FXSelector sel, void *ptr)
{
  return head_lesen( false );
}

long NotesWindow::button_pos_del( FXObject *obj, FXSelector sel, void *ptr)
{
  return 1;
}

long NotesWindow::button_pos_neu( FXObject *obj, FXSelector sel, void *ptr)
{
  return 1;
}


long NotesWindow::button_cancel( FXObject *obj, FXSelector sel, void *ptr)
{
  return FXDialogBox::onCmdCancel( obj, sel, ptr );
}
