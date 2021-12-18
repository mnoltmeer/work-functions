/*!
Copyright 2018-2021 Maxim Noltmeer (m.noltmeer@gmail.com)

This program is free software: you can redistribute it and/or modify
    it under the terms of the GNU General Public License as published by
	the Free Software Foundation, either version 3 of the License, or
	(at your option) any later version.

	This program is distributed in the hope that it will be useful,
	but WITHOUT ANY WARRANTY; without even the implied warranty of
	MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
    GNU General Public License for more details.

	You should have received a copy of the GNU General Public License
	along with this program.  If not, see <https://www.gnu.org/licenses/>.
*/

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
