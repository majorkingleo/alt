#include "fes.h"
#include "icons.h"

using namespace Tools;

extern Tools::Database *db;

FXDEFMAP( FESWindow) FESWindowMap[]={
FXMAPFUNC( SEL_COMMAND, FESWindow::ID_feldid, FESWindow::button_combo_changed),
FXMAPFUNC( SEL_COMMAND, FESWindow::ID_lesen, FESWindow::button_lesen),
FXMAPFUNC( SEL_COMMAND, FESWindow::ID_neu, FESWindow::button_neu),
FXMAPFUNC( SEL_COMMAND, FESWindow::ID_del, FESWindow::button_del),
FXMAPFUNC( SEL_COMMAND, FESWindow::ID_ok, FESWindow::button_ok),
FXMAPFUNC( SEL_COMMAND, FESWindow::ID_cancel, FESWindow::button_cancel),
};
FXIMPLEMENT( FESWindow, FXDialogBox, FESWindowMap, ARRAYNUMBER( FESWindowMap ));

void FESWindow::var_from_gui()
{
  fes.feldid = feldid->getText().text();
  fes.description = description->getText().text();
}

void FESWindow::var_to_gui()
{
  feldid->setText( fes.feldid.str().c_str());
  description->setText( fes.description.str().c_str());
}

void FESWindow::autoload_gui()
{
{
feldid->clearItems();
FES b[100];
DBInLimit limit( 100 );
int res;

while( ( res = StdSqlSelect( *db, "select distinct(FES.feldid) from FES order by feldid", DBInArrayList() >> b, limit ) ) > 0 )
 {
   for( int i = 0; i < res; i++ )
      feldid->appendItem( b[i].feldid.str().c_str() );
  }
  db->rollback();
}
}

FESWindow::FESWindow( FXWindow *owner )
: FXDialogBox( owner, "Feldstamm", DECOR_ALL )
{
group_1 = new FXGroupBox(this, "Feldstamm warten", LAYOUT_FILL_X|LAYOUT_FILL_Y|FRAME_RIDGE);
hframe_2 = new FXHorizontalFrame(group_1);
label_3 = new FXLabel(hframe_2, "Feldid:", 0, LAYOUT_CENTER_Y);
feldid = new FXComboBox(hframe_2, fes.feldid.get_size() + 1, 5, this, ID_feldid, FRAME_SUNKEN|FRAME_THICK|TEXTFIELD_LIMITED);
lesen = new FXButton(hframe_2, "Lesen", Icons->get_icon( "button_read"), this, ID_lesen);
neu = new FXButton(hframe_2, "Neu", Icons->get_icon( "filenew"), this, ID_neu);
group_4 = new FXGroupBox(group_1, "Description", LAYOUT_FILL_X|FRAME_RIDGE|LAYOUT_FILL_X|FRAME_RIDGE);
hframe_5 = new FXHorizontalFrame(group_4, LAYOUT_FILL_X|FRAME_SUNKEN|FRAME_THICK, 0, 0, 0, 0, 0, 0, 0, 0);
description = new FXText(hframe_5, this, ID_description, LAYOUT_FILL_X);
hframe_6 = new FXHorizontalFrame(group_1, LAYOUT_FILL_X|LAYOUT_SIDE_BOTTOM);
del = new FXButton(hframe_6, "Loeschen", Icons->get_icon( "edittrash"), this, ID_del);
hframe_7 = new FXHorizontalFrame(hframe_6, LAYOUT_RIGHT|PACK_UNIFORM_WIDTH|PACK_UNIFORM_HEIGHT);
ok = new FXButton(hframe_7, "Ok", Icons->get_icon( "button_ok"), this, ID_ok);
cancel = new FXButton(hframe_7, "Abbrechen", Icons->get_icon( "button_cancel"), this, ID_cancel, BUTTON_DEFAULT|BUTTON_NORMAL);
on_load();

}
