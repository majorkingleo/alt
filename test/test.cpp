#include "dbi.h"
#include <iostream>
#include <fstream>
#include "format.h"
#include "xml.h"
#include "string_utils.h"
// #include "generator.h"

class User : public DBBindType
{
 public:
  DBTypeInt     id;
  DBTypeVarChar name;
  DBTypeVarChar email;

 public:
  User()
    : DBBindType( "users" ),
      id( this, "id" ),
      name( this, "name", 25 ),
      email( this, "email", 50 )
  {
  }
};

class Car : public DBBindType
{
 public:
  DBTypeInt     id;
  DBTypeInt     user;

 public:
  Car()
    : DBBindType( "car" ),
      id( this, "id" ),
      user( this, "user" )
  {
  }
};


using namespace std;

static bool test_single_select( Database &db, bool quiet )
{
  std::vector< Ref<User> > users;
    
 
  if( !StdSqlSelect( db, "users", users ) )
    {
      if( !quiet )
	cout << "select failed:\n" << db.get_sql() << "\n" << db.get_error() << endl;
      return false;
    }
  
  if( !quiet )
    cout << format("count: %d\n", users.size() );
  
  unsigned i;
  for( i = 0; i < users.size(); i++ )
    {
      if( !quiet )
	cout << format( "id: %d name: %s email: %s\n", users[i]->id(), users[i]->name(), users[i]->email() );
    }

  return i;
}

static bool test_single_inlist( Database &db, bool quiet )
{
  DBTypeInt car_id, user_id;
  
  
  if( !StdSqlSelect( db, "select car.id, users.id from car, users where users.id = car.user group by users.id", 
		     DBInList<DBType>() >> car_id >> user_id ) )
    {
      cout << "select failed:\n" << db.get_sql() << "\n" << db.get_error() << endl;
      return 2;
    }
  
  if( !quiet )
    cout << format( "car_id: %d user_id: %d\n", car_id(), user_id() );

  return true;
}    

static bool test_single_inlist_limit( Database &db, bool quiet )
{
  DBInLimit limit;
  DBTypeInt car_id, user_id;
  
  int count = 0;

  while( StdSqlSelect( db, "select car.id, users.id from car, users where users.id = car.user group by users.id", 
		       DBInList<DBType>() >> car_id >> user_id, limit ) > 0 )
    {
      if( !quiet )
	cout << format( "car_id: %d user_id: %d\n", car_id(), user_id() );
      count++;
    }

  return count == 2;
}

static bool test_single_bindtype_limit( Database &db, bool quiet )
{
  User user;
  Car car;
  
  DBInLimit limit;
  
  int count = 0;

  while( StdSqlSelect( db, "select car.id, users.id from car, users where users.id = car.user group by users.id", 
		       DBInList<DBBindType>() >> user >> car, limit ) > 0 )
    {
      count++;
      if( !quiet )
	cout << format( "car_id: %d user_id: %d\n", car.id(), user.id() );
    }

  return count == 2;
}

static bool test_single_bindtype_format( Database &db, bool quiet )
{
  User user;
  Car car;
  
  DBInLimit limit;
  
  int count = 0;

  while( StdSqlSelect( db, "select %car, %users from car, users where users.id = car.user group by users.id", 
		       DBInList<DBBindType>() >> user >> car, limit ) > 0 )
    {
      count++;
      if( !quiet )
	cout << format( "car_id: %d user_id: %d\n", car.id(), user.id() );
    }

  return count == 2;
}


static bool test_multi_bindtype_limit( Database &db, bool quiet )
{
  User a_user[20];
  Car a_car[20];
  
  int res;
  DBInLimit a_limit(20);
  
  unsigned count = 0;

  while( ( res = StdSqlSelect( db, "select car.id, users.id from car, users where users.id = car.user", 
			       DBInArrayList() >> a_user >> a_car, a_limit )  ) > 0 )
    {
      for( int i = 0; i < res; i++ )
	{
	  count++;
	  if( !quiet )
	    cout << format( "user_id: %d car_id: %s\n", a_user[i].id(), a_car[i].id() );
	}
    }   

  return count ==  144;
}

static bool test_multi_bindtype_exception( Database &db, bool quiet )
{
  User a_user[20];
  Car a_car[10];
  
  DBInLimit a_limit(20);
  
  try {

    StdSqlSelect( db, "select car.id, users.id from car, users where users.id = car.user", 
		  DBInArrayList() >> a_user >> a_car, a_limit );
    return false;

  } catch( BaseException &err ) {

    return true;

  }
  return false;
}

static bool test_multi_bindtype_format( Database &db, bool quiet )
{
  User a_user[20];
  Car a_car[20];
  
  int res;
  DBInLimit a_limit(20);
    
  int count = 0;

  while( ( res = StdSqlSelect( db, "select %car, %users from car, users where users.id = car.user", 
			       DBInArrayList() >> a_user >> a_car, a_limit )  ) > 0 )
    {
      for( int i = 0; i < res; i++ )
	{
	  count++;
	  if( !quiet )
	    cout << format( "user_id: %d car_id: %s\n", a_user[i].id(), a_car[i].id() );
	}
    }  

  return count == 144;
}

static bool test_xml_option( const std::string &s, const std::string &name, const std::string &value, bool quiet )
{
  XML::Item::Option o;
  
  XML::parse_option( s, o );
  
  if( o.name != name || o.value != value )
    {
      if( !quiet )
	{
	  cout << format( "s: '%s' => '%s' '%s' res: '%s' '%s'\n", s, name, value, o.name, o.value );
	}

	return false;
    }

  return true;
}

static bool test_xml_option( Database &db, bool quiet )
{
  if( !test_xml_option( "hello=\"world\"", "hello", "world", quiet ) )
    return false;

  if( !test_xml_option( "hello=\"world\" ", "hello", "world", quiet ) )
    return false;

  if( !test_xml_option( "hello=world", "hello", "world", quiet ) )
    return false;

  if( !test_xml_option( "hello=\"world world\"", "hello", "world world", quiet ) )
    return false;

  if( !test_xml_option( "hello", "hello", "", quiet ) )
    return false;

  if( !test_xml_option( "hello= ", "hello", "", quiet ) )
    return false;

  if( !test_xml_option( "hello=", "hello", "", quiet ) )
    return false;

  return true;
}

static bool test_s_split( const std::string &s, unsigned expected, bool quiet )
{
  std::vector<std::string> sl = split_safe( s );

  if( !quiet )
    {
      cout << "s: " << s << '\n';
      for( unsigned i = 0; i < sl.size(); i++ )
	cout << ">>" << sl[i] << "<<" << '\n';
    }

  if( sl.size() != expected )
    {
      return false;
    }

  return true;
}

static bool test_safe_split( Database &db, bool quiet )
{
  if( !test_s_split( "hello this is martin", 4, quiet ) )
    return false;

 if( !test_s_split( "hello \"this is\" martin", 3, quiet ) )
    return false;

 if( !test_s_split( "\"hello this\" is martin", 3, quiet ) )
    return false;

 if( !test_s_split( "\"hello \\\"this\" is martin", 3, quiet ) )
    return false;


 if( !test_s_split( "\"hello \\\"this\" is \"martin\"", 3, quiet ) )
    return false;

  return true;
}

static bool test_p_tag( const std::string &tag, const XML::Item & item, bool quiet )
{
  XML::Item i;

  if( !quiet )
    cout << "tag: " << tag << '\n';

  if( !parse_tag( tag, i ) )
    return false;

  if( !quiet )
    cout << "i: " << i << '\n' << "expected: " << item << '\n';

  if( item != i )
    return false;

  return true;
}

static bool test_parse_tag( Database &db, bool quiet )
{
  quiet = true;

  {
    XML::Item i;
    
    i.name = "body";
    
    if( !test_p_tag( "<body>", i, quiet ) )
      return false;
  }

  {
    XML::Item i;
    
    i.name = "body";
    i.beg_close = true;
    
    if( !test_p_tag( "</body>", i, quiet ) )
      return false;
  }

  {
    XML::Item i;
    
    i.name = "body";
    i.end_close = true;
    
    if( !test_p_tag( "<body/>", i, quiet ) )
      return false;
  }


  {
    XML::Item i;
    
    i.name = "img";
    i.end_close = true;
    i.options.push_back( XML::Item::Option( "src", "foo.gif" ) );

    if( !test_p_tag( "<img src=\"foo.gif\"/>", i, quiet ) )
      return false;
  }
 

 {
    XML::Item i;
    
    i.name = "img";
    i.end_close = true;
    i.options.push_back( XML::Item::Option( "src", "foo.gif" ) );
    i.options.push_back( XML::Item::Option( "notag", "" ) );

    if( !test_p_tag( "<img src=\"foo.gif\" notag/>", i, quiet ) )
      return false;
  }

  return true;
}

static bool test_parse( Database &db, bool quiet )
{
  std::string s;
  
  if( !XML::read_file( "foo.xml", s ) )
    {
      if( !quiet )
	cout << "cannot read file foo.xml\n";
      return false;
    }

  XML::Item item;

  if( !XML::parse( s, item ) )
    return false;

  return true;
}
/*
static bool test_db_generator( Database &db, bool quiet )
{
  std::string s;
  
  if( !XML::read_file( "database.xml", s ) )
    {
      if( !quiet )
	cout << "cannot read file foo.xml\n";
      return false;
    }
  
  XML::Item item;

  if( !XML::parse( s, item ) )
    return false;

  if( !create_db_code( Ref<XML::Item>(&item, false ), "test" ) )
    return false;

  return true;
}
*/

#define TEST( name, what )					\
  {								\
    cout << name;						\
    for( unsigned i = std::string(name).size(); i < 50; i++ )	\
      cout << ' ';						\
    if( what ( db, true ) )					\
      cout << "OK" << endl;					\
    else							\
      {								\
	cout << "FAILED" << endl;				\
	what ( db, false );					\
      }								\
  }

int main()
{
  Database db( "beast", "root", "", "auto" );

  if( !db )
    {
      cout << "cannot open db\n";
      return 1;
    }

  /*
  TEST( "single select", test_single_select );
  TEST( "single inlist", test_single_inlist );
  TEST( "single inlist limit", test_single_inlist_limit );
  TEST( "single bind type with limit", test_single_bindtype_limit );
  TEST( "single bind type with format", test_single_bindtype_format );
  TEST( "multi bind type with limit", test_multi_bindtype_limit );
  TEST( "multi bind type with %", test_multi_bindtype_format );
  TEST( "multi bind type with exception", test_multi_bindtype_exception );
  */

  TEST( "xml option", test_xml_option );
  TEST( "split_safe", test_safe_split );
  TEST( "xml tag", test_parse_tag );
  TEST( "xml file", test_parse );
//  TEST( "DB generator", test_db_generator );
  
  return 0;
}
