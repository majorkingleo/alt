#include "sequence.h"
#include "icons.h"

using namespace Tools;

extern Tools::Database *db;

FXDEFMAP( SequenceWindow) SequenceWindowMap[]={
FXMAPFUNC( SEL_COMMAND, SequenceWindow::ID_del, SequenceWindow::button_del),
FXMAPFUNC( SEL_COMMAND, SequenceWindow::ID_ok, SequenceWindow::button_ok),
FXMAPFUNC( SEL_COMMAND, SequenceWindow::ID_cancel, SequenceWindow::button_cancel),
};
FXIMPLEMENT( SequenceWindow, FXDialogBox, SequenceWindowMap, ARRAYNUMBER( SequenceWindowMap ));

void SequenceWindow::var_from_gui()
{
  sequence.name = name->getText().text();
  sequence.from = from->getText().text();
  sequence.to = to->getText().text();
  sequence.nextval = nextval->getText().text();
  sequence.format = format->getText().text();
}

void SequenceWindow::var_to_gui()
{
  name->setText( sequence.name.str().c_str());
  from->setText( sequence.from.str().c_str());
  to->setText( sequence.to.str().c_str());
  nextval->setText( sequence.nextval.str().c_str());
  format->setText( sequence.format.str().c_str());
}

void SequenceWindow::autoload_gui()
{
}

SequenceWindow::SequenceWindow( FXWindow *owner )
: FXDialogBox( owner, "Sequence warten", DECOR_ALL )
{
group_1 = new FXGroupBox(this, "Sequence warten", LAYOUT_FILL_X|LAYOUT_FILL_Y|FRAME_RIDGE);
matrix_2 = new FXMatrix(group_1, 5);
label_3 = new FXLabel(matrix_2, "Name:", 0);
label_4 = new FXLabel(matrix_2, "Von:", 0);
label_5 = new FXLabel(matrix_2, "Bis:", 0);
label_6 = new FXLabel(matrix_2, "Naechster Wert:", 0);
label_7 = new FXLabel(matrix_2, "Format:", 0);
name = new DBEntry(&sequence.name, matrix_2, sequence.name.get_size(), this, ID_name, TEXTFIELD_LIMITED|FRAME_SUNKEN|FRAME_THICK);
from = new DBEntry(&sequence.from, matrix_2, sequence.from.get_size(), this, ID_from, TEXTFIELD_LIMITED|FRAME_SUNKEN|FRAME_THICK);
to = new DBEntry(&sequence.to, matrix_2, sequence.to.get_size(), this, ID_to, TEXTFIELD_LIMITED|FRAME_SUNKEN|FRAME_THICK);
nextval = new DBEntry(&sequence.nextval, matrix_2, sequence.nextval.get_size(), this, ID_nextval, TEXTFIELD_LIMITED|FRAME_SUNKEN|FRAME_THICK);
format = new DBEntry(&sequence.format, matrix_2, sequence.format.get_size(), this, ID_format, TEXTFIELD_LIMITED|FRAME_SUNKEN|FRAME_THICK);
format->setTipText( "Verwenden %d fuer die Zahl" );
hframe_8 = new FXHorizontalFrame(group_1, LAYOUT_FILL_X|LAYOUT_SIDE_BOTTOM);
del = new FXButton(hframe_8, "Loeschen", Icons->get_icon( "edittrash"), this, ID_del);
hframe_9 = new FXHorizontalFrame(hframe_8, LAYOUT_RIGHT|PACK_UNIFORM_WIDTH|PACK_UNIFORM_HEIGHT);
ok = new FXButton(hframe_9, "Ok", Icons->get_icon( "button_ok"), this, ID_ok);
cancel = new FXButton(hframe_9, "Abbrechen", Icons->get_icon( "button_cancel"), this, ID_cancel, BUTTON_DEFAULT|BUTTON_NORMAL);

}
