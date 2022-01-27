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
#include <ComObj.hpp>
#include <memory>
#include <vector>

#define IN_THREAD true
#define NOT_IN_THREAD false

//---------------------------------------------------------------------------
class TStructuredData
{
  private:
	std::unique_ptr<TStringList> FFields;

	String FGetVal(int ind);
	void FSetVal(int ind, const String &val);

	inline int FCount(){return FFields->Count;}

  public:
	TStructuredData();
	TStructuredData(const String &data_string, const String &delim);
	inline virtual ~TStructuredData(){};

	void ImportData(const String &data_string, const String &delim);
	String ExportData(const String &delim);
	inline void Add(const String &data){FFields->Add(data);}
	inline void Remove(int ind){FFields->Delete(ind);}

	__property int FieldCount = {read = FCount};
	__property String Fields[int ind] = {read = FGetVal, write = FSetVal};
};

class TDataHolder
{
  private:
	std::vector<TStructuredData*> FRecords;

	String FGetCell(int col, int row);
	void FSetCell(int col, int row, const String &val);

	TStructuredData *FGetRow(int ind);
	void FSetRow(int ind, TStructuredData *row);

	inline int FCount(){return FRecords.size();}

  public:
	TDataHolder(){};
	inline virtual ~TDataHolder(){Clear();}

	void Clear();

	void ImportCSV(const String &file, const String &delim);
	void ExportCSV(const String &file, const String &delim);
	void ImportXLS(const String &file, int last_row, int last_col, bool used_thread);
	void ExportXLS(const String &file, bool used_thread);
	inline void ImportXLS(const String &file, int last_row, int last_col){ImportXLS(file, last_row, last_col, NOT_IN_THREAD);}
	inline void ExportXLS(const String &file){ExportXLS(file, NOT_IN_THREAD);}

	TStructuredData *Add();
	TStructuredData *Add(const String &data_string, const String &delim);
	void Remove(int ind);

	__property int RecordCount = {read = FCount};
	__property TStructuredData *Rows[int ind] = {read = FGetRow, write = FSetRow};
	__property String Cells[int col][int row] = {read = FGetCell, write = FSetCell};
};


String ParseString(const String &main_str, String target_str, const String insert_str);
void StrToList(TStringList *list, String text, String delim);
String ListToStr(TStringList *list, String delim);
#endif
