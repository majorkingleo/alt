#include "spreadsheet.h"

using namespace Tools;

FXDEFMAP( Spreadsheet ) SpreadsheetMap[] = {

};

FXIMPLEMENT( Spreadsheet, FXTable, SpreadsheetMap, ARRAYNUMBER( SpreadsheetMap ) );

Spreadsheet::Spreadsheet( Tools::DBBindType *bt, FXComposite *p, FXint nr, FXint nc, 
			 FXObject* tgt, 
			 FXSelector sel,
			 FXuint opts)
  : FXTable( p, nr, nc, tgt, sel, opts ),
    bt( bt )
{
  setTableSize( 1 + bt->get_names().size(), 1 );

  setLeadingRows( 1 );
  setLeadingCols( 1 );
  setItemText( 0, 0, FXString::null );

  getItem( 0, 0 )->setButton( true );

  for( unsigned i = 1; i < bt->get_names().size() + 1; i++ )
    {
      setItemText( 0, i, bt->get_names()[i-1].c_str() );
      getItem( 0, i )->setButton( true );
    }
}
