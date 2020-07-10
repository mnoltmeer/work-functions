//---------------------------------------------------------------------------

#pragma hdrstop

#include "Logs.h"
#include "ThreadSafeLog.h"
//---------------------------------------------------------------------------

void TThreadSafeLog::Add(const String &rec)
{
  try
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
  catch (Exception &e)
	 {
	   SaveLog("exceptions.log", "TThreadSafeLog::Add(): " + e.ToString());
	 }
}
//---------------------------------------------------------------------------

void TThreadSafeLog::Add(const String &rec, bool no_timestamp)
{
  try
	 {
	   if (no_timestamp)
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
	   SaveLog("exceptions.log", "TThreadSafeLog::Add(): " + e.ToString());
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
			SaveLog("exceptions.log", "TThreadSafeLog::IndexOf(): " +
					e.ToString());
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
			SaveLog("exceptions.log", "TThreadSafeLog::Get(): " +
					e.ToString());
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
		  {FList->Delete(ind);
		  }
	   catch (Exception &e)
		  {
			SaveLog("exceptions.log", "TThreadSafeLog::Remove(): " +
					e.ToString());
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
			SaveLog("exceptions.log", "TThreadSafeLog::Clear(): " +
					e.ToString());
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
			SaveLog("exceptions.log", "TThreadSafeLog::SaveToFile(): " +
					e.ToString());
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
			SaveLog("exceptions.log", "TThreadSafeLog::SaveToStream(): " +
					e.ToString());
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
			SaveLog("exceptions.log", "TThreadSafeLog::LoadFromFile(): " +
					e.ToString());
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
			SaveLog("exceptions.log", "TThreadSafeLog::LoadFromFile(): " +
					e.ToString());
		  }
	 }
  __finally {FLock->Leave();}

  return text;
}
//---------------------------------------------------------------------------

#pragma package(smart_init)
