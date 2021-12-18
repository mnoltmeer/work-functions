/*!
Copyright 2021 Maxim Noltmeer (m.noltmeer@gmail.com)

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

#ifndef ImportExportH
#define ImportExportH

#include <System.Classes.hpp>
#include <vector>

//---------------------------------------------------------------------------
class TStructuredData
{
  private:
	std::vector<String> FFields;

	String FGetVal(int ind);
	void FSetVal(int ind, const String &val);

	inline int FCount(){return FFields.size();}

  public:
	TStructuredData(){};
	TStructuredData(int col_cnt){FFields.reserve(col_cnt);}
	inline virtual ~TStructuredData(){Clear();}

	inline void Clear(){FFields.clear();}

	void ImportData(String data_string, String delim);

	__property int Count = {read = FCount};
	__property String Fields[int ind] = {read = FGetVal, write = FSetVal};
};

class TDataHolder
{
  private:
	std::vector<TStructuredData*> FRecords;

	String FGetVal(int col, int row);
	void FSetVal(int col, int row, const String &val);

	inline int FCount(){return FRecords.size();}

  public:
	TDataHolder(){};
	inline virtual ~TDataHolder(){Clear();}

	inline void Clear(){for (int i = 0; i < RecordCount; i++){delete FRecords[i]; FRecords.clear();}}

	__property int RecordCount = {read = FCount};
	__property String Cells[int col][int row] = {read = FGetVal, write = FSetVal};
};


String ParseString(const String &main_str, String target_str, const String insert_str);
#endif
