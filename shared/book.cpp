#include "book.h"
#include "icons.h"

using namespace Tools;

extern Tools::Database *db;

FXDEFMAP( BOOKWindow) BOOKWindowMap[]={
FXMAPFUNC( SEL_COMMAND, BOOKWindow::ID_b_lesen, BOOKWindow::button_lesen),
FXMAPFUNC( SEL_COMMAND, BOOKWindow::ID_b_neu, BOOKWindow::button_neu),
FXMAPFUNC( SEL_COMMAND, BOOKWindow::ID_f_ref, BOOKWindow::onBarcode),
FXMAPFUNC( SEL_COMMAND, BOOKWindow::ID_del, BOOKWindow::button_del),
FXMAPFUNC( SEL_COMMAND, BOOKWindow::ID_ok, BOOKWindow::button_ok),
FXMAPFUNC( SEL_COMMAND, BOOKWindow::ID_cancel, BOOKWindow::button_cancel),
};
FXIMPLEMENT( BOOKWindow, FXDialogBox, BOOKWindowMap, ARRAYNUMBER( BOOKWindowMap ));

void BOOKWindow::var_from_gui()
{
  book.id = f_id->getText().text();
  book.type = f_type->getText().text();
  book.title = f_title->getText().text();
  book.author = f_author->getText().text();
  book.description = f_description->getText().text();
  book.isbn = f_isbn->getText().text();
  book.identifier = f_ref->getText().text();
  book.location = f_location->getText().text();
  book.owner = f_owner->getText().text();
  book.comment = f_comment->getText().text();
}

void BOOKWindow::var_to_gui()
{
  f_id->setText( book.id.str().c_str());
  f_type->setText( book.type.str().c_str());
  f_title->setText( book.title.str().c_str());
  f_author->setText( book.author.str().c_str());
  f_description->setText( book.description.str().c_str());
  f_isbn->setText( book.isbn.str().c_str());
  f_ref->setText( book.identifier.str().c_str());
  f_location->setText( book.location.str().c_str());
  f_owner->setText( book.owner.str().c_str());
  f_comment->setText( book.comment.str().c_str());
}

void BOOKWindow::autoload_gui()
{
{
f_id->clearItems();
BOOKS b[100];
DBInLimit limit( 100 );
int res;

while( ( res = StdSqlSelect( *db, "select distinct(BOOKS.id) from BOOKS order by id", DBInArrayList() >> b, limit ) ) > 0 )
 {
   for( int i = 0; i < res; i++ )
      f_id->appendItem( b[i].id.str().c_str() );
  }
  db->rollback();
}
{
f_author->clearItems();
BOOKS b[100];
DBInLimit limit( 100 );
int res;

while( ( res = StdSqlSelect( *db, "select distinct(BOOKS.author) from BOOKS order by author", DBInArrayList() >> b, limit ) ) > 0 )
 {
   for( int i = 0; i < res; i++ )
      f_author->appendItem( b[i].author.str().c_str() );
  }
  db->rollback();
}
{
f_location->clearItems();
FES b[100];
DBInLimit limit( 100 );
int res;

while( ( res = StdSqlSelect( *db, "select distinct(FES.feldid) from FES order by feldid", DBInArrayList() >> b, limit ) ) > 0 )
 {
   for( int i = 0; i < res; i++ )
      f_location->appendItem( b[i].feldid.str().c_str() );
  }
  db->rollback();
}
{
f_owner->clearItems();
BOOKS b[100];
DBInLimit limit( 100 );
int res;

while( ( res = StdSqlSelect( *db, "select distinct(BOOKS.owner) from BOOKS order by owner", DBInArrayList() >> b, limit ) ) > 0 )
 {
   for( int i = 0; i < res; i++ )
      f_owner->appendItem( b[i].owner.str().c_str() );
  }
  db->rollback();
}
}

BOOKWindow::BOOKWindow( FXWindow *owner )
: FXDialogBox( owner, "Buecher", DECOR_ALL )
{
foo = new FXGroupBox(this, "Buecher warten", LAYOUT_FILL_X|LAYOUT_FILL_Y|FRAME_RIDGE);
hframe_1 = new FXHorizontalFrame(foo, LAYOUT_FILL_X);
label_2 = new FXLabel(hframe_1, "Id:", 0, LAYOUT_CENTER_Y);
f_id = new FXComboBox(hframe_1, book.id.get_size() + 1, 5, this, ID_f_id, FRAME_SUNKEN|FRAME_THICK|TEXTFIELD_LIMITED);
f_type = new FXComboBox(hframe_1, book.type.get_size() + 1, book.type.get_cases(), this, ID_f_type, FRAME_SUNKEN|FRAME_THICK|TEXTFIELD_LIMITED);
f_type->setEditable( false );
hframe_3 = new FXHorizontalFrame(hframe_1, LAYOUT_RIGHT|PACK_UNIFORM_WIDTH, 0, 0, 0, 0, 0, 0, 0, 0);
b_lesen = new FXButton(hframe_3, "Lesen", Icons->get_icon( "button_read"), this, ID_b_lesen);
b_neu = new FXButton(hframe_3, "Neu", Icons->get_icon( "filenew"), this, ID_b_neu);
group_4 = new FXGroupBox(foo, "Titel", LAYOUT_FILL_X|FRAME_RIDGE|LAYOUT_FILL_X|FRAME_RIDGE);
hframe_5 = new FXHorizontalFrame(group_4, LAYOUT_FILL_X|FRAME_SUNKEN|FRAME_THICK, 0, 0, 0, 0, 0, 0, 0, 0);
f_title = new FXText(hframe_5, this, ID_f_title, LAYOUT_FILL_X);
hframe_6 = new FXHorizontalFrame(foo);
label_7 = new FXLabel(hframe_6, "Author:", 0, LAYOUT_CENTER_Y);
f_author = new FXComboBox(hframe_6, book.author.get_size() + 1, 5, this, ID_f_author, FRAME_SUNKEN|FRAME_THICK|TEXTFIELD_LIMITED);
group_8 = new FXGroupBox(foo, "Beschreibung", LAYOUT_FILL_X|FRAME_RIDGE|LAYOUT_FILL_X|FRAME_RIDGE);
hframe_9 = new FXHorizontalFrame(group_8, LAYOUT_FILL_X|FRAME_SUNKEN|FRAME_THICK, 0, 0, 0, 0, 0, 0, 0, 0);
f_description = new FXText(hframe_9, this, ID_f_description, LAYOUT_FILL_X);
matrix_10 = new FXMatrix(foo, 5);
label_11 = new FXLabel(matrix_10, "ISBN:", 0);
label_12 = new FXLabel(matrix_10, "Ref:", 0);
label_13 = new FXLabel(matrix_10, "Ort:", 0);
label_14 = new FXLabel(matrix_10, "Besitzer:", 0);
label_15 = new FXLabel(matrix_10, "Kommentar:", 0);
f_isbn = new FXTextField(matrix_10, book.isbn.get_size(), this, ID_f_isbn, TEXTFIELD_LIMITED|FRAME_SUNKEN|FRAME_THICK);
f_ref = new FXTextField(matrix_10, book.identifier.get_size(), this, ID_f_ref, TEXTFIELD_LIMITED|FRAME_SUNKEN|FRAME_THICK);
f_location = new FXComboBox(matrix_10, book.location.get_size() + 1, 5, this, ID_f_location, FRAME_SUNKEN|FRAME_THICK|TEXTFIELD_LIMITED);
f_location->setEditable( false );
f_owner = new FXComboBox(matrix_10, book.owner.get_size() + 1, 5, this, ID_f_owner, FRAME_SUNKEN|FRAME_THICK|TEXTFIELD_LIMITED);
f_comment = new FXTextField(matrix_10, book.comment.get_size(), this, ID_f_comment, TEXTFIELD_LIMITED|FRAME_SUNKEN|FRAME_THICK);
hseperator_16 = new FXHorizontalSeparator(foo);
hframe_17 = new FXHorizontalFrame(foo, LAYOUT_FILL_X|LAYOUT_SIDE_BOTTOM);
del = new FXButton(hframe_17, "Loeschen", Icons->get_icon( "edittrash"), this, ID_del);
hframe_18 = new FXHorizontalFrame(hframe_17, LAYOUT_RIGHT|PACK_UNIFORM_WIDTH|PACK_UNIFORM_HEIGHT);
ok = new FXButton(hframe_18, "Ok", Icons->get_icon( "button_ok"), this, ID_ok);
cancel = new FXButton(hframe_18, "Abbrechen", Icons->get_icon( "button_cancel"), this, ID_cancel, BUTTON_DEFAULT|BUTTON_NORMAL);
on_load();

}
