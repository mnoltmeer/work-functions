//---------------------------------------------------------------------------

#ifndef LogsH
#define LogsH

#include <System.Classes.hpp>
//---------------------------------------------------------------------------

  void SaveToFile(String file, String text);
  void AddToFile(String file, String text);

//заносит лог события в файл
  void SaveLog(String file, String rec);
  void SaveLog(String text, TStringList *log);
#endif
