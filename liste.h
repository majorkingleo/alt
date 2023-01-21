#ifndef LISTE_H
#define LISTE_H

#include "fx.h"
#include "db_struct.h"

class Liste : public FXDialogBox
{
  FXDECLARE( Liste );

 public:
  FXTable *table;
  Tools::DBBindType *bt;

  enum
    {
      ID_SAVE_LAYOUT = FXDialogBox::ID_LAST,
      ID_OK,
      ID_LAST
    };

 protected:
  Liste() {};  

 public:
  Liste( FXWindow *owner, const std::string &title );
  ~Liste();

  long onSaveLayout( FXObject *obj, FXSelector sel, void *ptr );
  long onOk( FXObject *obj, FXSelector sel, void *ptr );

  template<class T> bool create_liste()
  {
    T bind_type;

    if( !Liste::bt )
      Liste::bt = new T();

    unsigned count = count_erg( bind_type.get_table_name() );
    
    table->setTableSize( count + 1, bind_type.get_names().size() + 1 );

    restore_layout();

    table->setLeadingRows(1);
    table->setLeadingCols(1);
    table->setItemText(0,0, FXString::null );
    
    table->getItem( 0, 0 )->setButton(true);
    
    for( unsigned i = 1; i < bind_type.get_names().size() + 1; i++ )
      {
	table->setItemText( 0, i, bind_type.get_names()[i-1].c_str() );
	table->getItem( 0, i )->setButton( true );
      }

    for( unsigned i = 1; i < count + 1; i++ )
      {
	table->setItemText( i, 0, x2s( i ).c_str() );
	table->getItem( i, 0 )->setButton(true);
      }
    
    std::vector< Tools::Ref<T> > bt;
    
    Tools::StdSqlSelect( *db, bind_type.get_table_name(), bt );
    
    for( unsigned i = 0; i < bt.size(); i++ )
      {
	for( unsigned j = 0; j < bind_type.get_types().size(); j++ )
	  {
	    table->setItemText( i + 1, j + 1, bt[i]->get_types()[j]->str().c_str() );
	  }
      }

    db->rollback();   

    return true;
  }


 private:
  unsigned count_erg( const std::string &table_name );
  bool restore_layout();
};

#endif
