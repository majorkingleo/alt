#ifndef DB_SETTINGS_HWINDOW_H
#define DB_SETTINGS_HWINDOW_H

#include "fx.h"
#include "db_struct.h"
#include "tools_gui.h"

class DBSettingsWindow : public FXDialogBox
{
  FXDECLARE( DBSettingsWindow );

protected:
   DBSettingsWindow(){}
public:
   DBSettingsWindow( FXWindow *owner );
   void var_from_gui();
   void var_to_gui();
   void autoload_gui();

void on_load();

FXGroupBox* 	group_1;
FXMatrix* 	matrix_2;
FXLabel* 	label_3;
FXLabel* 	label_4;
FXLabel* 	label_5;
FXLabel* 	label_6;
FXTextField* 	host;
FXTextField* 	database;
FXTextField* 	username;
FXTextField* 	password;
FXHorizontalSeparator* 	hseperator_7;
FXHorizontalFrame* 	hframe_8;
FXHorizontalFrame* 	hframe_9;
FXButton* 	botton_ok;
FXButton* 	button_test;
FXButton* 	butten_cancel;
  enum {
    ID_FIRST = FXDialogBox::ID_LAST,
ID_group_1,
ID_matrix_2,
ID_label_3,
ID_label_4,
ID_label_5,
ID_label_6,
ID_host,
ID_database,
ID_username,
ID_password,
ID_hseperator_7,
ID_hframe_8,
ID_hframe_9,
ID_botton_ok,
ID_button_test,
ID_butten_cancel,
    ID_LAST
  };
virtual long on_ok( FXObject *obj, FXSelector sel, void *ptr);
virtual long on_test( FXObject *obj, FXSelector sel, void *ptr);
virtual long on_cancel( FXObject *obj, FXSelector sel, void *ptr);
};

#endif
