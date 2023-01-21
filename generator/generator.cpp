#include "cppdir.h"
#include "string_utils.h"
#include "generator.h"
#include <iostream>
#include <fstream>
#include <sstream>

using namespace std;

Ref<XML::Item> find_item( Ref<XML::Item> item, const std::string & name )
{
  if( icase_cmp( item->name, name ) )
    return item;

  for( unsigned i = 0; i < item->subitems.size(); i++ )
    {
      Ref<XML::Item> it = find_item( item->subitems[i], name );
      if( it.valid() )
	return it;
    }

  return Ref<XML::Item>();
}

static bool update_files();
static bool include_file( Ref<XML::Item> items, const std::string & file );

static bool find_include( Ref<XML::Item> item )
{
  if( icase_cmp( item->name, "include" ) )
    {
      std::string s = item->get_option( "file" );
      if( s.empty() )
	{
	  cerr << "parse error! mission file=foo in include tag" << endl;
	  return false;
	}
      
      if( !include_file( item, s ) )
	return false;
    }

  for( int i = 0; i < item->subitems.size(); i++ )
    {
      if( !find_include( item->subitems[i] ) )
	return false; 
    }
  
  return true;
}

static bool include_file( Ref<XML::Item> items, const std::string & file )
{
  std::string s;

  if( !XML::read_file( file, s ) )
    {
      cerr << "cannot open file: " << s << '\n';
      return false;
    }
  
  if( !XML::parse( s, *items ) )
    {
      cerr << "parse error!\n" << endl;
      return false;
    }
  
  return true;
}

static bool strip_includes( Ref<XML::Item> item )
{
  if( icase_cmp( item->name, "include" ) )
    {
      if( item->parent )
	{
	  for( int i = 0; i < item->subitems.size(); i++ )
	    {
	      item->parent->subitems.push_back( item->subitems[i] );
	      item->subitems[i]->parent = item->parent;	      
	    }
	}
      item->subitems.clear();
    }
  else
    {
        for( int i = 0; i < item->subitems.size(); i++ )
	    {
	      
	    }
    }
}

int main( int argc, char **argv )
{
  if( argc != 3 )
    {
      cerr << "usage: " << argv[0] << " project.xml outdir\n";
      return 1;
    }

  std::string s;
  
  if( !XML::read_file( argv[1], s ) )
    {
      cerr << "cannot open file: " << argv[1] << '\n';
      return 2;
    }

  XML::Item item;

  if( !XML::parse( s, item ) )
    {
      cerr << "parse error!\n" << endl;
      return 3;
    }

  // if( !find_include( Ref<XML::Item>( &item, false ) ) )
  //  return 5;

  std::string err;
  std::vector<Ref<GEN::GenDefineElement> > defines;

  if( !( err = parse_defines( Ref<XML::Item>( &item, false ),  defines ) ).empty() )
    {
      cerr << err << '\n';
      cerr << "parsing defines failed\n";
      return false;
    }

  Ref<GEN::GenConstructElement> el;

    if( !( err = contruct_tree( Ref<XML::Item>( &item, false ),  defines, el ) ).empty() )
    {
      cerr << err << '\n';
      cerr << "parsing defines failed\n";
      return false;
    }
  
    if( !( err = gen_struct_code( el, argv[2] ) ).empty() )
      {
	cerr << err << '\n';
	cerr << "generating code failed\n";
	return false;
      }

    update_files();

    if( !( err = gen_sql_code( el, argv[2] ) ).empty() )
      {
	cerr << err << '\n';
	cerr << "generating code failed\n";
	return false;
      }

    update_files();

    if( !( err = gen_gui_code( el, argv[2] ) ).empty() )
      {
	cerr << err << '\n';
	cerr << "generating code failed\n";
	return false;
      }

    update_files();

  /*
  if( !create_db_code( Ref<XML::Item>( &item, false ), argv[2] ) )
    {
      cerr << "cannot generate db code\n";
      return 4;
    }

  if( !create_db_sql( Ref<XML::Item>( &item, false ), argv[2] ) )
    {
      cerr << "cannot generate sql code\n";
      return 4;
    }

  if( !create_gui_code( Ref<XML::Item>( &item, false ), argv[2] ) )
    {
      cerr << "cannot generate gui code\n";
      return 4;
    }
  */

  return 0;
}

struct MEMFILE
{
  std::string name;
  Ref<std::stringstream> s;
};

std::vector<MEMFILE> files;

bool update_file2( std::stringstream &out, const std::string &file_name, bool append )
{
  for( unsigned i = 0; i < files.size(); i++ )
    {
      if( files[i].name == file_name )
	{
	  if( !append )
	    {
	      files[i].s = new std::stringstream();
	      *files[i].s << out.str();
	      return true;
	    }
	  else
	    {
	      *files[i].s << out.str();
	      return true;
	    }
	}
    }

  MEMFILE f;

  f.name = file_name;
  f.s = new std::stringstream();
  *f.s << out.str();

  files.push_back( f );

  return true;
}

static bool update_files()
{
  for( unsigned i = 0; i < files.size(); i++ )
    {
      if( !update_file( *files[i].s, files[i].name ) )
	return false;
    }
  
  files.clear();

  return true;
}

static bool write_file( std::stringstream &out, const std::string &file_name, bool append = false )
{
  cout << "updating file " << file_name << '\n';

  std::ofstream o( file_name.c_str(), append ? std::ios_base::app : std::ios_base::trunc );

  if( !o )
    return false;

  o << out.str();

  return true;
}

bool update_file( std::stringstream &out, const std::string &file_name, bool append )
{
  if( append )
    return write_file( out, file_name, append );

  std::ifstream in( file_name.c_str() );

  if( !in )
    {
      if( !write_file( out, file_name ) )
	{
	  cerr << "cannot open file " << file_name << '\n';
	  return false;
	}
      return true;
    }

  std::string s;

  while( !in.eof() )
    {
      std::string ss;
      getline( in, ss );
      s += ss + '\n';
    }

  if( strip( s ) != strip( out.str() ) )
    {
      if( !write_file( out, file_name ) )
	{
	  cerr << "cannot open file " << file_name << '\n';
	  return false;
	}

      return true;
    }

  // nothing to update
  return true;
}
