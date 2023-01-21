#ifndef GEN_CODE_DB_STRUCT_H
#define GEN_CODE_DB_STRUCT_H

#include "dbi.h"
#include "range.h"

class FES : public Tools::DBBindType
{
  public:
Tools::DBTypeInt 	id;
Tools::DBTypeVarChar 	description;
Tools::DBTypeVarChar 	feldid;
  public:
    FES();

};

class BOOKS : public Tools::DBBindType
{
  public:
Tools::DBTypeInt 	id;
struct ESTRUCT_1
{
  enum ETYPE {
     FIRST__ = -1,
     BOOK,
     CD,
     DVD,
     VHS,
     LAST__
  };
  static const char* STYPES[];
};
Tools::DBTypeEnum< Tools::EnumRange<ESTRUCT_1> > 	type;
Tools::DBTypeVarChar 	title;
Tools::DBTypeVarChar 	author;
Tools::DBTypeVarChar 	description;
Tools::DBTypeVarChar 	isbn;
Tools::DBTypeVarChar 	identifier;
Tools::DBTypeVarChar 	location;
Tools::DBTypeVarChar 	owner;
Tools::DBTypeVarChar 	comment;
  public:
    BOOKS();

};

class NOTES_HEADS : public Tools::DBBindType
{
  public:
Tools::DBTypeInt 	id;
Tools::DBTypeVarChar 	title;
Tools::DBTypeInt 	hist_anzeit;
Tools::DBTypeInt 	hist_aezeit;
struct ESTRUCT_2
{
  enum ETYPE {
     FIRST__ = -1,
     OPEN,
     PENDING,
     CLOSED,
     LAST__
  };
  static const char* STYPES[];
};
Tools::DBTypeEnum< Tools::EnumRange<ESTRUCT_2> > 	status;
  public:
    NOTES_HEADS();

};

class NOTES_LINES : public Tools::DBBindType
{
  public:
Tools::DBTypeInt 	id;
Tools::DBTypeInt 	head_id;
Tools::DBTypeInt 	hist_anzeit;
Tools::DBTypeInt 	hist_aezeit;
Tools::DBTypeVarChar 	note;
struct ESTRUCT_3
{
  enum ETYPE {
     FIRST__ = -1,
     NOTE,
     ALERT,
     LAST__
  };
  static const char* STYPES[];
};
Tools::DBTypeEnum< Tools::EnumRange<ESTRUCT_3> > 	type;
struct ESTRUCT_4
{
  enum ETYPE {
     FIRST__ = -1,
     OPEN,
     PENDING,
     CLOSED,
     LAST__
  };
  static const char* STYPES[];
};
Tools::DBTypeEnum< Tools::EnumRange<ESTRUCT_4> > 	status;
  public:
    NOTES_LINES();

};

class SEQUENCE : public Tools::DBBindType
{
  public:
Tools::DBTypeVarChar 	name;
Tools::DBTypeInt 	from;
Tools::DBTypeInt 	to;
Tools::DBTypeInt 	nextval;
Tools::DBTypeVarChar 	format;
  public:
    SEQUENCE();

};


#endif
