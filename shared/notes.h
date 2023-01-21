#ifndef NOTES_HWINDOW_H
#define NOTES_HWINDOW_H

#include "fx.h"
#include "db_struct.h"
#include "tools_gui.h"

class NotesWindow : public FXDialogBox
{
  FXDECLARE( NotesWindow );

protected:
   NotesWindow(){}
public:
   NotesWindow( FXWindow *owner );
   void var_from_gui();
   void var_to_gui();
   void autoload_gui();

void on_load();

NOTES_HEADS 	notes_heads;
NOTES_LINES 	notes_lines;
FXTable* 	positionen;
FXGroupBox* 	group_1;
FXHorizontalFrame* 	hframe_2;
FXLabel* 	label_3;
FXComboBox* 	head_title;
FXComboBox* 	head_status;
FXHorizontalFrame* 	hframe_4;
FXButton* 	lesen;
FXButton* 	neu;
FXGroupBox* 	group_5;
FXHorizontalFrame* 	pos_parent;
FXHorizontalFrame* 	hframe_6;
FXButton* 	p_del;
FXButton* 	p_neu;
FXHorizontalFrame* 	hframe_7;
FXButton* 	del;
FXHorizontalFrame* 	hframe_8;
FXButton* 	ok;
FXButton* 	cancel;
  enum {
    ID_FIRST = FXDialogBox::ID_LAST,
ID_group_1,
ID_hframe_2,
ID_label_3,
ID_head_title,
ID_head_status,
ID_hframe_4,
ID_lesen,
ID_neu,
ID_group_5,
ID_pos_parent,
ID_hframe_6,
ID_p_del,
ID_p_neu,
ID_hframe_7,
ID_del,
ID_hframe_8,
ID_ok,
ID_cancel,
    ID_LAST
  };
virtual long button_combo_changed( FXObject *obj, FXSelector sel, void *ptr);
virtual long button_head_lesen( FXObject *obj, FXSelector sel, void *ptr);
virtual long button_head_neu( FXObject *obj, FXSelector sel, void *ptr);
virtual long button_pos_del( FXObject *obj, FXSelector sel, void *ptr);
virtual long button_pos_neu( FXObject *obj, FXSelector sel, void *ptr);
virtual long button_del( FXObject *obj, FXSelector sel, void *ptr);
virtual long button_ok( FXObject *obj, FXSelector sel, void *ptr);
virtual long button_cancel( FXObject *obj, FXSelector sel, void *ptr);
};

#endif
