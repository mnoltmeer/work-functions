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

String TStructuredData::FGetVal(int ind)
{
  String res;

  try
	 {
	   if ((ind < 0) || (ind >= Count))
		 throw Exception("Out of range");
	   else
		 res = FFields[ind];
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
	   if ((ind < 0) || (ind >= Count))
		 throw Exception("Out of range");
	   else
         FFields[ind] = val;
	 }
  catch (Exception &e)
	 {
	   e.Message = "TStructuredData::Fields: " + e.Message;
	   throw e;
	 }
}
//---------------------------------------------------------------------------

void TStructuredData::ImportData(String data_string, String delim)
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
						//_wstrncopy(text, cstr, sspos, i - sspos + 1);
						vecStrings.push_back(operstr.SubString(sspos, i - sspos + 1));
				   		op = false;
					  }
				  }
			  }

		   String str;

		   for (int i = 0; i < vecStrings.size(); i++)
			  MessageBox(NULL, vecStrings[i].c_str(), L"", MB_OK);
		 }

	   
	 }
  catch (Exception &e)
	 {
	   e.Message = "TStructuredData::ImportData: " + e.Message;
	   throw e;
	 }
}
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
