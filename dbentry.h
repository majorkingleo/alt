#ifndef DB_ENTRY_H
#define DB_ENTRY_H

#include "fx.h"
#include "dbi.h"

class DBEntry : public FXTextField
{
  FXDECLARE( DBEntry );
 public: 

 private:
  Tools::DBType *dbtype;

  Tools::DBType::TYPE type;
  unsigned len;
  bool is_signed;
  
  DBEntry() {}

 public:
  DBEntry( Tools::DBType *dbtype, FXComposite* p,FXint ncols,FXObject* tgt=NULL,FXSelector sel=0,FXuint opts=TEXTFIELD_NORMAL );

  long onVerify(FXObject* o,FXSelector sel,void* p);

};

#endif
