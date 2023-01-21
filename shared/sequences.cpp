#include "sequences.h"
#include "icons.h"

using namespace Tools;

extern Tools::Database *db;

FXDEFMAP( SequencesWindow) SequencesWindowMap[]={
FXMAPFUNC( SEL_COMMAND, SequencesWindow::ID_del, SequencesWindow::button_del),
FXMAPFUNC( SEL_COMMAND, SequencesWindow::ID_ok, SequencesWindow::button_ok),
FXMAPFUNC( SEL_COMMAND, SequencesWindow::ID_cancel, SequencesWindow::button_cancel),
};
FXIMPLEMENT( SequencesWindow, FXDialogBox, SequencesWindowMap, ARRAYNUMBER( SequencesWindowMap ));

void SequencesWindow::var_from_gui()
{
}

void SequencesWindow::var_to_gui()
{
}

void SequencesWindow::autoload_gui()
{
}

SequencesWindow::SequencesWindow( FXWindow *owner )
: FXDialogBox( owner, "Sequencen warten", DECOR_ALL )
{
group_1 = new FXGroupBox(this, "Sequencen warten", LAYOUT_FILL_X|LAYOUT_FILL_Y|FRAME_RIDGE);
spread_parent = new FXHorizontalFrame(group_1, LAYOUT_FILL_X, 0, 0, 0, 0, 0, 0, 0, 0);
hframe_2 = new FXHorizontalFrame(group_1, LAYOUT_FILL_X|LAYOUT_SIDE_BOTTOM);
del = new FXButton(hframe_2, "Loeschen", Icons->get_icon( "edittrash"), this, ID_del);
hframe_3 = new FXHorizontalFrame(hframe_2, LAYOUT_RIGHT|PACK_UNIFORM_WIDTH|PACK_UNIFORM_HEIGHT);
ok = new FXButton(hframe_3, "Ok", Icons->get_icon( "button_ok"), this, ID_ok);
cancel = new FXButton(hframe_3, "Abbrechen", Icons->get_icon( "button_cancel"), this, ID_cancel, BUTTON_DEFAULT|BUTTON_NORMAL);
on_load();

}
