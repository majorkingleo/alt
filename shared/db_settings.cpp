#include "db_settings.h"
#include "icons.h"

using namespace Tools;

extern Tools::Database *db;

FXDEFMAP( DBSettingsWindow) DBSettingsWindowMap[]={
FXMAPFUNC( SEL_COMMAND, DBSettingsWindow::ID_botton_ok, DBSettingsWindow::on_ok),
FXMAPFUNC( SEL_COMMAND, DBSettingsWindow::ID_button_test, DBSettingsWindow::on_test),
FXMAPFUNC( SEL_COMMAND, DBSettingsWindow::ID_butten_cancel, DBSettingsWindow::on_cancel),
};
FXIMPLEMENT( DBSettingsWindow, FXDialogBox, DBSettingsWindowMap, ARRAYNUMBER( DBSettingsWindowMap ));

void DBSettingsWindow::var_from_gui()
{
}

void DBSettingsWindow::var_to_gui()
{
}

void DBSettingsWindow::autoload_gui()
{
}

DBSettingsWindow::DBSettingsWindow( FXWindow *owner )
: FXDialogBox( owner, "Datanbankeinstellungen", DECOR_ALL )
{
group_1 = new FXGroupBox(this, "Datenbankeinstellungen", LAYOUT_FILL_X|LAYOUT_FILL_Y|FRAME_RIDGE);
matrix_2 = new FXMatrix(group_1, 4);
label_3 = new FXLabel(matrix_2, "Host", 0);
label_4 = new FXLabel(matrix_2, "Datenbank", 0);
label_5 = new FXLabel(matrix_2, "Benutzername", 0);
label_6 = new FXLabel(matrix_2, "Passwort:", 0);
host = new FXTextField(matrix_2, 20, this, ID_host, TEXTFIELD_LIMITED|FRAME_SUNKEN|FRAME_THICK);
database = new FXTextField(matrix_2, 20, this, ID_database, TEXTFIELD_LIMITED|FRAME_SUNKEN|FRAME_THICK);
username = new FXTextField(matrix_2, 20, this, ID_username, TEXTFIELD_LIMITED|FRAME_SUNKEN|FRAME_THICK);
password = new FXTextField(matrix_2, 20, this, ID_password, FRAME_SUNKEN|FRAME_THICK|TEXTFIELD_PASSWD|FRAME_SUNKEN|FRAME_THICK|TEXTFIELD_PASSWD);
hseperator_7 = new FXHorizontalSeparator(group_1);
hframe_8 = new FXHorizontalFrame(group_1, LAYOUT_FILL_X);
hframe_9 = new FXHorizontalFrame(hframe_8, LAYOUT_CENTER_X|PACK_UNIFORM_WIDTH|PACK_UNIFORM_HEIGHT);
botton_ok = new FXButton(hframe_9, "ok", Icons->get_icon( "button_ok"), this, ID_botton_ok);
button_test = new FXButton(hframe_9, "Testen", Icons->get_icon( "wizard"), this, ID_button_test);
butten_cancel = new FXButton(hframe_9, "Abbrechen", Icons->get_icon( "button_cancel"), this, ID_butten_cancel);
on_load();

}
