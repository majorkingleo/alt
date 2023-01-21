#include "db_struct.h"

FES::FES()
 : Tools::DBBindType( "FES" ),
id( this, "id"),
description( this, "description", 50),
feldid( this, "feldid", 50)
{}

BOOKS::BOOKS()
 : Tools::DBBindType( "BOOKS" ),
id( this, "id"),
type( this, "type", type.data.BOOK ),
title( this, "title", 255),
author( this, "author", 100),
description( this, "description", 255),
isbn( this, "isbn", 50),
identifier( this, "identifier", 50),
location( this, "location", 50),
owner( this, "owner", 50),
comment( this, "comment", 50)
{}

NOTES_HEADS::NOTES_HEADS()
 : Tools::DBBindType( "NOTES_HEADS" ),
id( this, "id"),
title( this, "title", 50),
hist_anzeit( this, "hist_anzeit"),
hist_aezeit( this, "hist_aezeit"),
status( this, "status", status.data.OPEN )
{}

NOTES_LINES::NOTES_LINES()
 : Tools::DBBindType( "NOTES_LINES" ),
id( this, "id"),
head_id( this, "head_id"),
hist_anzeit( this, "hist_anzeit"),
hist_aezeit( this, "hist_aezeit"),
note( this, "note", 255),
type( this, "type" ),
status( this, "status", status.data.OPEN )
{}

SEQUENCE::SEQUENCE()
 : Tools::DBBindType( "SEQUENCE" ),
name( this, "name", 25),
from( this, "from"),
to( this, "to"),
nextval( this, "nextval"),
format( this, "format", 50)
{}


const char* BOOKS::ESTRUCT_1::STYPES[] = {
     "BOOK",
     "CD",
     "DVD",
     "VHS",
};
const char* NOTES_HEADS::ESTRUCT_2::STYPES[] = {
     "OPEN",
     "PENDING",
     "CLOSED",
};
const char* NOTES_LINES::ESTRUCT_3::STYPES[] = {
     "NOTE",
     "ALERT",
};
const char* NOTES_LINES::ESTRUCT_4::STYPES[] = {
     "OPEN",
     "PENDING",
     "CLOSED",
};
