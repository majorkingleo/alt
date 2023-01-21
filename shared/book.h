#ifndef BOOK_HWINDOW_H
#define BOOK_HWINDOW_H

#include "fx.h"
#include "db_struct.h"
#include "tools_gui.h"

class BOOKWindow : public FXDialogBox
{
  FXDECLARE( BOOKWindow );

protected:
   BOOKWindow(){}
public:
   BOOKWindow( FXWindow *owner );
   void var_from_gui();
   void var_to_gui();
   void autoload_gui();

void on_load();

BOOKS 	book;
FXGroupBox* 	foo;
FXHorizontalFrame* 	hframe_1;
FXLabel* 	label_2;
FXComboBox* 	f_id;
FXComboBox* 	f_type;
FXHorizontalFrame* 	hframe_3;
FXButton* 	b_lesen;
FXButton* 	b_neu;
FXGroupBox* 	group_4;
FXHorizontalFrame* 	hframe_5;
FXText* 	f_title;
FXHorizontalFrame* 	hframe_6;
FXLabel* 	label_7;
FXComboBox* 	f_author;
FXGroupBox* 	group_8;
FXHorizontalFrame* 	hframe_9;
FXText* 	f_description;
FXMatrix* 	matrix_10;
FXLabel* 	label_11;
FXLabel* 	label_12;
FXLabel* 	label_13;
FXLabel* 	label_14;
FXLabel* 	label_15;
FXTextField* 	f_isbn;
FXTextField* 	f_ref;
FXComboBox* 	f_location;
FXComboBox* 	f_owner;
FXTextField* 	f_comment;
FXHorizontalSeparator* 	hseperator_16;
FXHorizontalFrame* 	hframe_17;
FXButton* 	del;
FXHorizontalFrame* 	hframe_18;
FXButton* 	ok;
FXButton* 	cancel;
  enum {
    ID_FIRST = FXDialogBox::ID_LAST,
ID_foo,
ID_hframe_1,
ID_label_2,
ID_f_id,
ID_f_type,
ID_hframe_3,
ID_b_lesen,
ID_b_neu,
ID_group_4,
ID_hframe_5,
ID_f_title,
ID_hframe_6,
ID_label_7,
ID_f_author,
ID_group_8,
ID_hframe_9,
ID_f_description,
ID_matrix_10,
ID_label_11,
ID_label_12,
ID_label_13,
ID_label_14,
ID_label_15,
ID_f_isbn,
ID_f_ref,
ID_f_location,
ID_f_owner,
ID_f_comment,
ID_hseperator_16,
ID_hframe_17,
ID_del,
ID_hframe_18,
ID_ok,
ID_cancel,
    ID_LAST
  };
virtual long button_lesen( FXObject *obj, FXSelector sel, void *ptr);
virtual long button_neu( FXObject *obj, FXSelector sel, void *ptr);
virtual long onBarcode( FXObject *obj, FXSelector sel, void *ptr);
virtual long button_del( FXObject *obj, FXSelector sel, void *ptr);
virtual long button_ok( FXObject *obj, FXSelector sel, void *ptr);
virtual long button_cancel( FXObject *obj, FXSelector sel, void *ptr);
};

#endif
