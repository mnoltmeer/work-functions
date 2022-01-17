/*!
Copyright 2022 Maxim Noltmeer (m.noltmeer@gmail.com)

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

#ifndef QueryManagerH
#define QueryManagerH

#include <FireDAC.Comp.Client.hpp>
#include <Xml.XMLDoc.hpp>
#include <Xml.omnixmldom.hpp>
#include <vector>
#include <memory>
//---------------------------------------------------------------------------

struct TextMark
{
  wchar_t Mark[10];
  String Text;
};

class TManagedQuery
{
  private:
	std::unique_ptr<TFDTransaction> FTrans;
	std::unique_ptr<TFDQuery> FQuery;
	String FText;
	String FID;
	std::vector<TextMark> FProcMarks;
	std::vector<TextMark> FStrMarks;

	TFDQuery *FGetQuery(){return FQuery.get();}
	TFDParam *FGetParam(const String &name);
	TField *FGetField(const String &name);
	int FGetRecCount(){return FQuery->RecordCount;}

    String ParsingProcedures(String query_text);
	String InsertProcedureText(String text);
	String ParsingStrings(String query_text);
	String ReparseStringsInProcedure(String procedure_text);
    String PrepareStringParams(String text);

  public:
	TManagedQuery(const String &id, TFDConnection *conn);
	inline virtual ~TManagedQuery(){};

	bool Execute();

	__property TFDQuery *DataSet = {read = FGetQuery};
	__property String ID = {read = FID, write = FID};
	__property String Text = {read = FText, write = FText};
	__property TFDParam *Params[const String &name] = {read = FGetParam};
	__property TField *Fields[const String &name] = {read = FGetField};
	__property int RecordCount = {read = FGetRecCount};
};

class TQueryManager
{
  private:
	std::vector<TManagedQuery*> FQueries;
	TFDConnection *FConn;

	TManagedQuery *FGetItem(int ind);
	void FSetItem(int ind, TManagedQuery *val);

	TManagedQuery *FGetItemID(const String &id);
	void FSetItemID(const String &id, TManagedQuery *val);

	inline int FCount(){return FQueries.size();}

    int IndexOf(const String &id);

  public:
	TQueryManager(TFDConnection *conn){FConn = conn;}
	inline virtual ~TQueryManager(){Clear();}

	void Clear();

	TManagedQuery *Add(const String &id, TFDConnection *conn);
	TManagedQuery *Add(TManagedQuery *query);
	void Remove(const String &id);
	void Remove(int ind);

	void SaveToFile(const String &xml);
	void LoadFromFile(const String &xml);

	bool ImportFromStream(TStringStream *ms);
    TStringStream *ExportToStream();

	__property int Count = {read = FCount};
	__property TManagedQuery *Items[int ind] = {read = FGetItem, write = FSetItem};
	__property TManagedQuery *ItemsByID[const String &id] = {read = FGetItemID, write = FSetItemID};
};
//---------------------------------------------------------------------------

TFDTransaction *CreateNewTransactionObj(TFDConnection *conn);
TFDQuery *CreateNewQueryObj(TFDTransaction *t, TFDConnection *conn);
String ParseString(const String &main_str, String target_str, const String insert_str);
#endif
