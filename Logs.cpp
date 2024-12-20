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

#include <memory>
#pragma hdrstop

#include "Logs.h"
//---------------------------------------------------------------------------

void SaveToFile(String file, String text)
{
  try
	 {
	   std::unique_ptr<TStringStream> ms(new TStringStream(text, TEncoding::UTF8, true));

	   ms->Position = 0;
	   ms->SaveToFile(file);
	 }
  catch (Exception &e)
	 {
	   SaveLogToUserFolder("exceptions.log", "", "SaveToFile: " + e.ToString());
	 }
}
//---------------------------------------------------------------------------

void AddToFile(String file, String text)
{
  try
	 {
	   if (!FileExists(file))
		 SaveToFile(file, text);
	   else
		 {
		   std::unique_ptr<TFileStream> srv_file(new TFileStream(file, fmOpenWrite));
		   std::unique_ptr<TStringStream> ms(new TStringStream(text, TEncoding::UTF8, true));

		   ms->Position = 0;
		   srv_file->Position = srv_file->Size;
		   srv_file->CopyFrom(ms.get(), ms->Size);
		 }
	 }
  catch (Exception &e)
	 {
	   SaveLogToUserFolder("exceptions.log", "", "AddToFile: " + e.ToString());
	 }

  
}
//---------------------------------------------------------------------------

void SaveLog(String file, String rec)
{
//������ �� ������ ���� ���� �� ���
  rec = "[" +
		DateToStr(Date()) +
		" " +
		TimeToStr(Time()) +
		"]" +
		" : " +
		rec +
		"\r\n";

  AddToFile(file, rec);
}
//---------------------------------------------------------------------------

void SaveLog(String text, TStringList *log)
{
  try
	 {
	   String msg = "[" +
					DateToStr(Date()) +
					" " +
					TimeToStr(Time()) +
					"]" +
					" : " +
					text;

	   log->Add(msg);
	 }
  catch (Exception &e)
	 {
	   SaveLogToUserFolder("exceptions.log", "", "SaveLog: " + e.ToString());
	 }
}
//---------------------------------------------------------------------------

void SaveLogToUserFolder(const String &file, const String &subdir, const String &msg)
{
//������ �� ������ ���� ���� �� ���
  String rec = "[" + DateToStr(Date())
				+ " "
				+ TimeToStr(Time())
				+ "]"
				+ " : "
				+ msg
				+ "\r\n";

  String path = GetEnvironmentVariable("USERPROFILE") + "\\";

  if (subdir != "")
	path += subdir + "\\" + file;
  else
	path += file;

  AddToFile(path, rec);
}
//---------------------------------------------------------------------------
#pragma package(smart_init)
