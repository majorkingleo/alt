#include "liste.h"
#include "icons.h"
#include <iostream>

using namespace Tools;

extern Database *db;

FXDEFMAP( Liste ) ListeMap[] = {
  FXMAPFUNC( SEL_COMMAND, Liste::ID_SAVE_LAYOUT, Liste::onSaveLayout ),
  FXMAPFUNC( SEL_COMMAND, Liste::ID_OK, Liste::onOk ),
};

FXIMPLEMENT( Liste, FXDialogBox, ListeMap, ARRAYNUMBER( ListeMap ) );

Liste::Liste( FXWindow *owner, const std::string &title )
  : FXDialogBox( owner, title.c_str(), DECOR_ALL )
{
  FXVerticalFrame *f = new FXVerticalFrame( this, LAYOUT_FILL_X | LAYOUT_FILL_Y );

  table = new FXTable( f, 5, 5, 0, 0, TABLE_COL_SIZABLE|TABLE_ROW_SIZABLE | LAYOUT_FILL_X | LAYOUT_FILL_Y );  
  bt = 0;

  new FXHorizontalSeparator( f );

  FXHorizontalFrame *bf = new FXHorizontalFrame( f, LAYOUT_FILL_X|LAYOUT_SIDE_BOTTOM );
  new FXButton( bf, "Layout Speichern", Icons->get_icon( "save" ), this, ID_SAVE_LAYOUT );
  FXHorizontalFrame *brf = new FXHorizontalFrame( bf, LAYOUT_RIGHT|PACK_UNIFORM_WIDTH,0,0,0,0,0,0,0,0 );
  new FXButton( brf, "Schlieszen", Icons->get_icon( "close" ), this, ID_OK );
}

Liste::~Liste()
{
  delete bt;
}

unsigned Liste::count_erg( const std::string &table_name )
{
  DBTypeInt count;
  
  StdSqlSelect( *db, "select count(*) from " + table_name, DBInList<DBType>() >> count );
  
  return count();
}

bool Liste::restore_layout()
{
  if( !bt )
    return false;

  std::string name = std::string("Liste::" + bt->get_table_name());

  if( !getApp()->reg().readIntEntry( name.c_str(), "have_layout", false ) )
    return false;

  for( unsigned i = 0; i < bt->get_names().size() + 1; i++ )
    {
      table->setColumnWidth( i, 
			     getApp()->reg().readIntEntry( name.c_str(), 
							   std::string( "w" + x2s( i ) ).c_str(), 
							   table->getDefColumnWidth() ) );
    }
  
  int height = getApp()->reg().readIntEntry( name.c_str(), "height" , 
					     table->getDefRowHeight() );

  for( int i = 1; i < table->getNumRows(); i++ )
    table->setRowHeight( i, height );

  table->setDefRowHeight( height );

  setWidth( getApp()->reg().readIntEntry( name.c_str(), "window_width", 500 ) );
  setHeight( getApp()->reg().readIntEntry( name.c_str(), "window_height", 500 ) );				

  return true;
}

long Liste::onSaveLayout( FXObject *obj, FXSelector sel, void *ptr )
{
  if( !bt )
    return 0;

  std::string name = std::string("Liste::" + bt->get_table_name());

  for( unsigned i = 0; i < bt->get_names().size() + 1; i++ )
    {
      getApp()->reg().writeIntEntry( name.c_str(), 
				     std::string( "w" + x2s( i )).c_str(), 
				     table->getColumnWidth( i ) );
    }

  getApp()->reg().writeIntEntry( name.c_str(), "height" , table->getRowHeight( 1 ) );				 
  getApp()->reg().writeIntEntry( name.c_str(), "have_layout", true );
  getApp()->reg().writeIntEntry( name.c_str(), "window_width", getWidth() );
  getApp()->reg().writeIntEntry( name.c_str(), "window_height", getHeight() );

  return 1;
}

long Liste::onOk( FXObject *obj, FXSelector sel, void *ptr )
{
  return FXDialogBox::onCmdCancel( obj, sel, ptr );
}
