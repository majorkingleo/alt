#include "elements2.h"
#include "generator.h"

using namespace GEN;
using namespace Tools;

bool GEN::GenDefOptionElement::parse( Ref<XML::Item> item )
{
  required = false;

  for( unsigned i = 0; i < item->options.size(); i++ )
    {
      XML::Item::Option &o = item->options[i];
      
      if( icase_cmp( o.name, "name" ) )
	name = o.value;
      else if( icase_cmp( o.name, "type" ) )
	type = o.value;
      else if( icase_cmp( o.name, "values" ) )
	values = o.value;
      else if( icase_cmp( o.name, "required" ) )
	{
	  if( !o.value.empty() )
	    required = s2x<bool>( o.value, true );
	  else
	    required = true;
	}
      else if( icase_cmp( o.name, "default" ) )
	def_value = o.value;
      else if( icase_cmp( o.name, "add" ) )
	  add = true;
      else
	{
	  error = "unknwon property: " + o.name;
	  return false;
	}	
    }
  
  if( name.empty() )
    {
      error = "expectin name";
      return false;
    }

  if( type.empty() )
    type = "string";
  
  if( icase_cmp( type, "enum" ) && values.empty() )
    {
      error = "missing values";
      return false;
    }    
  
  if( !icase_cmp( type, "string" ) &&
      !icase_cmp( type, "enum" ) &&
      !icase_cmp( type, "bool" ) &&
      !icase_cmp( type, "int" ) )
    {
      error = "unknown option type";
      return false;
    }    

  return true;
}

bool GEN::GenDefOptionElement::check( Ref<XML::Item> item )
{
  if( required && !item->is_set( name ) )
    {
      error = "missing reqired option " + name;
      return false;
    }

  for( unsigned i = 0; i < 2; i++ )
    {
      std::string n;

      if( i == 0 )
	n = item->get_option( name );
      else
	{
	  n = def_value;
	  if( n.empty() )
	    break;
	}
      

      if( icase_cmp( type, "bool" ) )
	{
	  if( !n.empty() &&
	      !icase_cmp( n, "true" ) &&
	      !icase_cmp( n, "false" ) &&
	      n != "0" &&
	      n != "1" )
	    {
	      error = "not a bollean value: " + n + " at option " + name;
	      return false;
	    }
	}
      else if( icase_cmp( type, "int" ) )
	{
	  if( !n.empty() && !is_int( n ) )
	    {
	      error = "not an integer value: " + n;
	      return false;
	    }
	}
      else if( icase_cmp( type, "enum" ) )
	{
	  std::vector<std::string> sl = split_simple( values, "," );
	  
	  bool found = false;
	  
	  for( unsigned i = 0; i < sl.size(); i++ )
	    {
	      if( icase_cmp( sl[i], n ) )
		{
		  found = true;
		  break;
		}
	    }
	  
	  if( !found )
	    {
	      error = "option not in enum range: " + n;
	      return false;
	    }
	}      
    }

  if( item->is_set( name ) )
    {
      std::string n = item->get_option( name );

      if( n.empty() && icase_cmp( type, "bool" ) )
	n = "true";
      else if( n.empty() )
	n = def_value;

      value = n;
    }
  else if( !def_value.empty() )
    {
      value = def_value;
    }

  return true;
}


XML::Item::Option GEN::GenDefOptionElement::get_option()
{
  XML::Item::Option o;

  o.name = name;
  o.value = value;

  return o;
}


bool GEN::GenDefParentElement::parse( Ref<XML::Item> item )
{
  strict = false;

  for( unsigned i = 0; i < item->options.size(); i++ )
    {
      XML::Item::Option &o = item->options[i];
      
      if( icase_cmp( o.name, "name" ) )
	parent_name = o.value;
      else if( icase_cmp( o.name, "strict" ) )
	strict = s2x<bool>( o.value, true );
      else
	{
	  error = "unknwon property: " + o.name;
	  return false;
	}	

      if( parent_name.empty() )
	{
	  error = "missing parent name";
	  return false;
	}     
    }
  return true;
}


bool GEN::GenDefChildElement::parse( Ref<XML::Item> item )
{
  children = false;

  for( unsigned i = 0; i < item->options.size(); i++ )
    {
      XML::Item::Option &o = item->options[i];
      
      if( icase_cmp( o.name, "none" ) )
	children = s2x<bool>( o.value, false );
      else
	{
	  error = "unknwon property: " + o.name;
	  return false;
	}	
    }

  return true;
}

bool GEN::GenDefImplementElement::parse( Ref<XML::Item> item_ )
{
  if( item_->options.size() )
    {
      error = "no options required for this tag";
      return false;
    }

  item = item_;

  return true;
}

GEN::GenDefImplementElement* GEN::GenDefImplementElement::clone()
{
  return new GenDefImplementElement( *this );
}

void GEN::GenDefImplementElement::append( Ref<XML::Item> i )
{
  for( unsigned j = 0; j < item->subitems.size(); j++ )
    {
      i->subitems.push_back( item->subitems[j]->clone() );
    }
}

GEN::GenDefineElement* GEN::GenDefineElement::clone()
{
  GenDefineElement *el = new GenDefineElement();

  el->name = name;
  el->parent_name = name;
  el->is_declare = is_declare;

  for( unsigned i = 0; i < children.size(); i++ )
    {
      el->children.push_back( children[i]->clone() );
    }

  return el;
}

bool GEN::GenDefineElement::parse( Ref<Tools::XML::Item> item )
{
  for( unsigned i = 0; i < item->options.size(); i++ )
    {
      XML::Item::Option &o = item->options[i];
      
      if( icase_cmp( o.name, "name" ) )
	name = o.value;
      else if( icase_cmp( o.name, "parent" ) )
	parent_name = o.value;
      else
	{
	  error = "unknwon property: " + o.name;
	  return false;
	}      
    }

  if( name.empty() )
    {
      error = "missing name property";
      return false;
    }

  for( unsigned i = 0; i < item->subitems.size(); i++ )
    {
      Tools::Ref<Tools::XML::Item> it( item->subitems[i] );
      Ref<GenElement2> el;
      
      if( icase_cmp( it->name, "option" ) )
	el = new GenDefOptionElement();
      else if( icase_cmp( it->name, "parent" ) )
	el = new GenDefParentElement();
      else if( icase_cmp( it->name, "children" ) )
	el = new GenDefChildElement();
      else if( is_declare && icase_cmp( it->name, "implement" ) )
	el = new GenDefImplementElement();
            
      if( !el.valid() )
	{
	  error = x2s( it->line ) + ": unknown item: " + it->name;
	  return false;
	}
      
      if( !el->parse( it ) )
	{
	  error =  x2s( it->line ) + ":" + el->error + '\n' + x2s( *it );
	  return false;
	}

      children.push_back( el );
    }

  return true;
}


bool GEN::GenConstructElement::construct( Ref<XML::Item> item, std::vector<Ref<GEN::GenDefineElement> > & defines )
{
  for( unsigned i = 0; i < defines.size(); i++ )
    {
      if( icase_cmp( item->name, defines[i]->get_name() ) )
	{
	  define = defines[i]->clone();
	  break;
	}
    }

  if( !define.valid() )
    {
      error = x2s( item->line ) + ": unknown item " + item->name;
      return false;
    }

  tag.name = define->get_name();

  for( unsigned i = 0; i < define->children.size(); i++ )
    {
      GenDefOptionElement *ol = dynamic_cast<GenDefOptionElement*>( define->children[i].address() );

      if( ol )
	{
	  if( !ol->check( item ) )
	    {
	      error = x2s( item->line ) + ": " + ol->error;
	      return false;
	    }
	  else 
	    {
	      if( ol->add_it() )
		{
		  tag.add_option( ol->get_option() );
		}
	      else
		tag.set_option( ol->get_option() );
	    }

	  continue;
	}
    }

  if( define->declare() )
    {
      for( unsigned i = 0; i < define->children.size(); i++ )
	{
	  GenDefImplementElement *il = dynamic_cast<GenDefImplementElement*>( define->children[i].address() );
	  
	  if( il )
	    {
	      il->append( item );
	      continue;
	    }
	}
    }


  for( unsigned i = 0; i < item->subitems.size(); i++ )
    {
      GenConstructElement* e = new GenConstructElement();

      e->parent = this;
      
      if( !e->construct( item->subitems[i], defines ) )
	{
	  error = e->error;
	  return false;
	}

      children.push_back( e );
    }

  return true;
}

bool GEN::GenConstructElement::implement( GenConstructElement *p )
{
  if( p == 0 && define->declare() )
    {
      for( unsigned i = 0; i < children.size(); i++ )
	{
	  GenConstructElement *e = dynamic_cast<GenConstructElement*>( children[i].address() );

	  if( !e )
	    {
	      error = "strange things happend";
	      return false;
	    }

	  if( !e->implement( this ) )
	    {
	      error = children[i]->error;
	      return false;
	    }
	}
    }
  else if( p )
    {
      for( unsigned i = 0; i < p->tag.options.size(); i++ )
	{
	  for( unsigned k = 0; k < tag.options.size(); k++ )
	    {
	      if( tag.options[k].value == ( "::" + p->tag.options[i].name ) )
		{
		  tag.options[k].value = p->tag.options[i].value;
		}
	    }
	}

      for( unsigned i = 0; i < children.size(); i++ )
	{
	  GenConstructElement *e = dynamic_cast<GenConstructElement*>( children[i].address() );

	  if( !e )
	    {
	      error = "strange things happend";
	      return false;
	    }

	  if( !e->implement( p ) )
	    {
	      error = children[i]->error;
	      return false;
	    }
	}
    } 
  else 
    {
      for( unsigned i = 0; i < children.size(); i++ )
	{
	  GenConstructElement *e = dynamic_cast<GenConstructElement*>( children[i].address() );

	  if( !e )
	    {
	      error = "strange things happend";
	      return false;
	    }

	  if( !e->implement( p ) )
	    {
	      error = children[i]->error;
	      return false;
	    }
	}
    }

  return true;
}

GEN::GenConstructElement* GEN::GenConstructElement::clone()
{
  GenConstructElement *el = new GenConstructElement;

  el->define = define->clone();
  el->tag = tag;
  
  for( unsigned i = 0; i < children.size(); i++ )
    {
      el->children.push_back( children[i]->clone() );
    }

  return el;
}

std::string contruct_tree( Ref<XML::Item> item, std::vector<Ref<GEN::GenDefineElement> > & defines,   
			   Ref<GenConstructElement> &el )
{
  Ref<XML::Item> i = find_item( item, "app" );
  
  el = new GenConstructElement();

  if( !el->construct( i, defines ) )
    {
      return el->error;
    }

  if( !el->implement() )
    {
      return el->error;
    }

  return std::string();
}

static std::string contruct_deps( std::vector<Ref<GEN::GenDefineElement> > & defines )
{
  for( unsigned i = 0; i < defines.size(); i++ )
    {
      if( !defines[i]->get_parent_name().empty() )
	{
	  bool found = false;

	  for( unsigned j = 0; j < defines.size(); j++ )
	    {
	      if( icase_cmp( defines[j]->get_name(), defines[i]->get_parent_name() ) )
		{
		  for( unsigned k = 0; k < defines[j]->children.size(); k++ )
		    {
		      defines[i]->children.push_back( defines[j]->children[k]->clone() );
		    }
		  found = true;
		  break;
		}
	    }

	  if( !found )
	    {
	      return "cannot find parent " + defines[i]->get_parent_name();
	    }
	}
    }

  return std::string();
}

std::string parse_defines( Ref<XML::Item> item, std::vector<Ref<GEN::GenDefineElement> > & defines )
{
  for( unsigned i = 0; i < item->subitems.size(); i++ )
    {
      if( icase_cmp( item->subitems[i]->name, "define" ) )
	{
	  Ref<GenDefineElement> el = new GenDefineElement();
	  if( !el->parse( item->subitems[i] ) )
	    {
	      return el->error;
	    }

	  defines.push_back( el );
	}
      else if( icase_cmp( item->subitems[i]->name, "declare" ) )
	{
	  Ref<GenDefineElement> el = new GenDefineElement( true );
	  if( !el->parse( item->subitems[i] ) )
	    {
	      return el->error;
	    }

	  defines.push_back( el );
	}
    }  

  return contruct_deps( defines );
}

