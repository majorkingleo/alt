#include "elements2.h"
#include "generator.h"
#include "cppdir.h"
#include <iostream>
#include <sstream>

namespace {

enum MODE
  {
    MODE_DECLARE,
    MODE_PRIMARY,
    MODE_FOREIGN_KEY
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
    file_name = el->tag.get_option( "file_sql" );

    if( mode == MODE_DECLARE )
      {
	return true;
      }
    else if( mode == MODE_PRIMARY )
      return true;

    else if( mode == MODE_FOREIGN_KEY )
      return true;

    error = "unknown mode " __FILE__ ":" + x2s( __LINE__ );
    return false;
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
  std::string type;
  bool first;

public:
  GenTable( std::ostream &out, MODE mode )
    : GenStructs( out, mode )
  {
    first = true;
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
    type = el->tag.get_option( "type" );

    if( mode == MODE_DECLARE )
      {
	out << "create table `" << name << "` (\n";
      }
    else if( mode == MODE_PRIMARY || mode == MODE_FOREIGN_KEY )
      {
	// out << "ALTER TABLE `" << name << "` " ;
	return true;
      }
    else
      {
	error = "unknown mode " __FILE__ ":" + x2s( __LINE__ );
	return false;
      }
    
    return true;
  }

  bool parse_end( Ref<GenConstructElement> el )
  {
    if( mode == MODE_DECLARE )
      {
	out << "\n) TYPE = " << type << ";\n\n";
      }
    else if( mode == MODE_PRIMARY )
      {
	out << "\n";
	return true;
      }
    else if( mode == MODE_FOREIGN_KEY )
      return true;
    else
      {
	error = "unknown mode  " __FILE__ ":" + x2s( __LINE__ );
	return false;
      }

    return true;
  }

  bool parse_child( Ref<GenConstructElement> el )
  {
    if( first )
      first = false;
    else if( mode == MODE_DECLARE )
      out << ",\n";

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
  std::string length;
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
    length = el->tag.get_option( "length" );    

    std::stringstream s;

    if( mode == MODE_DECLARE || mode == MODE_PRIMARY )
      {
	s << "`" << name << "` ";

	if( icase_cmp( type, "varchar" ) )
	  s << "varchar(" << length << ") ";
	else if( icase_cmp( type, "text" ) )
	  s << "text ";
	else if( icase_cmp( type, "int" ) )
	  s << "int ";
	else if( icase_cmp( type, "enum" ) )
	  {
	    s << "enum( ";

	    std::string val = el->tag.get_option( "values" );
	    
	    values = split_simple( val, "," );
	    
	    for( unsigned i = 0; i < values.size(); i++ )
	      {
		if( i > 0 )
		  s << ",";

		s << "'" << values[i] << "'";
	      }	    

	    s << ") ";
	  }

	if( !icase_cmp( el->tag.get_option( "null" ), "true" ) )
	  {
	    s<< "NOT NULL ";
	  }

	std::string def = el->tag.get_option( "default" );

	if( !def.empty() )
	  {
	    s << "default '" << def << "' ";
	  }

	if( mode == MODE_DECLARE )
	  {	    
	    out << s.str();
	  }       	 
	else
	  {
	    if( icase_cmp( el->tag.get_option( "autoincrement" ), "true" ) )
	      {
		s << "auto_increment ";
	      }

	    if( icase_cmp( el->tag.get_option( "unique" ), "true" ) )
	      {
		s << "unique ";
	      }
	  }
      }   
   
    if( mode == MODE_PRIMARY )
      {
	if( icase_cmp( el->tag.get_option( "primary" ), "true" ) )
	  {
	    out << "ALTER TABLE `";

	    GenConstructElement *e = dynamic_cast< GenConstructElement *>(el->parent);

	    if( !e )
	      {
		error = "null pointer!";
		return false;
	      }

	    out << e->tag.get_option( "name" ) << "` ";

	    out << "ADD PRIMARY KEY ( `" << name << "` );\n";
	  }	  

    	if( icase_cmp( el->tag.get_option( "autoincrement" ), "true" ) )
	  {
	    out << "ALTER TABLE `";

	    GenConstructElement *e = dynamic_cast< GenConstructElement *>(el->parent);

	    if( !e )
	      {
		error = "null pointer!";
		return false;
	      }

	    out << e->tag.get_option( "name" ) << "` ";

	    out << "CHANGE `" << name << "` " << s.str() << ";\n";
	  }

    	if( icase_cmp( el->tag.get_option( "unique" ), "true" ) )
	  {
	    out << "ALTER TABLE `";

	    GenConstructElement *e = dynamic_cast< GenConstructElement *>(el->parent);

	    if( !e )
	      {
		error = "null pointer!";
		return false;
	      }

	    out << e->tag.get_option( "name" ) << "` ";

	    out << "CHANGE `" << name << "` " << s.str() << ";\n";
	  }
       
	std::string fk;
    	if( !( fk = el->tag.get_option( "foreign_key" ) ).empty() )
	  {
	    out << "ALTER TABLE `";

	    GenConstructElement *e = dynamic_cast< GenConstructElement *>(el->parent);

	    if( !e )
	      {
		error = "null pointer!";
		return false;
	      }

	    out << e->tag.get_option( "name" ) << "` ";
	    // ALTER TABLE `NOTES_LINES` ADD INDEX `INDEX_HEAD_ID` ( `head_id` ) 
	    out << "ADD INDEX ( `" << name << "`);\n";
	  }
      }
    else if( mode == MODE_DECLARE )
      return true;
    else if( mode == MODE_FOREIGN_KEY )
      {
	std::string fk;
    	if( !( fk = el->tag.get_option( "foreign_key" ) ).empty() )
	  {
	    out << "ALTER TABLE `";

	    GenConstructElement *e = dynamic_cast< GenConstructElement *>(el->parent);

	    if( !e )
	      {
		error = "null pointer!";
		return false;
	      }

	    out << e->tag.get_option( "name" ) << "` ";
	    //  alter table `NOTES_LINES` add FOREIGN KEY `FK_HEAD_ID` ( head_id )
	    //  REFERENCES `NOTES_HEADS` ( id ) ON DELETE RESTRICT
	    out << "ADD FOREIGN KEY ( `" << name << "`) ";
	    out << "REFERENCES `";

	    std::vector<std::string> sl = split_simple( fk, ".", 2 );

	    out << sl[0] << "` ( `" << sl[1] << "` ) ON DELETE RESTRICT;\n";
	  }
      }
    else
      {
	error = "unknown mode  " __FILE__ ":" + x2s( __LINE__ );
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

static std::string gen_sql_code2(  Ref<GenConstructElement> el, MODE mode, const std::string &dir,
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
  
  if( !update_file2( out, CppDir::concat_dir( dir, db.get_file_name() ), false ) )
    return "updating/creating file failed";

  return std::string();
}

} // namespace

std::string gen_sql_code( Ref<GenConstructElement> el, const std::string &dir )
{
  std::string err;

  std::stringstream out;

  if( !( err = gen_sql_code2( el, MODE_DECLARE, dir, out ) ).empty() )
    return err;

  if( !( err = gen_sql_code2( el, MODE_PRIMARY, dir, out ) ).empty() )
    return err;

  if( !( err = gen_sql_code2( el, MODE_FOREIGN_KEY, dir, out ) ).empty() )
    return err;

  /*

  std::stringstream out2;

  if( !( err = gen_struct_code2( el, MODE_DEFINE, dir, out2 ) ).empty() )
    return err;

  if( !( err = gen_struct_code2( el, MODE_IMPLEMENT, dir, out2 ) ).empty() )
    return err;
  */

  return std::string();
}
