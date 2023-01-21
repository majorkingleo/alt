#include "db_settings.h"

using namespace Tools;

const char *DBSETTINGS  = "DBSETTINGS";
const char *DB_HOST     = "host";
const char *DB_PASSWORD = "password";
const char *DB_USERNAME = "username";
const char *DB_DATABASE = "database";

void DBSettingsWindow::on_load()
{
  host->setText( getApp()->reg().readStringEntry( DBSETTINGS, DB_HOST, "localhost" ) );
  username->setText( getApp()->reg().readStringEntry( DBSETTINGS, DB_USERNAME, "root" ) );
  password->setText( getApp()->reg().readStringEntry( DBSETTINGS, DB_PASSWORD, "" ) );
  database->setText( getApp()->reg().readStringEntry( DBSETTINGS, DB_DATABASE, "atb" ) );
}

long DBSettingsWindow::on_ok( FXObject* obj, FXSelector sel, void* ptr )
{
  getApp()->reg().writeStringEntry( DBSETTINGS, DB_HOST, host->getText().text() );
  getApp()->reg().writeStringEntry( DBSETTINGS, DB_PASSWORD, password->getText().text() );
  getApp()->reg().writeStringEntry( DBSETTINGS, DB_USERNAME, username->getText().text() );
  getApp()->reg().writeStringEntry( DBSETTINGS, DB_DATABASE, database->getText().text() );

  return FXDialogBox::onCmdAccept( obj, sel, ptr );
}

long DBSettingsWindow::on_cancel( FXObject* obj, FXSelector sel, void* ptr )
{
  return FXDialogBox::onCmdCancel( obj, sel, ptr );
}

long DBSettingsWindow::on_test( FXObject* obj, FXSelector sel, void* )
{
  Database db( host->getText().text(),
	       username->getText().text(),
	       password->getText().text(),
	       database->getText().text() );

  if( !db )
    {
      FXMessageBox::error( this, MBOX_OK, "Datenbankfehler", "Die Verbindung konnte nicht hergestellt werden!" );
      return 1;
    }

  FXMessageBox::information( this, MBOX_OK, "Info", "Erfolg!\nDie Verbindung zur Datanbank konnte mit\n"
			     "diesen Einstellungen hergestellt werden!" );

  return 1;
}
