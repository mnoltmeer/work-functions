//---------------------------------------------------------------------------
#include <memory>
#pragma hdrstop

#include "Logs.h"
//---------------------------------------------------------------------------

//сохраняет текст в файл
void SaveToFile(String file, String text)
{
  std::unique_ptr<TStringStream> ms(new TStringStream(text, TEncoding::UTF8, true));

  ms->Position = 0;
  ms->SaveToFile(file);
}
//---------------------------------------------------------------------------

void AddToFile(String file, String text)
{
  if (!FileExists(file))
	SaveToFile(file, text);
  else
	{
	  std::unique_ptr<TFileStream> srv_file(new TFileStream(file, fmOpenWrite));
	  std::unique_ptr<TStringStream> ms(new TStringStream(text, TEncoding::UTF8, true));

	  ms->Position = 0;
	  srv_file->Position = srv_file->Size;
	  srv_file->CopyFrom(ms, ms->Size);
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
		rec +
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
