#ifndef SEQUENCE_HWINDOW_H
#define SEQUENCE_HWINDOW_H

#include "fx.h"
#include "db_struct.h"
#include "tools_gui.h"

class SequenceWindow : public FXDialogBox
{
  FXDECLARE( SequenceWindow );

protected:
   SequenceWindow(){}
public:
   SequenceWindow( FXWindow *owner );
   void var_from_gui();
   void var_to_gui();
   void autoload_gui();

SEQUENCE 	sequence;
FXGroupBox* 	group_1;
FXMatrix* 	matrix_2;
FXLabel* 	label_3;
FXLabel* 	label_4;
FXLabel* 	label_5;
FXLabel* 	label_6;
FXLabel* 	label_7;
DBEntry* 	name;
DBEntry* 	from;
DBEntry* 	to;
DBEntry* 	nextval;
DBEntry* 	format;
FXHorizontalFrame* 	hframe_8;
FXButton* 	del;
FXHorizontalFrame* 	hframe_9;
FXButton* 	ok;
FXButton* 	cancel;
  enum {
    ID_FIRST = FXDialogBox::ID_LAST,
ID_group_1,
ID_matrix_2,
ID_label_3,
ID_label_4,
ID_label_5,
ID_label_6,
ID_label_7,
ID_name,
ID_from,
ID_to,
ID_nextval,
ID_format,
ID_hframe_8,
ID_del,
ID_hframe_9,
ID_ok,
ID_cancel,
    ID_LAST
  };
virtual long button_del( FXObject *obj, FXSelector sel, void *ptr);
virtual long button_ok( FXObject *obj, FXSelector sel, void *ptr);
virtual long button_cancel( FXObject *obj, FXSelector sel, void *ptr);
};

#endif
