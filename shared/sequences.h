#ifndef SEQUENCES_HWINDOW_H
#define SEQUENCES_HWINDOW_H

#include "fx.h"
#include "db_struct.h"
#include "tools_gui.h"

class SequencesWindow : public FXDialogBox
{
  FXDECLARE( SequencesWindow );

protected:
   SequencesWindow(){}
public:
   SequencesWindow( FXWindow *owner );
   void var_from_gui();
   void var_to_gui();
   void autoload_gui();

void on_load();

SEQUENCE 	sequence;
FXGroupBox* 	group_1;
FXHorizontalFrame* 	spread_parent;
FXHorizontalFrame* 	hframe_2;
FXButton* 	del;
FXHorizontalFrame* 	hframe_3;
FXButton* 	ok;
FXButton* 	cancel;
  enum {
    ID_FIRST = FXDialogBox::ID_LAST,
ID_group_1,
ID_spread_parent,
ID_hframe_2,
ID_del,
ID_hframe_3,
ID_ok,
ID_cancel,
    ID_LAST
  };
virtual long button_del( FXObject *obj, FXSelector sel, void *ptr);
virtual long button_ok( FXObject *obj, FXSelector sel, void *ptr);
virtual long button_cancel( FXObject *obj, FXSelector sel, void *ptr);
};

#endif
