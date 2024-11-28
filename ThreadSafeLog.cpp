/*!
Copyright 2019-2021 Maxim Noltmeer (m.noltmeer@gmail.com)

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

#include "Logs.h"
#include "ThreadSafeLog.h"
//---------------------------------------------------------------------------

TThreadSafeLog::TThreadSafeLog()
{
  FList.reset(new TStringList());
  FLock.reset(new TCriticalSection());
}
//---------------------------------------------------------------------------

void TThreadSafeLog::Add(const String &rec)
{
  try
	 {
	   Add(rec, UseTimeStamp);
	 }
  catch (Exception &e)
	 {
	   SaveLogToUserFolder("exceptions.log", "", "TThreadSafeLog::Add: " + e.ToString());
	 }
}
//---------------------------------------------------------------------------

void TThreadSafeLog::Add(const String &rec, bool timestamp)
{
  try
	 {
	   if (!timestamp)
		 FList->Add(rec);
	   else
		 {
		   String msg = "[" +
						DateToStr(Date()) +
						" " +
						TimeToStr(Time()) +
						"]" +
						" : " +
						rec;

		   FList->Add(msg);
		 }
	 }
  catch (Exception &e)
	 {
	   SaveLogToUserFolder("exceptions.log", "", "TThreadSafeLog::Add: " + e.ToString());
	 }
}
//---------------------------------------------------------------------------

int TThreadSafeLog::IndexOf(const String &rec)
{
  int index;

  FLock->Enter();

  try
	 {
	   try
		  {
			index = FList->IndexOf(rec);
		  }
	   catch (Exception &e)
		  {
			index = -1;
			SaveLogToUserFolder("exceptions.log", "", "TThreadSafeLog::IndexOf(): " + e.ToString());
		  }
	 }
  __finally {FLock->Leave();}

  return index;
}
//---------------------------------------------------------------------------

String TThreadSafeLog::Get(int ind)
{
  String rec;

  FLock->Enter();

  try
	 {
	   try
		  {
			rec = FList->Strings[ind];
		  }
	   catch (Exception &e)
		  {
			rec = "";
			SaveLogToUserFolder("exceptions.log", "", "TThreadSafeLog::Get: " + e.ToString());
		  }
	 }
  __finally {FLock->Leave();}

  return rec;
}
//---------------------------------------------------------------------------

void TThreadSafeLog::Remove(int ind)
{
  FLock->Enter();

  try
	 {
	   try
		  {
		    FList->Delete(ind);
		  }
	   catch (Exception &e)
		  {
			SaveLogToUserFolder("exceptions.log", "", "TThreadSafeLog::Remove: " + e.ToString());
		  }
	 }
  __finally {FLock->Leave();}
}
//---------------------------------------------------------------------------

void TThreadSafeLog::Clear()
{
  FLock->Enter();

  try
	 {
	   try
		  {
			FList->Clear();
		  }
	   catch (Exception &e)
		  {
			SaveLogToUserFolder("exceptions.log", "", "TThreadSafeLog::Clear: " + e.ToString());
		  }
	 }
  __finally {FLock->Leave();}
}
//---------------------------------------------------------------------------

void TThreadSafeLog::SaveToFile(const String &file)
{
  FLock->Enter();

  try
	 {
	   try
		  {
			FList->SaveToFile(file, TEncoding::UTF8);
		  }
	   catch (Exception &e)
		  {
			SaveLogToUserFolder("exceptions.log", "", "TThreadSafeLog::SaveToFile: " + 	e.ToString());
		  }
	 }
  __finally {FLock->Leave();}
}
//---------------------------------------------------------------------------

void TThreadSafeLog::SaveToStream(TStringStream *stream)
{
  FLock->Enter();

  try
	 {
	   try
		  {
			FList->SaveToStream(stream, TEncoding::UTF8);
		  }
	   catch (Exception &e)
		  {
			SaveLogToUserFolder("exceptions.log", "", "TThreadSafeLog::SaveToStream: " + e.ToString());
		  }
	 }
  __finally {FLock->Leave();}
}
//---------------------------------------------------------------------------

void TThreadSafeLog::LoadFromFile(const String &file)
{
  FLock->Enter();

  try
	 {
	   try
		  {
			FList->LoadFromFile(file, TEncoding::UTF8);
		  }
	   catch (Exception &e)
		  {
			SaveLogToUserFolder("exceptions.log", "", "TThreadSafeLog::LoadFromFile: " + e.ToString());
		  }
	 }
  __finally {FLock->Leave();}
}
//---------------------------------------------------------------------------

String TThreadSafeLog::GetText()
{
  String text;

  FLock->Enter();

  try
	 {
	   try
		  {
			text = FList->GetText();
		  }
	   catch (Exception &e)
		  {
			text = "";
			SaveLogToUserFolder("exceptions.log", "", "TThreadSafeLog::LoadFromFile: " + e.ToString());
		  }
	 }
  __finally {FLock->Leave();}

  return text;
}
//---------------------------------------------------------------------------

#pragma package(smart_init)
