#ifndef GENERATOR_H
#define GENERATOR_H

#include "xml.h"
#include "ref.h"
#include "elements2.h"

#include <sstream>

Ref<XML::Item> find_item( Ref<XML::Item> item, const std::string & name );
bool update_file( std::stringstream &out, const std::string &file_name, bool append = false );
bool update_file2( std::stringstream &out, const std::string &file_name, bool append = false );
std::string parse_defines( Ref<XML::Item> item, std::vector<Ref<GEN::GenDefineElement> > & defines );
std::string contruct_tree( Ref<XML::Item> item, std::vector<Ref<GEN::GenDefineElement> > & defines,
			   Ref<GEN::GenConstructElement> &el );
std::string gen_struct_code( Ref<GEN::GenConstructElement> el, const std::string &dir );
std::string gen_gui_code( Ref<GEN::GenConstructElement> el, const std::string &dir );
std::string gen_sql_code( Ref<GEN::GenConstructElement> el, const std::string &dir );

#endif
