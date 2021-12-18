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

#pragma hdrstop

#include "ImportExport.h"

#pragma package(smart_init)
//---------------------------------------------------------------------------

TStructuredData::TStructuredData()
{
  FFields.reset(new TStringList());
}
//---------------------------------------------------------------------------

TStructuredData::TStructuredData(const String &data_string, const String &delim)
{
  FFields.reset(new TStringList());
  ImportData(data_string, delim);
}
//---------------------------------------------------------------------------

String TStructuredData::FGetVal(int ind)
{
  String res;

  try
	 {
	   if ((ind < 0) || (ind >= FieldCount))
		 throw Exception("Out of range");
	   else
		 {
		   res = FFields->Strings[ind];

		   int len = res.Length();

		   if ((len > 0) &&
			   (res[1] == '"') &&
			   (res[len] == '"')) //якщо це конст. строка, повернеом "чисті" дані
			 {
			   res = res.Delete(len, 1);
			   res = res.Delete(1, 1);
			 }
		 }
	 }
  catch (Exception &e)
	 {
	   res = "";
	   e.Message = "TStructuredData::Fields: " + e.Message;
	   throw e;
	 }

  return res;
}
//---------------------------------------------------------------------------

void TStructuredData::FSetVal(int ind, const String &val)
{
  try
	 {
	   if ((ind < 0) || (ind >= FieldCount))
		 throw Exception("Out of range");
	   else
		 FFields->Strings[ind] = val;
	 }
  catch (Exception &e)
	 {
	   e.Message = "TStructuredData::Fields: " + e.Message;
	   throw e;
	 }
}
//---------------------------------------------------------------------------

void TStructuredData::ImportData(const String &data_string, const String &delim)
{
  try
	 {
	   String operstr = data_string;
	   int sspos = -1;
	   bool op = false;
	   UINT len = operstr.Length();
	   std::vector<String> vecStrings;

	   if (operstr.Pos("\""))
		 {
		   for (UINT i = 1; i < len; i++) //знайдемо всі конст. строки і занесемо у вектор
			  {
				if ('"' == operstr[i])
				  {
					if (!op)
					  {
						sspos = i;
						op = true;
					  }
					else
					  {
						vecStrings.push_back(operstr.SubString(sspos, i - sspos + 1));
						op = false;
					  }
				  }
			  }

		   for (int i = 0; i < vecStrings.size(); i++) //замінюємо конст. строки на змінні
			  operstr = ParseString(operstr, vecStrings[i], "$" + IntToStr(i));
		 }

	   StrToList(FFields.get(), operstr, delim); //розбиваємо на поля

	   for (int i = 0; i < FieldCount; i++)
		  {
			for (int j = 0; j < vecStrings.size(); j++)
			   {
				 if (FFields->Strings[i].Pos("$" + IntToStr(j)))
				   {
					 FFields->Strings[i] = ParseString(FFields->Strings[i],
													   "$" + IntToStr(j),
													   vecStrings[j]);
				   }
			   }
		  }
	 }
  catch (Exception &e)
	 {
	   e.Message = "TStructuredData::ImportData: " + e.Message;
	   throw e;
	 }
}
//---------------------------------------------------------------------------

String TStructuredData::ExportData(const String &delim)
{
  String res;

  try
	 {
       res = ListToStr(FFields.get(), delim);
	 }
  catch (Exception &e)
	 {
	   e.Message = "TStructuredData::ExportData: " + e.Message;
	   throw e;
	 }

  return res;
}
//---------------------------------------------------------------------------
//---------------------------------------------------------------------------
//---------------------------------------------------------------------------

String TDataHolder::FGetCell(int row, int col)
{
  String res;

  try
	 {
	   if ((row < 0) || (row >= RecordCount))
		 throw Exception("Out of range");
	   else
		 res = FRecords[row]->Fields[col];
	 }
  catch (Exception &e)
	 {
	   res = "";
	   e.Message = "TDataHolder::Cells: " + e.Message;
	   throw e;
	 }

  return res;
}
//---------------------------------------------------------------------------

void TDataHolder::FSetCell(int row, int col, const String &val)
{
  try
	 {
	   if ((row < 0) || (row >= RecordCount))
		 throw Exception("Out of range");
	   else
		 FRecords[row]->Fields[col] = val;
	 }
  catch (Exception &e)
	 {
	   e.Message = "TDataHolder::Cells: " + e.Message;
	   throw e;
	 }
}
//---------------------------------------------------------------------------

TStructuredData *TDataHolder::FGetRow(int ind)
{
  TStructuredData *res;

  try
	 {
	   if ((ind < 0) || (ind >= RecordCount))
		 throw Exception("Out of range");
	   else
		 res = FRecords[ind];
	 }
  catch (Exception &e)
	 {
	   res = nullptr;
	   e.Message = "TDataHolder::Rows: " + e.Message;
	   throw e;
	 }

  return res;
}
//---------------------------------------------------------------------------

void TDataHolder::FSetRow(int ind, TStructuredData *row)
{
  try
	 {
	   if ((ind < 0) || (ind >= RecordCount))
		 throw Exception("Out of range");
	   else if (!row)
		 throw Exception("Invalid TStructuredData pointer");
	   else
		 {
		   delete FRecords[ind];
		   FRecords[ind] = row;
		 }
	 }
  catch (Exception &e)
	 {
	   e.Message = "TDataHolder::Rows: " + e.Message;
	   throw e;
	 }
}
//---------------------------------------------------------------------------

void TDataHolder::Clear()
{
  for (int i = 0; i < RecordCount; i++)
	 {
	   delete FRecords[i];
	   FRecords.clear();
	 }
}
//---------------------------------------------------------------------------

void TDataHolder::Import(const String &file, const String &delim)
{
  try
	 {
	   Clear();

	   std::unique_ptr<TStringList> lst(new TStringList());

	   lst->LoadFromFile(file, TEncoding::UTF8);

	   for (int  i = 0; i < lst->Count; i++)
		  FRecords.push_back(new TStructuredData(lst->Strings[i], delim));
	 }
  catch (Exception &e)
	 {
	   e.Message = "TDataHolder::Import: " + e.Message;
	   throw e;
	 }
}
//---------------------------------------------------------------------------

void TDataHolder::Export(const String &file, const String &delim)
{
  try
	 {
	   std::unique_ptr<TStringList> lst(new TStringList());

	   for (int  i = 0; i < RecordCount; i++)
		  lst->Add(FRecords[i]->ExportData(delim));

	   lst->SaveToFile(file, TEncoding::UTF8);
	 }
  catch (Exception &e)
	 {
	   e.Message = "TDataHolder::Import: " + e.Message;
	   throw e;
	 }
}
//---------------------------------------------------------------------------
//---------------------------------------------------------------------------
//---------------------------------------------------------------------------

String ParseString(const String &main_str, String target_str, const String insert_str)
{
  String result = "";

  try
	 {
	   result = main_str;
	   int pos = 0, count = 0;

	   while (result.Pos(target_str) != 0)
		 {
		   pos = result.Pos(target_str);
		   count = target_str.Length();
		   result = result.Delete(pos, count);
		   result = result.Insert(insert_str, pos);
		 }
	 }
  catch (Exception &e)
	 {
	   e.Message = "ParseString: " + e.Message;
	   throw e;
	 }

  return result;
}
//---------------------------------------------------------------------------

void StrToList(TStringList *list, String text, String delim)
{
  int i;
  String str;

//если разделитель не задан, по умолчанию используется признак конца строки
  if (delim == NULL)
	delim = "\r\n";

  try
	 {
	   list->Clear();

//будем обрабатывать исходную строку пока будет встречаться символ-разделитель
	   while (text.Pos(delim) != 0)
		 {
//копируем все что до символа-разделителя в str
		   i = text.Pos(delim);
		   str = text.SubString(1, i - 1);
//обрезаем строку по символ-разделитель
		   text = text.Delete(1, i);
//заносим содержимое str в список
		   if ((str != NULL) || (str != ""))
		     list->Add(str);
		 }

	   list->Add(text);
	 }
  catch (Exception &e)
	 {
	   e.Message = "StrToList: " + e.Message;
	   throw e;
	 }
}
//---------------------------------------------------------------------------

String ListToStr(TStringList *list, String delim)
{
  String str_list;

  try
	 {
//если разделитель не задан, по умолчанию используется признак конца строки
	   if (delim == NULL)
		 delim = "\r\n";

	   if (list->Count > 0)
		 {
		   for (int i = 0; i < list->Count; i++)
			  str_list = str_list + list->Strings[i] + delim;

		   str_list = str_list.Delete(str_list.LastDelimiter(delim), delim.Length());
		 }
	 }
  catch (Exception &e)
	 {
	   str_list = "";
	   e.Message = "ListToStr: " + e.Message;
	   throw e;
	 }

  return str_list;
}
//---------------------------------------------------------------------------
