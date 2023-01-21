#ifndef SPREADSHEET_H
#define SPREADSHEET_H

#include "fx.h"
#include "dbi.h"

class Spreadsheet : public FXTable
{
  FXDECLARE( Spreadsheet );
 private:
  Tools::DBBindType *bt;

  Spreadsheet() {}
 public:
  Spreadsheet( Tools::DBBindType *bt, FXComposite *p, FXint nr, FXint nc, 
	       FXObject* tgt = NULL, 
	       FXSelector sel = 0,
	       FXuint opts = TABLE_COL_SIZABLE|TABLE_ROW_SIZABLE | LAYOUT_FILL_X | LAYOUT_FILL_Y );
  
};

#endif
