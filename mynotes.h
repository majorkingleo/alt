#ifndef MYNOTES_H
#define MYNOTES_H

#include "notes.h"

class MyNotes : public NotesWindow
{
 public:
  MyNotes( FXWindow *owner )
    : NotesWindow( owner )
    {}

  long button_combo_changed( FXObject* obj, FXSelector sel, void* ptr );
  long head_lesen( bool from_combo );
  long button_head_lesen( FXObject *obj, FXSelector sel, void *ptr);

};


#endif
