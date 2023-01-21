#include "db_struct.h"
#include "fx.h"
#include "fes.h"
#include "db_settings.h"
#include "book.h"
#include "liste.h"
#include <iostream>
#include "icons.h"
#include "mynotes.h"
#include "sequence.h"
#include "sequences.h"

using namespace Tools;

Database *db = 0;
IconManager *Icons = 0;

class Main : public FXMainWindow
{
  FXDECLARE( Main );

public:

  enum 
    {
      ID_BUTTON = FXMainWindow::ID_LAST,
      ID_SET_DATABASE,
      ID_QUIT,
      ID_BOOKS,
      ID_FELD_LISTE,
      ID_BUECHER_LISTE,
      ID_NOTES,
      ID_SEQUENCE,
      ID_SEQUENCES,
      ID_LAST
    };

  FXMenubar *menubar;

protected:
  Main() {}

public:
  
  Main( FXApp *app );

  ~Main()
  {
    delete Icons;
  }

  long onShow( FXObject* obj, FXSelector sel, void* );
  long onBooks( FXObject* obj, FXSelector sel, void* );
  long onQuit( FXObject* obj, FXSelector sel, void* );
  long onFeldListe( FXObject* obj, FXSelector sel, void* );
  long onBuecherListe( FXObject* obj, FXSelector sel, void* );
  long onSetDatabase( FXObject* obj, FXSelector sel, void* );
  long onNotes( FXObject* obj, FXSelector sel, void* );
  long onSequence( FXObject* obj, FXSelector sel, void* );
  long onSequences( FXObject* obj, FXSelector sel, void* );

  void create();
};

FXDEFMAP( Main ) MainMap[] = {
  FXMAPFUNC(SEL_COMMAND, Main::ID_BUTTON, Main::onShow ),
  FXMAPFUNC(SEL_COMMAND, Main::ID_BOOKS, Main::onBooks ),
  FXMAPFUNC(SEL_COMMAND, Main::ID_FELD_LISTE, Main::onFeldListe ),
  FXMAPFUNC(SEL_COMMAND, Main::ID_BUECHER_LISTE, Main::onBuecherListe ),
  FXMAPFUNC(SEL_COMMAND, Main::ID_SET_DATABASE, Main::onSetDatabase ),
  FXMAPFUNC(SEL_COMMAND, Main::ID_NOTES, Main::onNotes ),
  FXMAPFUNC(SEL_COMMAND, Main::ID_SEQUENCE, Main::onSequence ),
  FXMAPFUNC(SEL_COMMAND, Main::ID_SEQUENCES, Main::onSequences ),
  FXMAPFUNC(SEL_COMMAND, Main::ID_QUIT, Main::onQuit ),
  FXMAPFUNC(SEL_CLOSE,   0, Main::onQuit ),
  FXMAPFUNC(SEL_SIGNAL,  0, Main::onQuit )
};

FXIMPLEMENT( Main, FXMainWindow, MainMap, ARRAYNUMBER( MainMap ) );

Main::Main( FXApp *app )
  : FXMainWindow( app, "ALT", NULL, NULL, DECOR_CLOSE | DECOR_TITLE | DECOR_MINIMIZE )
{
  Icons = new IconManager( app );
  IconManager::implement();

  menubar = new FXMenubar( this, LAYOUT_SIDE_TOP|LAYOUT_FILL_X );

  FXMenuPane *settings = new FXMenuPane( this );
  new FXMenuTitle( menubar, "Einstellungen", NULL, settings );
  new FXMenuCommand( settings, "Datenbankverbindung", NULL, this, ID_SET_DATABASE );

  FXMenuPane *database = new FXMenuPane( this );
  new FXMenuTitle( menubar, "Datenbank", NULL, database );  
  new FXMenuCommand( database, "Feldstamm warten", NULL, this, ID_BUTTON );
  new FXMenuCommand( database, "Buecher warten", NULL, this, ID_BOOKS );
  new FXMenuCommand( database, "Notitzen warten", NULL, this, ID_NOTES );
  new FXMenuCommand( database, "Sequence warten", NULL, this, ID_SEQUENCE );
  new FXMenuCommand( database, "Sequencen warten", NULL, this, ID_SEQUENCES );

  FXMenuPane *listen = new FXMenuPane( this );
  new FXMenuTitle( menubar, "Listen", NULL, listen );
  new FXMenuCommand( listen, "Liste Feldstamm", NULL, this, ID_FELD_LISTE );
  new FXMenuCommand( listen, "Liste Buecher", NULL, this, ID_BUECHER_LISTE );

  FXMenuPane *program = new FXMenuPane( this );
  new FXMenuTitle( menubar, "Programm", NULL, program );  
  new FXMenuCommand( program, "Beenden", NULL, this, ID_QUIT );

  FXHorizontalFrame *f = new FXHorizontalFrame( this, 
						LAYOUT_FILL_X | LAYOUT_FILL_Y | FRAME_SUNKEN | FRAME_THICK );

  FXText *text = new FXText( f, 0, 0, LAYOUT_CENTER_X | TEXT_READONLY | LAYOUT_FIX_WIDTH | LAYOUT_FIX_HEIGHT, 
			     0, 0, 110, 60);

  text->setText( "         ALT\n\n Axi Library Tool\n    Version " VERSION );
  text->setBackColor( getBackColor() );
}

void Main::create()
{
  FXMainWindow::create();

  Icons->create();

  show( PLACEMENT_DEFAULT );
}

long Main::onShow( FXObject* obj, FXSelector sel, void* )
{
  FESWindow fes( this );

  fes.execute( PLACEMENT_DEFAULT );
  return 1;
}

long Main::onSetDatabase( FXObject* obj, FXSelector sel, void* )
{
  DBSettingsWindow set( this );

  set.execute( PLACEMENT_DEFAULT );
  return 1;
}

long Main::onNotes( FXObject* obj, FXSelector sel, void* )
{
  MyNotes set( this );

  set.execute( PLACEMENT_DEFAULT );
  return 1;
}

long Main::onSequence( FXObject* obj, FXSelector sel, void* )
{
  SequenceWindow set( this );

  set.execute( PLACEMENT_DEFAULT );
  return 1;
}

long Main::onSequences( FXObject* obj, FXSelector sel, void* )
{
  SequencesWindow set( this );

  set.execute( PLACEMENT_DEFAULT );
  return 1;
}

long Main::onQuit( FXObject* obj, FXSelector sel, void* )
{
  getApp()->exit(0);

  return 1;
}

long Main::onBooks( FXObject* obj, FXSelector sel, void* )
{
  BOOKWindow b( this );

  b.execute( PLACEMENT_DEFAULT );
  return 1;
}

long Main::onFeldListe( FXObject* obj, FXSelector sel, void* )
{
  Liste l( this, "Liste Feldstamm" );

  l.create_liste<FES>();

  l.execute( PLACEMENT_DEFAULT );

  return 1;
}

long Main::onBuecherListe( FXObject* obj, FXSelector sel, void* )
{
  Liste l( this, "Liste Buecher" );

  l.create_liste<BOOKS>();

  l.execute( PLACEMENT_DEFAULT );

  return 1;
}


extern const char *DBSETTINGS;
extern const char *DB_HOST;
extern const char *DB_PASSWORD;
extern const char *DB_USERNAME;
extern const char *DB_DATABASE;

int main( int argc, char **argv )
{
  FXApp app( "ALT", "kingleo" );

  app.init( argc, argv );

  new Main( &app );

  app.create();

  Ref<Database> database = new Database( app.reg().readStringEntry( DBSETTINGS, DB_HOST, "localhost" ),
					 app.reg().readStringEntry( DBSETTINGS, DB_USERNAME, "root" ),
					 app.reg().readStringEntry( DBSETTINGS, DB_PASSWORD, "" ),
					 app.reg().readStringEntry( DBSETTINGS, DB_DATABASE, "atb" ) );

  db = database;

  return app.run();
}
