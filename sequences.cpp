#include "sequences.h"
#include "spreadsheet.h"

using namespace Tools;

void SequencesWindow::on_load()
{
  new Spreadsheet( &sequence, spread_parent, 1, 1 );
}

long SequencesWindow::button_ok( FXObject* obj, FXSelector sel, void* ptr )
{
  return 1;
}

long SequencesWindow::button_del( FXObject *obj, FXSelector sel, void *ptr)
{
  return 1;
}

long SequencesWindow::button_cancel( FXObject *obj, FXSelector sel, void *ptr)
{
  return 1;
}
