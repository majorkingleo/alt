#ifndef ICONS_H
#define ICONS_H

#include "fx.h"
#include <vector>

#include "string_utils.h"

class IconManager
{
  FXApp *app;

  struct Icon
  {
    std::string name;
    FXIcon *icon;
    unsigned char *data;

    Icon( const std::string &name, FXIcon *icon, unsigned char *data = 0 )
      : name( name ), icon( icon ), data ( data )
    {}

    ~Icon()
    {
      delete icon;
    }
  };

  std::vector<Icon> icons;

 public:
  IconManager( FXApp *app ) : app( app ) {}
    
  FXIcon * get_icon( const std::string &name );
    
  void add( const std::string &name, const unsigned char *icon );

  void create();
  static void implement();
};

extern IconManager *Icons;


#endif
