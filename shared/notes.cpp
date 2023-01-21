#include "notes.h"
#include "icons.h"

using namespace Tools;

extern Tools::Database *db;

FXDEFMAP( NotesWindow) NotesWindowMap[]={
FXMAPFUNC( SEL_COMMAND, NotesWindow::ID_head_title, NotesWindow::button_combo_changed),
FXMAPFUNC( SEL_COMMAND, NotesWindow::ID_lesen, NotesWindow::button_head_lesen),
FXMAPFUNC( SEL_COMMAND, NotesWindow::ID_neu, NotesWindow::button_head_neu),
FXMAPFUNC( SEL_COMMAND, NotesWindow::ID_p_del, NotesWindow::button_pos_del),
FXMAPFUNC( SEL_COMMAND, NotesWindow::ID_p_neu, NotesWindow::button_pos_neu),
FXMAPFUNC( SEL_COMMAND, NotesWindow::ID_del, NotesWindow::button_del),
FXMAPFUNC( SEL_COMMAND, NotesWindow::ID_ok, NotesWindow::button_ok),
FXMAPFUNC( SEL_COMMAND, NotesWindow::ID_cancel, NotesWindow::button_cancel),
};
FXIMPLEMENT( NotesWindow, FXDialogBox, NotesWindowMap, ARRAYNUMBER( NotesWindowMap ));

void NotesWindow::var_from_gui()
{
  notes_heads.title = head_title->getText().text();
  notes_heads.status = head_status->getText().text();
}

void NotesWindow::var_to_gui()
{
  head_title->setText( notes_heads.title.str().c_str());
  head_status->setText( notes_heads.status.str().c_str());
}

void NotesWindow::autoload_gui()
{
{
head_title->clearItems();
NOTES_HEADS b[100];
DBInLimit limit( 100 );
int res;

while( ( res = StdSqlSelect( *db, "select distinct(NOTES_HEADS.title) from NOTES_HEADS order by title", DBInArrayList() >> b, limit ) ) > 0 )
 {
   for( int i = 0; i < res; i++ )
      head_title->appendItem( b[i].title.str().c_str() );
  }
  db->rollback();
}
}

NotesWindow::NotesWindow( FXWindow *owner )
: FXDialogBox( owner, "Notitzen", DECOR_ALL )
{
group_1 = new FXGroupBox(this, "Notitzen warten", LAYOUT_FILL_X|LAYOUT_FILL_Y|FRAME_RIDGE);
hframe_2 = new FXHorizontalFrame(group_1, LAYOUT_FILL_X);
label_3 = new FXLabel(hframe_2, "Titel", 0);
head_title = new FXComboBox(hframe_2, notes_heads.title.get_size() + 1, 5, this, ID_head_title, FRAME_SUNKEN|FRAME_THICK|TEXTFIELD_LIMITED);
head_status = new FXComboBox(hframe_2, notes_heads.status.get_size() + 1, notes_heads.status.get_cases(), this, ID_head_status, FRAME_SUNKEN|FRAME_THICK|TEXTFIELD_LIMITED);
head_status->setEditable( false );
hframe_4 = new FXHorizontalFrame(hframe_2, LAYOUT_RIGHT|PACK_UNIFORM_WIDTH, 0, 0, 0, 0, 0, 0, 0, 0);
lesen = new FXButton(hframe_4, "Lesen", Icons->get_icon( "button_read"), this, ID_lesen);
neu = new FXButton(hframe_4, "Neu", Icons->get_icon( "filenew"), this, ID_neu);
group_5 = new FXGroupBox(group_1, "Positionen", LAYOUT_FILL_X|FRAME_RIDGE|LAYOUT_FILL_X|FRAME_RIDGE);
pos_parent = new FXHorizontalFrame(group_5, LAYOUT_FILL_X, 0, 0, 0, 0, 0, 0, 0, 0);
hframe_6 = new FXHorizontalFrame(group_5, LAYOUT_RIGHT|PACK_UNIFORM_WIDTH, 0, 0, 0, 0, 0, 0, 0, 0);
p_del = new FXButton(hframe_6, "Loeschen", Icons->get_icon( "edittrash"), this, ID_p_del);
p_neu = new FXButton(hframe_6, "Neu", Icons->get_icon( "filenew"), this, ID_p_neu);
hframe_7 = new FXHorizontalFrame(group_1, LAYOUT_FILL_X|LAYOUT_SIDE_BOTTOM);
del = new FXButton(hframe_7, "Loeschen", Icons->get_icon( "edittrash"), this, ID_del);
hframe_8 = new FXHorizontalFrame(hframe_7, LAYOUT_RIGHT|PACK_UNIFORM_WIDTH|PACK_UNIFORM_HEIGHT);
ok = new FXButton(hframe_8, "Ok", Icons->get_icon( "button_ok"), this, ID_ok);
cancel = new FXButton(hframe_8, "Abbrechen", Icons->get_icon( "button_cancel"), this, ID_cancel, BUTTON_DEFAULT|BUTTON_NORMAL);
on_load();

}
