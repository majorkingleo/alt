#ifndef DB_UTIL_H
#define DB_UTIL_H

#include <string>

class FXWindow;

void DBError( FXWindow *owner, const std::string &what = "Speichern fehlgeschlagen!" );

#endif
