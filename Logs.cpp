//---------------------------------------------------------------------------

#pragma hdrstop

#include "Logs.h"
//---------------------------------------------------------------------------

//сохраняет текст в файл
void SaveToFile(String file, String text)
{
  TStringStream *ms = new TStringStream(text, TEncoding::UTF8, true);

  try
	 {
	   ms->Position = 0;
	   ms->SaveToFile(file);
	 }
  __finally {delete ms;}
}
//---------------------------------------------------------------------------

void AddToFile(String file, String text)
{
  if (!FileExists(file))
	SaveToFile(file, text);
  else
	{
	  TFileStream *srv_file = new TFileStream(file, fmOpenWrite);
	  TStringStream *ms = new TStringStream(text, TEncoding::UTF8, true);

	  try
		 {
		   ms->Position = 0;
		   srv_file->Position = srv_file->Size;
		   srv_file->CopyFrom(ms, ms->Size);
		 }
	  __finally {delete srv_file; delete ms;}
	}
}
//---------------------------------------------------------------------------

void SaveLog(String file, String rec)
{
//добавим к тексту дату и время добавления
  rec = "[" +
		DateToStr(Date()) +
		" " +
		TimeToStr(Time()) +
		"]" +
		" : " +
		text +
		"\r\n";

//сохраняем в файл
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
	   SaveLog("exceptions.log", "SaveLog(): " + e.ToString());
	 }
}
//---------------------------------------------------------------------------
#pragma package(smart_init)
