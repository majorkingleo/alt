#include "elements2.h"
#include "generator.h"
#include "cppdir.h"
#include "string_utils.h"

namespace {

enum MODE
  {
    MODE_DECLARE,
    MODE_DECLARE_ID,
    MODE_DECLARE_CALLBACK,
    MODE_DEFINE_CALLBACK,
    MODE_DEFINE_FROM_GUI,
    MODE_DEFINE_TO_GUI,
    MODE_DEFINE_AUTOLOAD_GUI,
    MODE_IMPLEMENT
  };

using namespace GEN;
using namespace std;


class GenStructs : public GenCode
{
public:
  struct Constructor
  {
    std::string type;

    Constructor( const std::string &type ) : type( type ) {}

    std::vector<std::string> options;

    void add( const std::string &c )
    {
      options.push_back( c );
    }
  };


protected:
  MODE mode;

public:
  GenStructs* parent;

public:
  GenStructs( MODE mode ) 
    : GenCode(), mode( mode ), parent( 0 ) 
  {}

  virtual std::stringstream & out() 
  {  
    return parent->out();
  }

  virtual std::string get_parent_name() = 0;

  virtual unsigned get_count()
  {
    return parent->get_count();
  }

  virtual std::string get_window_name()
  {
    return parent->get_window_name();
  }
};

std::ostream & operator<<( std::ostream & out, const GenStructs::Constructor & c )
{
  out << c.type << "(";
  
  for( unsigned i = 0; i < c.options.size(); i++ )
    {
      if( i )
	out << ", ";
      
	  out << c.options[i];
    }
  
  out << ")";
  
  return out;
}

class GenWindow : public GenStructs
{
  Ref<std::stringstream> sout;

  std::string file_name;
  std::string dir;
  std::string window_name;
  unsigned counter;

public:
  GenWindow( MODE mode, const std::string &dir ) 
    : GenStructs( mode ), 
      dir( dir ),
      counter( 0 )
  {
    sout = new std::stringstream();
  }

  GenWindow( const GenWindow &w ) 
    : GenStructs( w.mode ), 
      dir( w.dir ),
      counter(0)
  {
    sout = new std::stringstream( w.sout->str() );
  }

  const std::string & get_file_name() const { return file_name; }

  std::string get_window_name()
  {
    return window_name;
  }

  std::string get_parent_name()
  {
    return "this";
  }

  unsigned get_count()
  {
    return ++counter;
  }

  bool want_listen( const std::string &name )
  {
    if( icase_cmp( "window", name ) )
      return true;

    return false;
  }

  bool parse_begin( Ref<GenConstructElement> el )
  {
    std::string n = el->tag.get_option( "name" ) + "Window";

    window_name = n;

    switch( mode )
      {
      case MODE_DECLARE:
	file_name = el->tag.get_option( "file_h" );
	return parse_declare_begin( el );
	break;

      case MODE_DECLARE_ID:
	file_name = el->tag.get_option( "file_h" );
	return parse_declare_id_begin( el );
	break;

      case MODE_DECLARE_CALLBACK:
	file_name = el->tag.get_option( "file_h" );
	break;

      case MODE_DEFINE_CALLBACK:
	file_name = el->tag.get_option( "file_cpp" );
	return parse_define_callback_begin( el );
	break;

      case MODE_DEFINE_FROM_GUI:
	file_name = el->tag.get_option( "file_cpp" );
	out() << "void " << n << "::var_from_gui()\n{\n";
	break;

      case MODE_DEFINE_TO_GUI:
	file_name = el->tag.get_option( "file_cpp" );
	out() << "void " << n << "::var_to_gui()\n{\n";
	break;

      case MODE_DEFINE_AUTOLOAD_GUI:
	file_name = el->tag.get_option( "file_cpp" );
	out() << "void " << n << "::autoload_gui()\n{\n";
	break;
	
      case MODE_IMPLEMENT:
	file_name = el->tag.get_option( "file_cpp" );
	return parse_implement_begin( el );
	break;

      default:
	error = "unknown mode";
	return false;
      }

    return true;
  }

  bool parse_end(  Ref<GenConstructElement> el  )
  {
    std::string n = el->tag.get_option( "name" ) + "Window";

    switch( mode )
      {
      case MODE_DECLARE:

	if( !update_file2( out(), CppDir::concat_dir( dir, file_name ) ) )
	  {
	    error = "updating/creating file failed";
	    return false;
	  }

	break;

      case MODE_DECLARE_ID:

	if( !parse_declare_id_end( el ) )
	  return false;

	if( !update_file2( out(), CppDir::concat_dir( dir, file_name ), true ) )
	  {
	    error = "updating/creating file failed";
	    return false;
	  }

	sout = new std::stringstream();

	break;


      case MODE_DECLARE_CALLBACK:

	out() << "};\n\n";
	out() << "#endif\n";

	if( !update_file2( out(), CppDir::concat_dir( dir, file_name ), true ) )
	  {
	    error = "updating/creating file failed";
	    return false;
	  }

	break;


      case MODE_DEFINE_CALLBACK:
	if( ! parse_define_callback_end( el ) )
	  return false;

	if( !update_file2( out(), CppDir::concat_dir( dir, file_name ) ) )
	  {
	    error = "updating/creating file failed";
	    return false;
	  }

	break;


      case MODE_DEFINE_AUTOLOAD_GUI:
      case MODE_DEFINE_FROM_GUI:
      case MODE_DEFINE_TO_GUI:
	out() << "}\n\n";
	if( !update_file2( out(), CppDir::concat_dir( dir, file_name ), true ) )
	  {
	    error = "updating/creating file failed";
	    return false;
	  }

	break;


      case MODE_IMPLEMENT:

	if( !el->tag.get_option( "on_load" ).empty() )
	  {
	    out() << el->tag.get_option( "on_load" ) << "();\n";
	  }

	out() << "\n}\n";

	if( !update_file2( out(), CppDir::concat_dir( dir, file_name ), true ) )
	  {
	    error = "updating/creating file failed";
	    return false;
	  }

	break;

      default:
	error = "unknown mode";
	return false;
      }

    return true;
  }

  bool parse_define_callback_begin( Ref<GenConstructElement> el )
  {
    out() << "#include \"" << el->tag.get_option( "file_h" ) << "\"\n";
    out() << "#include \"icons.h\"\n\n";
    out() << "using namespace Tools;\n\n";
    
    std::string n = el->tag.get_option( "name" ) + "Window";
    std::string nm = n + "Map";

    out() << "extern Tools::Database *db;\n\n";

    out() << "FXDEFMAP( " << n << ") " << nm << "[]={\n";

    return true;
  }

  bool parse_define_callback_end( Ref<GenConstructElement> el )
  {
    out() << "};\n";

    std::string n = el->tag.get_option( "name" ) + "Window";
    std::string nm = n + "Map";

    out() << "FXIMPLEMENT( " << n << ", FXDialogBox, " << nm << ", ARRAYNUMBER( " << nm << " ));\n\n";

    return true;
  }

  bool parse_declare_begin( Ref<GenConstructElement> el )
  {
    std::string f_name = el->tag.get_option( "file_h" );
    
    f_name = toupper( substitude( f_name, ".", "_" ) ) + "WINDOW_H";

    std::string n = el->tag.get_option( "name" ) + "Window";

    out() << "#ifndef " << f_name << '\n'
	  << "#define " << f_name << "\n\n";
    out() << "#include \"fx.h\"\n";
    out() << "#include \"db_struct.h\"\n";
    out() << "#include \"tools_gui.h\"\n\n";

    out() << "class " << n << " : public FXDialogBox\n{\n";
    out() << "  FXDECLARE( " << n << " );\n\n";
    out() << "protected:\n";
    out() << "   " << n << "(){}\n";
    out() << "public:\n";
    out() << "   " << n << "( FXWindow *owner );\n";
    out() << "   void var_from_gui();\n";
    out() << "   void var_to_gui();\n";
    out() << "   void autoload_gui();\n\n";

    if( !el->tag.get_option( "on_load" ).empty() )
      {
	out() << "void " << el->tag.get_option( "on_load" ) << "();\n\n";
      }

    if( !el->tag.get_option( "data" ).empty() )
      {
	std::vector<std::string> sl = split_simple( el->tag.get_option( "data" ), "," );
	
	for( unsigned i = 0; i < sl.size(); i++ )
	  {
	    std::vector<std::string> s = split_simple( sl[i], ":" );

	    if( s.size() != 2 )
	      {
		error = "data field incorrect: " + el->tag.get_option( "data" );
		return false;
	      }

	    out() << s[0] << " \t" << s[1] << ";\n";
	  }
      }


    return true;
  }

  bool parse_declare_id_begin( Ref<GenConstructElement> el )
  {
    out() << "  enum {\n";
    out() << "    ID_FIRST = FXDialogBox::ID_LAST,\n";
    return true;
  }

  bool parse_declare_id_end( Ref<GenConstructElement> el )
  {
    out() << "    ID_LAST\n";
    out() << "  };\n";

    return true;
  }

  bool parse_implement_begin( Ref<GenConstructElement> el )
  {
    std::string name = el->tag.get_option( "name" );

    out() << name << "Window::" << name << "Window( FXWindow *owner )\n";
    out() << ": FXDialogBox( owner, \"" << el->tag.get_option( "title" ) << "\", DECOR_ALL )\n";
    out() << "{\n";

    return true;
  }

  virtual std::stringstream & out() 
  {
    return *sout;
  }

  GenWindow* clone()
  {
    return new GenWindow( *this );
  }
};  

class GenFrame : public GenStructs
{
public:
  GenFrame( MODE mode )
    : GenStructs( mode )
  {}

  virtual std::string get_listen_name() = 0;
  virtual std::string get_type_name() = 0;
  virtual std::string get_name() = 0;
  virtual std::string get_constructor( Ref<GenConstructElement> el ) = 0;

  bool want_listen( const std::string & name )
  {
    if( icase_cmp( get_listen_name(), name ) )
      return true;

    return false;
  }

  bool parse_begin( Ref<GenConstructElement> el )
  {
    switch( mode )
      {
      case MODE_DECLARE:
	out() << get_type_name() << "* \t" << get_name() << ";\n";
	return true;
      case MODE_DECLARE_ID:
	out() << "ID_" << get_name() << ",\n";
	return true;

      case MODE_IMPLEMENT:
	out() << get_name() << " = new " << get_constructor( el ) << ";\n";
	return true;

      case MODE_DECLARE_CALLBACK:
      case MODE_DEFINE_CALLBACK:
      case MODE_DEFINE_FROM_GUI:
      case MODE_DEFINE_TO_GUI:
      case MODE_DEFINE_AUTOLOAD_GUI:
	return true;

      default:
	error = "unknown mode";
	return false;
      }
  }   

  std::string prepare_layout( std::string s )
  {
    return substitude( s, ",", "|" );
  }

  std::string get_parent_name()
  {
    return get_name();
  }
};

class GenGroup : public GenFrame
{
  std::string name;

 public:
  GenGroup( MODE mode )
    : GenFrame( mode )
  {}
  
  std::string get_listen_name() { return "group"; }
  std::string get_type_name() { return "FXGroupBox"; }
  std::string get_name() { return name; }
  
  bool parse_begin( Ref<GenConstructElement> el )
  {
    if( name.empty() && (name = el->tag.get_option( "name" )).empty() )
      {
	name = "group_" + x2s( get_count() );
      }

    return GenFrame::parse_begin( el );
  }

  GenGroup* clone() { return new GenGroup( *this ); }

  std::string get_constructor( Ref<GenConstructElement> el )
  {
    Constructor c( get_type_name() );

    c.add( parent->get_parent_name() );
    c.add( '"' + el->tag.get_option( "title" )  + '"' );

    std::string layout = el->tag.get_option( "layout" );

    if( layout.empty() )
      {
	layout = "FRAME_RIDGE";
      }
    
    c.add( prepare_layout( layout ) );

    return x2s( c );
  }
};

class GenHFrame : public GenFrame
{
  std::string name;

 public:
  GenHFrame( MODE mode )
    : GenFrame( mode )
  {}
  
  std::string get_listen_name() { return "hframe"; }
  std::string get_type_name() { return "FXHorizontalFrame"; }
  std::string get_name() { return name; }
  
  bool parse_begin( Ref<GenConstructElement> el )
  {
    if( name.empty() && (name = el->tag.get_option( "name" )).empty() )
      {
	name = "hframe_" + x2s( get_count() );
      }

    return GenFrame::parse_begin( el );
  }

  GenHFrame* clone() { return new GenHFrame( *this ); }

  std::string get_constructor( Ref<GenConstructElement> el )
  {
    Constructor c( get_type_name() );

    c.add( parent->get_parent_name() );  

    std::string layout = el->tag.get_option( "layout" );

    if( !layout.empty() )
      {
	c.add( prepare_layout( layout ) );
      }

    if( el->tag.get_option( "nopadding" ) == "true" )
      {
	if( layout.empty() )
	  c.add( "0" );

	c.add( "0" );
	c.add( "0" );
	c.add( "0" );
	c.add( "0" );
	c.add( "0" );
	c.add( "0" );
	c.add( "0" );
	c.add( "0" );
      }
    
    return x2s( c );
  }
};

class GenVFrame : public GenFrame
{
  std::string name;

 public:
  GenVFrame( MODE mode )
    : GenFrame( mode )
  {}
  
  std::string get_listen_name() { return "vframe"; }
  std::string get_type_name() { return "FXVerticalFrame"; }
  std::string get_name() { return name; }
  
  bool parse_begin( Ref<GenConstructElement> el )
  {
    if( name.empty() && (name = el->tag.get_option( "name" )).empty() )
      {
	name = "vframe_" + x2s( get_count() );
      }

    return GenFrame::parse_begin( el );
  }

  GenVFrame* clone() { return new GenVFrame( *this ); }

  std::string get_constructor( Ref<GenConstructElement> el )
  {
    Constructor c( get_type_name() );

    c.add( parent->get_parent_name() );  

    std::string layout = el->tag.get_option( "layout" );

    if( !layout.empty() )
      {
	c.add( prepare_layout( layout ) );
      }
    
    return x2s( c );
  }
};


class GenLabel : public GenFrame
{
  std::string name;

 public:
  GenLabel( MODE mode )
    : GenFrame( mode )
  {}
  
  std::string get_listen_name() { return "label"; }
  std::string get_type_name() { return "FXLabel"; }
  std::string get_name() { return name; }
  
  bool parse_begin( Ref<GenConstructElement> el )
  {
    if( name.empty() && (name = el->tag.get_option( "name" )).empty() )
      {
	name = "label_" + x2s( get_count() );
      }

    return GenFrame::parse_begin( el );
  }

  GenLabel* clone() { return new GenLabel( *this ); }

  std::string get_constructor( Ref<GenConstructElement> el )
  {
    Constructor c( get_type_name() );

    c.add( parent->get_parent_name() );  
    c.add( '"' + el->tag.get_option( "title" ) + '"' );
    c.add( "0" );

    std::string layout = el->tag.get_option( "layout" );

    if( !layout.empty() )
      {
	c.add( prepare_layout( layout ) );
      }
    
    return x2s( c );
  }
};

class GenHSep : public GenFrame
{
  std::string name;

 public:
  GenHSep( MODE mode )
    : GenFrame( mode )
  {}
  
  std::string get_listen_name() { return "hseparator"; }
  std::string get_type_name() { return "FXHorizontalSeparator"; }
  std::string get_name() { return name; }
  
  bool parse_begin( Ref<GenConstructElement> el )
  {
    if( name.empty() && (name = el->tag.get_option( "name" )).empty() )
      {
	name = "hseperator_" + x2s( get_count() );
      }

    return GenFrame::parse_begin( el );
  }

  GenHSep* clone() { return new GenHSep( *this ); }

  std::string get_constructor( Ref<GenConstructElement> el )
  {
    Constructor c( get_type_name() );

    c.add( parent->get_parent_name() );  

    std::string layout = el->tag.get_option( "layout" );

    if( !layout.empty() )
      {
	c.add( prepare_layout( layout ) );
      }
    
    return x2s( c );
  }
};

class GenVSep : public GenFrame
{
  std::string name;

 public:
  GenVSep( MODE mode )
    : GenFrame( mode )
  {}
  
  std::string get_listen_name() { return "vseparator"; }
  std::string get_type_name() { return "FXVerticalSeparator"; }
  std::string get_name() { return name; }
  
  bool parse_begin( Ref<GenConstructElement> el )
  {
    if( name.empty() && (name = el->tag.get_option( "name" )).empty() )
      {
	name = "vseperator_" + x2s( get_count() );
      }

    return GenFrame::parse_begin( el );
  }

  GenVSep* clone() { return new GenVSep( *this ); }

  std::string get_constructor( Ref<GenConstructElement> el )
  {
    Constructor c( get_type_name() );

    c.add( parent->get_parent_name() );  

    std::string layout = el->tag.get_option( "layout" );

    if( !layout.empty() )
      {
	c.add( prepare_layout( layout ) );
      }
    
    return x2s( c );
  }
};

class GenMatrix : public GenFrame
{
  std::string name;

 public:
  GenMatrix( MODE mode )
    : GenFrame( mode )
  {}
  
  std::string get_listen_name() { return "matrix"; }
  std::string get_type_name() { return "FXMatrix"; }
  std::string get_name() { return name; }
  
  bool parse_begin( Ref<GenConstructElement> el )
  {
    if( name.empty() && (name = el->tag.get_option( "name" )).empty() )
      {
	name = "matrix_" + x2s( get_count() );
      }

    return GenFrame::parse_begin( el );
  }

  GenMatrix* clone() { return new GenMatrix( *this ); }

  std::string get_constructor( Ref<GenConstructElement> el )
  {
    Constructor c( get_type_name() );

    c.add( parent->get_parent_name() );  
    c.add( el->tag.get_option( "fields" ) );

    std::string layout = el->tag.get_option( "layout" );

    if( !layout.empty() )
      {
	c.add( prepare_layout( layout ) );
      }
    
    return x2s( c );
  }
};

class GenCommand : public GenStructs
{
public:
  GenCommand( MODE mode )
    : GenStructs( mode )
  {}

  virtual std::string get_listen_name() = 0;
  virtual std::string get_type_name() = 0;
  virtual std::string get_name() = 0;
  virtual std::string get_constructor( Ref<GenConstructElement> el ) = 0;

  bool want_listen( const std::string & name )
  {
    if( icase_cmp( get_listen_name(), name ) )
      return true;

    return false;
  }

  bool parse_begin( Ref<GenConstructElement> el )
  {
    switch( mode )
      {
      case MODE_DECLARE:
	out() << get_type_name() << "* \t" << get_name() << ";\n";
	return true;
      case MODE_DECLARE_ID:
	out() << "ID_" << get_name() << ",\n";
	return true;

      case MODE_IMPLEMENT:
	{
	  out() << get_name() << " = new " << get_constructor( el ) << ";\n";
	  
	  if( el->tag.get_option( "readonly" ) == "true" )
	    {
	      out() << get_name() << "->setEditable( false );\n";
	    }
	  std::string s;
	  if( !( s =  el->tag.get_option( "tooltip" ) ).empty() )
	    {
	      out() << get_name() << "->setTipText( \"" << s << "\" );\n"; 
	    }
	}
	return true;

      case MODE_DECLARE_CALLBACK:
	{
	  std::string cb = el->tag.get_option( "callback" );
	  if( !cb.empty() )
	      out() << "virtual long " << cb << "( FXObject *obj, FXSelector sel, void *ptr);\n";
	  return true;
	}

      case MODE_DEFINE_CALLBACK:
	{
	  std::string cb = el->tag.get_option( "callback" );
	  if( !cb.empty() )
	    out() << "FXMAPFUNC( SEL_COMMAND, " 
		  << get_window_name() << "::ID_" 
		  << get_name() << ", "
		  << get_window_name() << "::" << cb << "),\n";

	  return true;
	}

      case MODE_DEFINE_FROM_GUI:
	{
	  std::string db_ref = el->tag.get_option( "db_ref" );

	  if( !db_ref.empty() )
	    {
	      out() << "  " << db_ref << " = " << get_name() << "->getText().text();\n";
	    }
	  return true;
	}

      case MODE_DEFINE_TO_GUI:
	{
	  std::string db_ref = el->tag.get_option( "db_ref" );

	  if( !db_ref.empty() )
	    {
	      out() << "  " << get_name() << "->setText( " << db_ref << ".str().c_str());\n";
	    }
	  return true;
	}
	return true;

      case MODE_DEFINE_AUTOLOAD_GUI:
	return true;

      default:
	error = "unknown mode";
	return false;
      }
  }   

  std::string prepare_layout( std::string s )
  {
    return substitude( s, ",", "|" );
  }

  std::string get_parent_name()
  {
    return get_name();
  }
};

class GenButton : public GenCommand
{
  std::string name;

 public:
  GenButton( MODE mode )
    : GenCommand( mode )
  {}
  
  std::string get_listen_name() { return "button"; }
  std::string get_type_name() { return "FXButton"; }
  std::string get_name() { return name; }
  
  bool parse_begin( Ref<GenConstructElement> el )
  {
    if( name.empty() && (name = el->tag.get_option( "name" )).empty() )
      {
	name = "button_" + x2s( get_count() );
      }

    return GenCommand::parse_begin( el );
  }

  GenButton* clone() { return new GenButton( *this ); }

  std::string get_constructor( Ref<GenConstructElement> el )
  {
    Constructor c( get_type_name() );

    c.add( parent->get_parent_name() );  
    c.add( '"' + el->tag.get_option( "title" ) + '"' );

    if( el->tag.get_option( "icon" ).size() )
      c.add( "Icons->get_icon( \"" + el->tag.get_option( "icon" ) + "\")" );
    else
      c.add( "NULL" );

    c.add( "this" );
    c.add( "ID_" + get_name() );   

    std::string layout = el->tag.get_option( "layout" );

    if( !layout.empty() )
      {
	c.add( prepare_layout( layout ) );
      }
    
    return x2s( c );
  }
};


class GenEntry : public GenCommand
{
  std::string name;

 public:
  GenEntry( MODE mode )
    : GenCommand( mode )
  {}
  
  std::string get_listen_name() { return "entry"; }
  std::string get_type_name() { return "FXTextField"; }
  std::string get_name() { return name; }
  
  bool parse_begin( Ref<GenConstructElement> el )
  {
    if( name.empty() && (name = el->tag.get_option( "name" )).empty() )
      {
	name = "entry_" + x2s( get_count() );
      }

    return GenCommand::parse_begin( el );
  }

  GenEntry* clone() { return new GenEntry( *this ); }

  std::string get_constructor( Ref<GenConstructElement> el )
  {
    Constructor c( get_type_name() );

    c.add( parent->get_parent_name() );  
    
    std::string width = el->tag.get_option( "width" );

    if( width.empty() )
      {
	std::string db_ref = el->tag.get_option( "db_ref" );
	if( !db_ref.empty() )
	  {
	    width = db_ref + ".get_size()";
	  }
      }

    if( width.empty() )
      width = "20";

    c.add( width );

    c.add( "this" );
    c.add( "ID_" + get_name() );   

    std::string layout = el->tag.get_option( "layout" );

    if( !layout.empty() )
      {
	c.add( prepare_layout( layout ) );
      }
    
    return x2s( c );
  }
};

class GenDBEntry : public GenCommand
{
  std::string name;

 public:
  GenDBEntry( MODE mode )
    : GenCommand( mode )
  {}
  
  std::string get_listen_name() { return "dbentry"; }
  std::string get_type_name() { return "DBEntry"; }
  std::string get_name() { return name; }
  
  bool parse_begin( Ref<GenConstructElement> el )
  {
    if( name.empty() && (name = el->tag.get_option( "name" )).empty() )
      {
	name = "dbentry_" + x2s( get_count() );
      }

    return GenCommand::parse_begin( el );
  }

  GenDBEntry* clone() { return new GenDBEntry( *this ); }

  std::string get_constructor( Ref<GenConstructElement> el )
  {
    Constructor c( get_type_name() );

    c.add( "&" + el->tag.get_option( "db_ref" ) );
    c.add( parent->get_parent_name() );  
    
    std::string width = el->tag.get_option( "width" );

    if( width.empty() )
      {
	std::string db_ref = el->tag.get_option( "db_ref" );
	if( !db_ref.empty() )
	  {
	    width = db_ref + ".get_size()";
	  }
      }

    if( width.empty() )
      width = "20";

    c.add( width );

    c.add( "this" );
    c.add( "ID_" + get_name() );   

    std::string layout = el->tag.get_option( "layout" );

    if( !layout.empty() )
      {
	c.add( prepare_layout( layout ) );
      }
    
    return x2s( c );
  }
};

class GenText : public GenCommand
{
  std::string name;

 public:
  GenText( MODE mode )
    : GenCommand( mode )
  {}
  
  std::string get_listen_name() { return "text"; }
  std::string get_type_name() { return "FXText"; }
  std::string get_name() { return name; }
  
  bool parse_begin( Ref<GenConstructElement> el )
  {
    if( name.empty() && (name = el->tag.get_option( "name" )).empty() )
      {
	name = "text_" + x2s( get_count() );
      }

    return GenCommand::parse_begin( el );
  }

  GenText* clone() { return new GenText( *this ); }

  std::string get_constructor( Ref<GenConstructElement> el )
  {
    Constructor c( get_type_name() );

    c.add( parent->get_parent_name() );  
    
    c.add( "this" );
    c.add( "ID_" + get_name() );   

    std::string layout = el->tag.get_option( "layout" );

    if( !layout.empty() )
      {
	c.add( prepare_layout( layout ) );
      }
    
    return x2s( c );
  }
};


class GenCombo : public GenCommand
{
  std::string name;

 public:
  GenCombo( MODE mode )
    : GenCommand( mode )
  {}
  
  std::string get_listen_name() { return "combobox"; }
  std::string get_type_name() { return "FXComboBox"; }
  std::string get_name() { return name; }
  
  bool parse_begin( Ref<GenConstructElement> el )
  {
    if( name.empty() && (name = el->tag.get_option( "name" )).empty() )
      {
	name = "combobox_" + x2s( get_count() );
      }

    if( !GenCommand::parse_begin( el ) )
      return false;

    std::string autoload = el->tag.get_option( "autoload" );

    if( mode == MODE_DEFINE_AUTOLOAD_GUI && !autoload.empty() )
      {
	out() << "{\n";

	std::vector<std::string> sl = split_simple( autoload, ":" );

	if( sl.size() != 2 )
	  {
	    error = "autoload vars are incorrect: " + autoload;
	    return false;
	  }

	out() << get_name() << "->clearItems();\n";
	out() << sl[0] << " b[100];\n";
	out() << "DBInLimit limit( 100 );\n";
	out() << "int res;\n\n";

	out() << "while( ( res = StdSqlSelect( *db, \"select distinct(" 
	      << sl[0] << "." << sl[1] << ") from " << sl[0]
	      << " order by " << sl[1] 
	      <<"\", DBInArrayList() >> b, limit ) ) > 0 )\n {\n";

	out() << "   for( int i = 0; i < res; i++ )\n";
	out() << "      " << get_name() << "->appendItem( b[i]." << sl[1] << ".str().c_str() );\n";
	out() << "  }\n";
	out() << "  db->rollback();\n";

	out() << "}\n";
      }

    return true;
  }

  GenCombo* clone() { return new GenCombo( *this ); }

  std::string get_constructor( Ref<GenConstructElement> el )
  {
    Constructor c( get_type_name() );

    c.add( parent->get_parent_name() );  
    
    std::string width = el->tag.get_option( "width" );
    std::string cases;

    if( width.empty() )
      {
	std::string db_ref = el->tag.get_option( "db_ref" );
	if( !db_ref.empty() )
	  {
	    width = db_ref + ".get_size() + 1";

	    if( el->tag.get_option( "autocase" ) == "true" )
	      cases = db_ref + ".get_cases()";
	  }
      }

    if( width.empty() )
      width = "20";

    if( cases.empty() )
      cases = "5";

    c.add( width );
    c.add( cases );

    c.add( "this" );
    c.add( "ID_" + get_name() );   

    std::string layout = el->tag.get_option( "layout" );

    if( !layout.empty() )
      {
	c.add( prepare_layout( layout ) );
      }
    
    return x2s( c );
  }
};


static bool walk_thru( Ref<GenConstructElement> el, std::vector<Ref<GenCode> > & table, std::string &error, std::vector<Ref<GenCode> > ct )
{
  std::vector<Ref<GenCode> > call_on_this;

  for( unsigned i = 0; i < table.size(); i++ )
    {
      if( table[i]->want_listen( el->tag.name ) )
	{
	  GenCode *gc = table[i]->clone();

	  if( gc )
	    {
	      if( ct.size() )
		{
		  GenStructs *s = dynamic_cast<GenStructs*>( gc );

		  if( !s )
		    {
		      error = "hey!";
		      return false;
		    }

		  s->parent = dynamic_cast<GenStructs*>( ct[ct.size()-1].address() );
		}

	      call_on_this.push_back( gc );
	    }
	  else
	    call_on_this.push_back( table[i] );
	}
    }

  for( unsigned i = 0; i < call_on_this.size(); i++ )
    {
      if( !call_on_this[i]->parse_begin( el ) )
	{
	  error = call_on_this[i]->error;
	  return false;
	}
    }


  for( unsigned j = 0; j < el->children.size(); j++ )
    {
      Ref<GenConstructElement> ref( dynamic_cast<GenConstructElement*>( el->children[j].address() ), false );

      for( unsigned i = 0; i < ct.size(); i++ )
	{
	  if( !ct[i]->parse_subchild( ref ) )
	    {
	      error = ct[i]->error;
	      return false;
	    }
	}

      for( unsigned i = 0; i < call_on_this.size(); i++ )
	{
	  if( !call_on_this[i]->parse_child( ref ) )
	    {
	      error = call_on_this[i]->error;
	      return false;
	    }
	  ct.push_back( call_on_this[i] );
	}      

      if( !walk_thru( ref, table, error, ct ) )
	{
	  return false;
	}
    }

  for( unsigned i = 0; i < call_on_this.size(); i++ )
    {
      if( !call_on_this[i]->parse_end( el ) )
	{
	  error = call_on_this[i]->error;
	  return false;
	}
    }

  return true;
}

static std::string gen_gui_code2(  Ref<GenConstructElement> el, MODE mode, const std::string &dir )
{
  std::vector<Ref<GenCode> > table;

  table.push_back( new GenWindow( mode, dir ) );
  table.push_back( new GenGroup(  mode ) );
  table.push_back( new GenHFrame( mode ) );
  table.push_back( new GenVFrame( mode ) );
  table.push_back( new GenLabel( mode ) );
  table.push_back( new GenMatrix( mode ) );
  table.push_back( new GenButton( mode ) );
  table.push_back( new GenEntry( mode ) );
  table.push_back( new GenDBEntry( mode ) );
  table.push_back( new GenCombo( mode ) );
  table.push_back( new GenText( mode ) );
  table.push_back( new GenHSep( mode ) );
  table.push_back( new GenVSep( mode ) );

  std::string error;

  std::vector<Ref<GenCode> > call_on_this;

  if( !walk_thru( el, table, error, call_on_this ) )
      return error;

  return std::string();
}

} // namespace

std::string gen_gui_code( Ref<GenConstructElement> el, const std::string &dir )
{
  std::string err;

  if( !( err = gen_gui_code2( el, MODE_DECLARE, dir ) ).empty() )
    return err;

  if( !( err = gen_gui_code2( el, MODE_DECLARE_ID, dir ) ).empty() )
    return err;

  if( !( err = gen_gui_code2( el, MODE_DECLARE_CALLBACK, dir ) ).empty() )
    return err;

  if( !( err = gen_gui_code2( el, MODE_DEFINE_CALLBACK, dir ) ).empty() )
    return err;

  if( !( err = gen_gui_code2( el, MODE_DEFINE_FROM_GUI, dir ) ).empty() )
    return err;

  if( !( err = gen_gui_code2( el, MODE_DEFINE_TO_GUI, dir ) ).empty() )
    return err;

  if( !( err = gen_gui_code2( el, MODE_DEFINE_AUTOLOAD_GUI, dir ) ).empty() )
    return err;

  if( !( err = gen_gui_code2( el, MODE_IMPLEMENT, dir ) ).empty() )
    return err;

  return std::string();
}
