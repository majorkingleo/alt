#include "elements2.h"
#include "generator.h"
#include "cppdir.h"
#include <iostream>
#include <sstream>

namespace {

enum MODE
  {
    MODE_DECLARE,
    MODE_DEFINE,
    MODE_IMPLEMENT
  };

using namespace GEN;
using namespace std;

class GenStructs : public GenCode
{
protected:
  std::ostream &out;
  MODE mode;

public:
  GenStructs( std::ostream &out, MODE mode ) : GenCode(), out( out ), mode( mode ) {}

  
};

class GenDatabase : public GenStructs
{
  std::string file_name;

public:

  GenDatabase( std::ostream & out, MODE mode ) : GenStructs( out, mode ) {}

  bool want_listen( const std::string &name )
  {
    if( icase_cmp( "database", name ) )
      return true;

    return false;
  }

  bool parse_begin( Ref<GenConstructElement> el )
  {
    if( mode == MODE_DECLARE )
      return parse_declare_begin( el );
    else if( mode == MODE_DEFINE  )
      return parse_define_begin( el );
    else if( mode == MODE_IMPLEMENT )
      {    
	file_name = el->tag.get_option( "file_cpp" );
	return true;
      }

    error = "unknown mode";
    return false;
  }

  bool parse_define_begin( Ref<GenConstructElement> el )
  {
    file_name = el->tag.get_option( "file_cpp" );
    std::string f_name = el->tag.get_option( "file_h" );
   
    out << "#include \"" << f_name << "\"\n\n";

    return true;
  }

  bool parse_define_end( Ref<GenConstructElement> el )
  {
    out << "\n";

    return true;
  }

  bool parse_declare_begin( Ref<GenConstructElement> el )
  {
    std::string f_name;

    f_name = el->tag.get_option( "file_h" );
     
    file_name = f_name;

    f_name = substitude( f_name, ".", "_" );
    f_name = toupper( f_name );

    out << 
      "#ifndef GEN_CODE_" + f_name + "\n"
      "#define GEN_CODE_" + f_name + "\n"
      "\n"
      "#include \"dbi.h\"\n"
      "#include \"range.h\"\n\n";

    return true;    
  }

  bool parse_end( Ref<GenConstructElement> el )
  {
    if( mode == MODE_DECLARE )
      return parse_declare_end( el );    
    else if( mode == MODE_DEFINE )
      return parse_define_end( el );
    else if( mode == MODE_IMPLEMENT )
      return true;

    error = "unknown mode";
    return false;
  }

  bool parse_declare_end( Ref<GenConstructElement> el )
  {
    out << 
      "\n"
      "#endif\n";

    return true;
  }

  std::string get_file_name() { return file_name; }
 
  GenDatabase* clone()
  {
    return 0;
  } 
};

class GenTable : public GenStructs
{
  std::string name;

public:
  GenTable( std::ostream &out, MODE mode )
    : GenStructs( out, mode )
  {
  }
  
  bool want_listen( const std::string &name )
  {
    if( icase_cmp( name, "table" ) )
      return true;

    return false;
  }

  bool parse_begin( Ref<GenConstructElement> el )
  {
    name = el->tag.get_option( "name" );

    if( mode == MODE_DECLARE )
      {
	out << "class " << name << " : public Tools::DBBindType\n{\n";
	out << "  public:\n";
      }
    else if( mode == MODE_DEFINE )
      {
	out << name << "::" << name << "()\n"
	    << " : Tools::DBBindType( \"" << name << "\" )";
      }
    else if( mode == MODE_IMPLEMENT )
      return true;
    else
      {
	error = "unknown mode";
	return false;
      }
    
    return true;
  }

  bool parse_end( Ref<GenConstructElement> el )
  {
    if( mode == MODE_DECLARE )
      {
	out << "  public:\n" 
	    << "    " << name << "();\n";
	
	out << "\n};\n\n";
      }
    else if( mode == MODE_DEFINE )
      {
	out << "\n{}\n\n";
      }
    else if( mode == MODE_IMPLEMENT )
      return true;
    else
      {
	error = "unknown mode";
	return false;
      }

    return true;
  }

  GenTable* clone()
  {
    return new GenTable( *this );
  } 
};

class GenRow : public GenStructs
{
  std::string name;
  std::string type;
  std::vector<std::string> values;

public:
  GenRow( std::ostream &out, MODE mode )
    : GenStructs( out, mode )
  {

  }

  bool want_listen( const std::string &name )
  {
    if( icase_cmp( name, "row" ) )
      return true;

    return false;
  }

  bool parse_begin( Ref<GenConstructElement> el )
  {
    name = el->tag.get_option( "name" );
    type = el->tag.get_option( "type" );    

    if( mode == MODE_DECLARE )
      {
	if( icase_cmp( type, "varchar" ) || icase_cmp( type, "text" ) )      
	  out << "Tools::DBTypeVarChar \t" << name << ";\n" ;      
	else if( icase_cmp( type, "int" ) )
	  out << "Tools::DBTypeInt \t" << name << ";\n" ;
	else if( icase_cmp( type, "enum" ) )
	  {
	    static unsigned count = 0;
	    count++;
	    
	    out << "struct ESTRUCT_" << count << "\n" << "{\n";
	    out << "  enum ETYPE {\n";
	    out << "     FIRST__ = -1,\n";
	    
	    std::string val = el->tag.get_option( "values" );
	    
	    values = split_simple( val, "," );
	    
	    for( unsigned i = 0; i < values.size(); i++ )
	      {
		out << "     " << values[i] << ",\n";
	      }
	    
	    out << "     LAST__\n";
	    out << "  };\n";
	    out << "  static const char* STYPES[];\n";
	    out << "};\n";
	    out << "Tools::DBTypeEnum< Tools::EnumRange<ESTRUCT_" << count << "> > \t" << name << ";\n";
	  }
	else
	  {
	    error = "unknown type: " + type;
	    return false;
	  }
      }
    else if( mode == MODE_DEFINE ) 
      {
	out << ",\n";

	if( icase_cmp( type, "varchar" ) || icase_cmp( type, "text" ) )      
	  out << name << "( this, \"" << name << "\", " << el->tag.get_option( "length" ) << ")" ;      
	else if( icase_cmp( type, "int" ) )
	  out << name << "( this, \"" << name << "\")";      
	else if( icase_cmp( type, "enum" ) )
	  {
	    out << name << "( this, \"" << name << "\"";

	    std::string def_type = el->tag.get_option( "default" );

	    if( !def_type.empty() )
	      {
		out << ", " << name << ".data." << def_type;
	      }

	    out << " )";
	  }
	else
	  {
	    error = "unknown type: " + type;
	    return false;
	  }
      }
    else if( mode == MODE_IMPLEMENT ) 
      {
	if( icase_cmp( type, "enum" ) )
	  {
	    static unsigned count = 0;
	    count++;
	    
	    GenConstructElement *e = dynamic_cast<GenConstructElement*>(el->parent);

	    std::string parent_name = e ? e->tag.get_option( "name" ) : "NULL";

	    out << "const char* " << parent_name << "::ESTRUCT_" << count << "::STYPES[] = {\n";
	    
	    std::string val = el->tag.get_option( "values" );
	    
	    values = split_simple( val, "," );
	    
	    for( unsigned i = 0; i < values.size(); i++ )
	      {
		out << "     \"" << values[i] << "\",\n";
	      }
	    out << "};\n";
	    return true;
	  }
	return true;
      }
    else
      {
	error = "unknown mode";
	return false;
      }

    return true;
  }

  GenRow* clone()
  {
    return new GenRow( *this );
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
	    call_on_this.push_back( gc );
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

static std::string gen_struct_code2(  Ref<GenConstructElement> el, MODE mode, const std::string &dir,
				        std::stringstream & out )
{
  std::vector<Ref<GenCode> > table;

  GenDatabase db( out, mode );

  table.push_back( Ref<GenCode>( &db, false ) );
  table.push_back( new GenTable( out, mode ) );
  table.push_back( new GenRow( out, mode ) );

  std::string error;

  std::vector<Ref<GenCode> > call_on_this;

  if( !walk_thru( el, table, error, call_on_this ) )
      return error;
  
  if( mode != MODE_DEFINE )
    if( !update_file( out, CppDir::concat_dir( dir, db.get_file_name() ) ) )
      return "updating/creating file failed";

  return std::string();
}

} // namespace

std::string gen_struct_code( Ref<GenConstructElement> el, const std::string &dir )
{
  std::string err;

  std::stringstream out;

  if( !( err = gen_struct_code2( el, MODE_DECLARE, dir, out ) ).empty() )
    return err;

  std::stringstream out2;

  if( !( err = gen_struct_code2( el, MODE_DEFINE, dir, out2 ) ).empty() )
    return err;

  if( !( err = gen_struct_code2( el, MODE_IMPLEMENT, dir, out2 ) ).empty() )
    return err;

  return std::string();
}
