#ifndef FES_HWINDOW_H
#define FES_HWINDOW_H

#include "fx.h"
#include "db_struct.h"
#include "tools_gui.h"

class FESWindow : public FXDialogBox
{
  FXDECLARE( FESWindow );

protected:
   FESWindow(){}
public:
   FESWindow( FXWindow *owner );
   void var_from_gui();
   void var_to_gui();
   void autoload_gui();

void on_load();

FES 	fes;
FXGroupBox* 	group_1;
FXHorizontalFrame* 	hframe_2;
FXLabel* 	label_3;
FXComboBox* 	feldid;
FXButton* 	lesen;
FXButton* 	neu;
FXGroupBox* 	group_4;
FXHorizontalFrame* 	hframe_5;
FXText* 	description;
FXHorizontalFrame* 	hframe_6;
FXButton* 	del;
FXHorizontalFrame* 	hframe_7;
FXButton* 	ok;
FXButton* 	cancel;
  enum {
    ID_FIRST = FXDialogBox::ID_LAST,
ID_group_1,
ID_hframe_2,
ID_label_3,
ID_feldid,
ID_lesen,
ID_neu,
ID_group_4,
ID_hframe_5,
ID_description,
ID_hframe_6,
ID_del,
ID_hframe_7,
ID_ok,
ID_cancel,
    ID_LAST
  };
virtual long button_combo_changed( FXObject *obj, FXSelector sel, void *ptr);
virtual long button_lesen( FXObject *obj, FXSelector sel, void *ptr);
virtual long button_neu( FXObject *obj, FXSelector sel, void *ptr);
virtual long button_del( FXObject *obj, FXSelector sel, void *ptr);
virtual long button_ok( FXObject *obj, FXSelector sel, void *ptr);
virtual long button_cancel( FXObject *obj, FXSelector sel, void *ptr);
};

#endif
