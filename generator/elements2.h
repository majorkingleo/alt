#ifndef GENERATOR_ELEMENTS2_H
#define GENERATOR_ELEMENTS2_H

#include "xml.h"
#include "string_utils.h"

using namespace Tools;

namespace GEN {

class GenElement2
{
public:
  GenElement2 *parent;
  std::vector< Ref<GenElement2> > children;
  std::string error;

  GenElement2() : parent( 0 ) {};
  virtual  ~GenElement2() {}

  virtual bool parse( Ref<XML::Item> item ) = 0;
  virtual GenElement2* clone() = 0;
};

class GenDefOptionElement : public GenElement2
{
  std::string name;
  std::string type;
  std::string values;
  std::string def_value;
  std::string value;
  bool required;
  bool add;

public:
  GenDefOptionElement()
    : GenElement2()
  {
  }

  bool parse( Ref<XML::Item> item );
  GenDefOptionElement* clone() { return new GenDefOptionElement( *this ); }

  bool check( Ref<XML::Item> item );

  XML::Item::Option get_option();

  bool add_it() { return add; }
};

class GenDefParentElement : public GenElement2
{
  std::string parent_name;
  bool strict;

public:
  GenDefParentElement()
    : GenElement2()
  {
  }

  bool parse( Ref<XML::Item> item );

  GenDefParentElement* clone() { return new GenDefParentElement( *this ); }
};


class GenDefChildElement : public GenElement2
{
  bool children;

public:
  GenDefChildElement()
    : GenElement2()
  {
  }

  bool parse( Ref<XML::Item> item );

  GenDefChildElement* clone() { return new GenDefChildElement( *this ); }
};

class GenDefImplementElement : public GenElement2
{
  Ref<XML::Item> item;

public:
  GenDefImplementElement()
    : GenElement2()
  {
  }

  bool parse( Ref<XML::Item> item );

  void append( Ref<XML::Item> item );

  GenDefImplementElement* clone();
};

class GenDefineElement : public GenElement2
{
  std::string name;
  std::string parent_name;
  bool is_declare;

public:
  GenDefineElement( bool is_declare = false )
    : GenElement2(),
      is_declare( is_declare )
  {}

  bool parse( Ref<XML::Item> item );

  const std::string & get_parent_name() const { return parent_name; }
  const std::string & get_name() const { return name; }
  bool declare() const { return is_declare; }

  GenDefineElement* clone();
};

class GenConstructElement : public GenElement2
{
  Ref<GenDefineElement> define;

 public:
  
  struct Tag
  {
    std::string name;
    std::vector<XML::Item::Option> options;

    std::vector<std::string> protect;

    std::string get_option( const std::string & name_ )
    {
      for( unsigned i = 0; i < options.size(); i++ )
	if( icase_cmp( options[i].name, name_ ) )
	    return options[i].value;

      return std::string();
    }    

    void set_option( const XML::Item::Option &o )
    {
      set_option( o.name, o.value );
    }

    void set_option( const std::string &name_, const std::string &value )
    {
      for( unsigned k = 0; k < protect.size(); k++ )
	{
	  if( icase_cmp( protect[k], name_ ) )
	    {
	      add_option( name_, value );
	      return;
	    }
	}


      for( unsigned i = 0; i < options.size(); i++ )
	if( icase_cmp( options[i].name, name_ ) )
	  {
	    options[i].value = value;
	    return;
	  }

      options.push_back( XML::Item::Option( name_, value ) );
    }

    void add_option( const std::string &name_, const std::string &value )
    {
      add_option( XML::Item::Option( name_, value ) );
    }

    void add_option( const XML::Item::Option &o )
    {
      for( unsigned i = 0; i < options.size(); i++ )
	if( icase_cmp( options[i].name, o.name ) )
	  {
	    std::string sep;

	    if( !o.value.empty() && !options[i].value.empty() )
	      sep = ',';

	    options[i].value += sep + o.value;
	    protect.push_back( o.name );
	    return;
	  }
      options.push_back( o );

      protect.push_back( o.name );
    }
  };

  Tag tag;

public:
  GenConstructElement()
    : GenElement2()
  {
  }

  bool parse( Ref<XML::Item> item ) { return true; }
  
  bool construct( Ref<XML::Item> item, std::vector<Ref<GEN::GenDefineElement> > & defines );
  bool implement( GenConstructElement *p = 0 ); 

  GenConstructElement* clone();
};

class GenCode
{
 public:
  std::string error;

  GenCode(){}

  virtual ~GenCode(){}

  virtual bool want_listen( const std::string &name ) { return false; }
  virtual bool parse_begin( Ref<GenConstructElement> el ) = 0;
  virtual bool parse_end( Ref<GenConstructElement> el ) { return true; }
  virtual bool parse_child( Ref<GenConstructElement> el ){ return true; }
  virtual bool parse_subchild( Ref<GenConstructElement> el ) { return true; }

  virtual GenCode* clone() = 0;
};

}

#endif
