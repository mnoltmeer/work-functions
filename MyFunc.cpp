/*!
Copyright 2014-2019 Maxim Noltmeer (m.noltmeer@gmail.com)

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

#include <vcl.h>
#include <shellapi.h>
#include <strsafe.h>
#pragma hdrstop

#include "MyFunc.h"

//---------------------------------------------------------------------------

#pragma package(smart_init)

//---------------------------------------------------------------------------
// Функции общего пользования
//---------------------------------------------------------------------------
void CPTransOut(UINT mode, const char *in_str, UINT str_len)
{
  char *out_str = new char[str_len];

  switch (mode)
     {
       case CHAROEM:
		  {
			CharToOemBuffA(in_str, out_str, str_len);
            printf(out_str);
            delete[] out_str;
            out_str = NULL;
            break;
          }

       case OEMCHAR:
		  {
			OemToCharBuffA(in_str, out_str, str_len);
            printf(out_str);
            delete[] out_str;
            out_str = NULL;
			break;
          }
     }
};
//---------------------------------------------------------------------------

wchar_t* __fastcall UnicodeOf(const char* c)
{
	static wchar_t w[STR_CONV_BUF_SIZE];
	memset(w,0,sizeof(w));
	MultiByteToWideChar(CP_ACP, MB_PRECOMPOSED, c, strlen(c), w, STR_CONV_BUF_SIZE);
	return(w);
}
//---------------------------------------------------------------------------

char* __fastcall AnsiOf(const wchar_t* w)
{
	static char c[STR_CONV_BUF_SIZE];
	memset(c, 0, sizeof(c));
	WideCharToMultiByte(CP_ACP, WC_NO_BEST_FIT_CHARS, w, wcslen(w), c, STR_CONV_BUF_SIZE, NULL, NULL);
	return(c);
}
//---------------------------------------------------------------------------

int IsOper(wchar_t symb)
{
  for (UINT i = 0; i < OPSYMCNT; i++)
    {
	  if (symb == OperSymb[i])
        return i;
    }

  return -1;
}
//-------------------------------------------------------------------------------------------------

int IsNum(wchar_t symb)
{
  for (int i = 0; i < 10; i++)
    {
      if (symb == NumSymb[i])
        return i;
    }

  return -1;
}
//-------------------------------------------------------------------------------------------------

int IsStrNum(const wchar_t *somestring)
{
  static int result = 0;
  UINT cmcnt = 0;
  UINT i = 0, j = 0;

  if ('-' == somestring[0])
    {
      result = 2;
      j = 1;
    }

  for (i = j; i < wcslen(somestring); i++)
     {
       if (('0' == somestring[i]) ||
           ('1' == somestring[i]) ||
           ('2' == somestring[i]) ||
           ('3' == somestring[i]) ||
           ('4' == somestring[i]) ||
           ('5' == somestring[i]) ||
           ('6' == somestring[i]) ||
           ('7' == somestring[i]) ||
           ('8' == somestring[i]) ||
           ('9' == somestring[i]))
         {
           if (POS_INT == result)
             result = POS_INT;
         }
       else if ((',' == somestring[i]) || ('.' == somestring[i]))
         {
           cmcnt++;

           if (cmcnt > 1)
             return NON_FLT;

           if (POS_FLT == result)
             result = POS_FLT;
           else if (NEG_INT == result)
             result = NEG_FLT;
         }
       else
         return INC_CHR;
     }

  return result;
}
//---------------------------------------------------------------------------

void StartProcessByExeName(const String &file, const String &parent_dir)
{
  PROCESS_INFORMATION     piProcInfo;
  STARTUPINFO             siStartInfo;

  ZeroMemory(&siStartInfo, sizeof(siStartInfo));
  siStartInfo.cb = sizeof(STARTUPINFO);
  ZeroMemory(&piProcInfo, sizeof(piProcInfo));

  int res = CreateProcess(NULL,
						  file.c_str(),
						  NULL,           	// process security attributes
						  NULL,           	// primary thread security attributes
						  0,              	// handles are inherited
						  0,              	// creation flags
						  NULL,           	// use parent's environment
						  parent_dir.c_str(), // use parent's current directory
						  &siStartInfo,   	// STARTUPINFO pointer
						  &piProcInfo);   	// receives PROCESS_INFORMATION

  if (res)
	{
	  CloseHandle(piProcInfo.hProcess);
	  CloseHandle(piProcInfo.hThread);
	}
}
//---------------------------------------------------------------------------

void StartProcessByExeName(const String &file)
{
  PROCESS_INFORMATION     piProcInfo;
  STARTUPINFO             siStartInfo;

  ZeroMemory(&siStartInfo, sizeof(siStartInfo));
  siStartInfo.cb = sizeof(STARTUPINFO);
  ZeroMemory(&piProcInfo, sizeof(piProcInfo));

  int res = CreateProcess(NULL,
						  file.c_str(),
						  NULL,           // process security attributes
						  NULL,           // primary thread security attributes
						  0,              // handles are inherited
						  0,              // creation flags
						  NULL,           // use parent's environment
						  NULL,     		// use parent's current directory
						  &siStartInfo,   // STARTUPINFO pointer
						  &piProcInfo);   // receives PROCESS_INFORMATION

  if (res)
	{
	  CloseHandle(piProcInfo.hProcess);
	  CloseHandle(piProcInfo.hThread);
	}
}
//---------------------------------------------------------------------------

void StartProcessByExeName(const String &file, const String &params, const String &parent_dir)
{
  PROCESS_INFORMATION     piProcInfo;
  STARTUPINFO             siStartInfo;

  ZeroMemory(&siStartInfo, sizeof(siStartInfo));
  siStartInfo.cb = sizeof(STARTUPINFO);
  ZeroMemory(&piProcInfo, sizeof(piProcInfo));

  int res = CreateProcess(NULL,
						  String(file + L" " + params).c_str(),
						  NULL,           	// process security attributes
						  NULL,           	// primary thread security attributes
						  0,              	// handles are inherited
						  0,              	// creation flags
						  NULL,           	// use parent's environment
						  parent_dir.c_str(), // use parent's current directory
						  &siStartInfo,   	// STARTUPINFO pointer
						  &piProcInfo);   	// receives PROCESS_INFORMATION

  if (res)
	{
	  CloseHandle(piProcInfo.hProcess);
	  CloseHandle(piProcInfo.hThread);
	}
}
//---------------------------------------------------------------------------

String LoadTextFile(String filepath)
{
  String file_text;

  if (FileExists(filepath))
    {
//открываем файл
	  TStringStream *srv_file = new TStringStream("", TEncoding::UTF8, true);
      srv_file->LoadFromFile(filepath);
	  srv_file->Position = 0;
	  file_text = srv_file->ReadString(srv_file->Size);
    }
  else
    file_text = "!no_file!";

//проверим, вдруг текст грузится из файла с кодировкой UTF8-BOM,
//тогда нулевой символ будет ''
//удалим его
  if (file_text[1] == wchar_t(65279))
	file_text.Delete(1, 1);

  return file_text;
}
//---------------------------------------------------------------------------

int LoadFile(String filepath, void *buf)
{
  int file_handle; //хендл файла
  int file_length; //длина
  int bytes_read;  //количество считанных байт

  if (!FileExists(filepath))
    {
//открываем файл
      file_handle = FileOpen(filepath, fmOpenRead);
//получаем длину файла
      file_length = FileSeek(file_handle, 0, 2);
//убедимся, что переданный указатель не присвоен другому значению
      buf = NULL;
//выделяем память под буффер
      buf = (void *) malloc(file_length);
//перемещаем курсор в начало файла
      FileSeek(file_handle, 0, 0);
//получаем файл и заносим в буфер
      bytes_read = FileRead(file_handle, buf, file_length);
//закрываем файл
      FileClose(file_handle);

      free(buf);
    }

  return bytes_read;
}
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
		   //ms->SaveToStream(srv_file);
		   srv_file->CopyFrom(ms, ms->Size);
		 }
	  __finally {delete srv_file; delete ms;}
	}
}
//---------------------------------------------------------------------------

int SaveVectorToFile(std::vector <String> *vecStr, String path)
{
//открываем файл
  try
     {
	   for (UINT i = 0; i < vecStr->size(); i++)
		  {
			AddToFile(path, vecStr->at(i));
		  }
     }
  catch(Exception &e)
     {
	   mfLastError = e.ToString();

	   return RF_ACCESS_ERR;
     }

  return RF_OK;
};
//---------------------------------------------------------------------------

int GetFileCount(String search_dir, String mask)
{
  TSearchRec rec;
  int cnt = 0;

  if (FindFirst(search_dir + "\\" + mask, faAnyFile, rec) == 0)
	{
	  do
		{
		  if ((rec.Name != ".") &&
			  (rec.Name != "..") &&
			  (rec.Attr != faDirectory))
			{
			  cnt++;
			}
		}
	  while (FindNext(rec) == 0);
	}
  else
	return 0;

  FindClose(rec);

  return cnt;
}
//---------------------------------------------------------------------------

int GetFileCountRegEx(String search_dir, String reg_exp)
{
  TSearchRec rec;
  int cnt = 0;

  if (FindFirst(search_dir + "\\*", faAnyFile, rec) == 0)
	{
	  do
		{
		  if ((rec.Name != ".") &&
			  (rec.Name != "..") &&
			  (rec.Attr != faDirectory) &&
			  (TRegEx::IsMatch(rec.Name, reg_exp)))
			{
			  cnt++;
			}
		}
	  while (FindNext(rec) == 0);
	}
  else
	return 0;

  FindClose(rec);

  return cnt;
}
//---------------------------------------------------------------------------

int GetFileCountSubDirs(String search_dir, String mask)
{
  TStringList *DirList = new TStringList();
  String src_name;
  int FileCount = 0;

  try
	 {
	   FileCount = GetFileCount(search_dir, mask);

	   try
		  {
			GetDirList(DirList, search_dir, WITH_FULL_PATH);

            for (int i = 0; i < DirList->Count; i++)
			   {
				 FileCount += GetFileCount(DirList->Strings[i], mask);
			   }
		  }
	   catch (Exception &e)
		  {
			SaveLog("exceptions.log", "GetFileCountSubDirs(): " + e.ToString());
			FileCount = 0;
		  }
	 }
  __finally {delete DirList;}

  return FileCount;
}
//---------------------------------------------------------------------------

int GetFileCountSubDirsRegEx(String search_dir, String reg_exp)
{
  TStringList *DirList = new TStringList();
  String src_name;
  int FileCount = 0;

  try
	 {
	   FileCount = GetFileCountRegEx(search_dir, reg_exp);

	   try
		  {
			GetDirList(DirList, search_dir, WITH_FULL_PATH);

            for (int i = 0; i < DirList->Count; i++)
			   {
				 FileCount += GetFileCountRegEx(DirList->Strings[i], reg_exp);
			   }
		  }
	   catch (Exception &e)
		  {
			SaveLog("exceptions.log", "GetFileCountSubDirsRegEx(): " + e.ToString());
			FileCount = 0;
		  }
	 }
  __finally {delete DirList;}

  return FileCount;
}
//---------------------------------------------------------------------------

void GetFileList(TStringList *list, String path, String mask, bool dirs, bool fullpath)
{
  TSearchRec rec;
  String item;

  if (FindFirst(path + "\\" + mask, faAnyFile, rec) == 0)
	{
	   do
		 {
		   if ((rec.Name != ".") && (rec.Name != ".."))
			 {
			   if (fullpath)
				  item = path + "\\" + rec.Name;
				else
				  item = rec.Name;

			   if ((rec.Attr == faDirectory) && dirs)
				 list->Add(item);
			   else if (rec.Attr != faDirectory)
				 list->Add(item);
			 }
		 }
	   while (FindNext(rec) == 0);
	}
  else
	return;

  FindClose(rec);
};
//---------------------------------------------------------------------------

void GetFileListRegEx(TStringList *list, String path, String reg_expr, bool dirs, bool fullpath)
{
  TSearchRec rec;
  String item;

  if (FindFirst(path + "\\*", faAnyFile, rec) == 0)
	{
	   do
		 {
		   if ((rec.Name != ".") &&
			   (rec.Name != "..") &&
			   (TRegEx::IsMatch(rec.Name, reg_expr)))
			 {
			   if (fullpath)
				  item = path + "\\" + rec.Name;
				else
				  item = rec.Name;

			   if ((rec.Attr == faDirectory) && dirs)
				 list->Add(item);
			   else if (rec.Attr != faDirectory)
				 list->Add(item);
			 }
		 }
	   while (FindNext(rec) == 0);
	}
  else
	return;

  FindClose(rec);
};
//---------------------------------------------------------------------------

void GetDirList(TStringList *list, String path, bool fullpath)
{
  TSearchRec rec;
  String item;

  if (FindFirst(path + "\\*.*", faAnyFile, rec) == 0)
	{
	  do
		{
		  if ((rec.Name != ".") && (rec.Name != ".."))
			{
			  if (rec.Attr & faDirectory)
				{
                  if (fullpath)
					item = path + "\\" + rec.Name;
				  else
					item = rec.Name;

				  list->Add(item);
				}
			}
		}
	  while (FindNext(rec) == 0);
	}
  else
	return;

  FindClose(rec);
};
//---------------------------------------------------------------------------

//возвращает размер указанного файла в байтах
//в случае, если файл не найден возвращает -1
int GetFileSize(String filepath)
{
  int file_size;

  if (!FileExists(filepath))
	return -1;

  TFileStream *fs = new TFileStream(filepath, fmOpenRead);

  try
	 {
	   try
		  {
			file_size = fs->Size;
		  }
       catch (Exception &e)
		  {
	   		file_size = -1;
		  }
	 }
  __finally {if (fs) delete fs;}

  return file_size;
}
//---------------------------------------------------------------------------

bool GetAppVersion(const wchar_t *FileName, int *VerInfo)
{
  if (!FileExists(FileName))
	   return false;

  DWORD FSize = GetFileVersionInfoSize(FileName, NULL); //размер инфы о версии файла

  if (FSize == 0)//функция GetFileVersionInfoSize неуспешна
	return false;

  LPVOID pBlock = (wchar_t*)malloc(FSize); //адрес буфера для ресурсов версии
  GetFileVersionInfo(FileName, NULL, FSize, pBlock); //получаем ресурс информации о версии
  LPVOID MS;
  UINT LS;

  try
	 {
	   VerQueryValueW(pBlock, L"\\", &MS, &LS); //извлекаем информацию из ресурса
	 }
  catch(Exception &e)
	 {
	   SaveLog("exceptions.log", "GetAppVersion(): " + e.ToString());

	   return false;
	 }

  VS_FIXEDFILEINFO FixedFileInfo; //структура с информацией о версии файла
  memmove(&FixedFileInfo, MS, LS); //приводим информацию к структуре

  DWORD FileVersionMS = FixedFileInfo.dwFileVersionMS;
  DWORD FileVersionLS = FixedFileInfo.dwFileVersionLS;

  VerInfo[0] = HIWORD(FileVersionMS); //получаем значения
  VerInfo[1] = LOWORD(FileVersionMS); //и присваиваеи их входному указателю
  VerInfo[2] = HIWORD(FileVersionLS);
  VerInfo[3] = LOWORD(FileVersionLS);

  return true;
}
//---------------------------------------------------------------------------

String GetVersionInString(const wchar_t *FileName)
{
  wchar_t version[30];

  int FileVersion[4] = {0,0,0,0};

  if (GetAppVersion(FileName, FileVersion))
	{
	  swprintf(version, L"%d.%d.%d.%d",
						FileVersion[0],
						FileVersion[1],
						FileVersion[2],
						FileVersion[3]);

      return String(version);
	}
  else
	return "no_data";
}
//---------------------------------------------------------------------------

int CompareVersions(const wchar_t *FileName1, const wchar_t *FileName2)
{
  int FileVersion1[4] = {0,0,0,0};
  int FileVersion2[4] = {0,0,0,0};
  int res = 0;

  if (!GetAppVersion(FileName1, FileVersion1) || !GetAppVersion(FileName2, FileVersion2))
	return -1;

  for (int i = 0; i < 4; i++)
	 {
	   if (FileVersion1[i] > FileVersion2[i])
		 {res = 1; break;}
	   else if (FileVersion1[i] < FileVersion2[i])
		 {res = 2; break;}
	 }

  return res;
}
//---------------------------------------------------------------------------

int CopyAll(String fr_path, String to_path, TStringList *log)
{
  TSearchRec fp;
  int cp_cnt = 0;

  if (FindFirst(fr_path + "\\*.*", faAnyFile | faDirectory, fp) != 0)
	return -1;

  do
	{
	  if ((fp.Name != ".") && (fp.Name != ".."))
		{
		  if (fp.Attr & faDirectory)
			cp_cnt += CopyAll(fr_path + '\\' + fp.Name, to_path, log);
		  else
			{
			  if (log)
				log->Add(fr_path + '\\' + fp.Name + " -> " +
						 to_path + '\\' + fp.Name);

			  try
				 {
				   CopyFile(String(fr_path + '\\' + fp.Name).c_str(),
							String(to_path + '\\' + fp.Name).c_str(), true);
				 }
			  catch (...)
				 {
				   if (log)
					 log->Add("[FAIL]");
				 }

			  if (log)
				log->Add("[OK]");

			  cp_cnt++;
			}
        }

	}
  while(!FindNext(fp));

  FindClose(fp);

  return cp_cnt;
}
//---------------------------------------------------------------------------

int CopyDirs(String source, String destin)
{
  SHFILEOPSTRUCT sf;
  ZeroMemory(&sf, sizeof(SHFILEOPSTRUCT));

  source = source + '\0';
  destin = destin + '\0';

  sf.hwnd = Application->Handle;
  sf.pFrom = source.c_str();
  sf.pTo = destin.c_str();
  sf.wFunc = FO_COPY;
  sf.fFlags = FOF_ALLOWUNDO | FOF_NOCONFIRMMKDIR | FOF_NOCONFIRMATION | FOF_SILENT;

  return SHFileOperation(&sf);
}
//---------------------------------------------------------------------------

void DeleteAllFromDir(String destin)
{
  TSearchRec rec;

  if (FindFirst(destin + "\\*.*", faAnyFile, rec) == 0)
	{
	  while (FindNext(rec) == 0)
		 {
		   if ((rec.Name != ".") && (rec.Name != ".."))
			 {
			   if (rec.Attr & faDirectory)
				 {
				   DeleteAllFromDir(destin + "\\" + rec.Name);
				 }
			   else
				 {
				   DeleteFile(destin + "\\" + rec.Name);
				 }
			 }
		 }
	}

  FindClose(rec);

  if (FindFirst(destin + "\\*.*", faDirectory, rec) == 0)
	{
	  while (FindNext(rec) == 0)
		 {
		   if ((rec.Name != ".") && (rec.Name != ".."))
			 {
			   RemoveDir(destin + "\\" + rec.Name);
			 }
		 }
	}

  FindClose(rec);
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
//если разделитель не встречается, занесем оставшийся текст в list
	   if (text.Pos(delim) == 0)
		 if (text != "")
		   {
			 if ((text != "\n") && (text != "\n\r"))
			   list->Add(text);
	  		}
	 }
  catch (Exception &e)
	 {
	   SaveLog("exceptions.log", "StrToList(): " + e.ToString());
	 }
}
//---------------------------------------------------------------------------

void StrToList(std::vector<String> *list, String text, String delim)
{
  TStringList *l = new TStringList();

  try
	 {
	   StrToList(l, text, delim);
	   list->clear();

	   for (int i = 0; i < l->Count; i++)
		  {
			list->push_back(l->Strings[i]);
		  }
	 }
  __finally {delete l;}
}
//---------------------------------------------------------------------------

String ListToStr(TStringList *list, String delim)
{
  String str_list;

//если разделитель не задан, по умолчанию используется признак конца строки
  if (delim == NULL)
    delim = "\n";

  if (list->Count > 0)
    for (int i = 0; i < list->Count; i++)
      {
        str_list = str_list + list->Strings[i] + delim;
      }
  else
    str_list = "ListToStr: !empty string!";

  return str_list;
}
//---------------------------------------------------------------------------

String GetConfigLine(String conf_file, int index)
{
 String param;

 if (index < 0)
   {
     param = "GetConfigLine: !incorrect index!";
     return param;
   }
    
//создадим лист и загрузим в него содержимое конфиг-файла
 TStringList *conf = new TStringList();

 try
    {
      conf->LoadFromFile(conf_file);

      if (conf->Count != 0)
        {
//если список не пустой и в нем есть строка с указанным индексом
          if (conf->Count > index)
            {
              param = conf->Strings[index];
              param = param.Delete(1, param.Pos("="));
            }
          else
            param = "!no_line!";
        }
      else
        param = "!no_data!";

    }
 __finally
    {
       delete conf;
       conf = NULL;
    }

 return param;
}
//---------------------------------------------------------------------------

String GetConfigLine(String conf_file, String param_name)
{
 int length, pos = 0;
 String param = "^no_line";

 TStringList *conf = new TStringList();

 try
    {
      conf->LoadFromFile(conf_file);

      if (conf->Count != 0)
        {
//если список не пустой и в нем есть строка с указанным индексом
//если список не пуст, построчно сверим имена параметров с исходным
		  int i = 0;

		  while (i < conf->Count)
			{
//вырежем из строки значение параметра, оставив только имя
			  length = conf->Strings[i].Length();
			  pos = conf->Strings[i].Pos("=");

			  if (param_name == conf->Strings[i].SubString(0, length - (length - pos + 1)))
				{
				  param = conf->Strings[i].SubString(pos + 1, length - pos);
				}

			  i++;
			}
		}
      else
        param = "!no_data!";

    }
 __finally
    {
       delete conf;
       conf = NULL;
    }

   return param;
}
//---------------------------------------------------------------------------

int GetConfigLineInd(String conf_file, String param)
{
  int index = -1;
  int length, pos;

//создадим лист и загрузим в него содержимое конфиг-файла
  TStringList *conf = new TStringList();

 try
    {
      conf->LoadFromFile(conf_file);

      if (conf->Count != 0)
        {
//если список не пуст, построчно сверим имена параметров с исходным
          int i = 0;
     
          while (i < conf->Count)
             {
//вырежем из строки значение параметра, оставив только имя        
               length = conf->Strings[i].Length();
               pos = conf->Strings[i].Pos("=");

               conf->Strings[i] = conf->Strings[i].Delete(pos, length - pos);

               if (conf->Strings[i].Pos(param) != 0)
                 {
                   index = i;
                   break;
                 }
            
               i++;
             }
         }
    }     
 __finally
    {
       delete conf;
       conf = NULL;
    }

  return index;
}
//---------------------------------------------------------------------------

bool SetConfigLine(String conf_file, int index, String value)
{
 bool done;

//создадим лист и загрузим в него содержимое конфиг-файла
 TStringList *conf = new TStringList();
 
 try
    {
      conf->LoadFromFile(conf_file);

      if (conf->Count != 0)
        {
//если список не пустой и в нем есть строка с указанным индексом
          if (conf->Count > index)
            {
              String str = conf->Strings[index];
              int pos = str.Pos("=") + 1;
//уберем старый параметр
              str = str.Delete(pos, str.Length() - pos + 1);
//внесем новый
              conf->Delete(index);
              conf->Insert(index, str + value);
              conf->SaveToFile(conf_file);
              done = true;
            }
          else
            done = false;
        }
      else
        done = false;
    }
 __finally
    {
       delete conf;
       conf = NULL;
	}

 return done;
}
//---------------------------------------------------------------------------

bool SetConfigLine(String conf_file, String param, String value)
{
  return SetConfigLine(conf_file,
                       GetConfigLineInd(conf_file, param),
                       value); 
}
//---------------------------------------------------------------------------

bool AddConfigLine(String conf_file, String param, String value)
{
  bool result;

  try
	 {
	   AddToFile(conf_file, param + "=" + value);
       AddToFile(conf_file, "\r\n");
	   result = true;
	 }
  catch (Exception &e)
	 {
	   SaveLog("exceptions.log", "AddConfigLine(): " + e.ToString());
	   result = false;
	 }

  return result;
}
//---------------------------------------------------------------------------

bool RemConfigLine(String conf_file, String param)
{
  bool result = false;
  int index = -1;

//создадим лист и загрузим в него содержимое конфиг-файла
  TStringList *conf = new TStringList();

 try
    {
	  conf->LoadFromFile(conf_file);

      if (conf->Count != 0)
		{
//если список не пуст, узнаем индекс нужного параметра и удалим строку
		  index = GetConfigLineInd(conf_file, param);

		  if (index > -1)
			{
			  try
				 {
				   conf->Delete(index);
				   conf->SaveToFile(conf_file);
				   result = true;
				 }
			  catch (Exception &e)
				 {
				   SaveLog("exceptions.log", "RemConfigLine(): " + e.ToString());
				   result = false;
				 }
			}
		}
    }
 __finally
    {
	  delete conf;
      conf = NULL;
	}

  return result;
}
//---------------------------------------------------------------------------

int ReadParameter(String conf_file, String param_name, void *target, TranslateType ttype)
{
  String val;

  if (!target)
	{LastReadParamResult = RP_E_INV_PTR; return LastReadParamResult;}
  else if (!FileExists(conf_file))
	{LastReadParamResult = RP_E_NO_FILE; return LastReadParamResult;}
  else if ("^no_line" == (val = GetConfigLine(conf_file, param_name)))
	{LastReadParamResult = RP_E_NO_LINE; return LastReadParamResult;}
  else
	{
	  switch (ttype)
		{
		  case TT_TO_STR:
						{
						  String *param;

						  try
							 {
							   param = (String*)target;
							 }
						  catch (Exception &e)
							 {
							   LastReadParamResult = RP_E_INV_TYPE;
							   mfLastError = e.ToString();
							   return LastReadParamResult;
							 }

						  try
							 {
							   param->operator =(val);
							 }
						  catch (Exception &e)
							 {
							   LastReadParamResult = RP_E_TO_STR;
							   mfLastError = e.ToString();
							   return LastReadParamResult;
							 }

						  LastReadParamResult = RP_OK;
						  return LastReadParamResult;
						}
		  case TT_TO_INT:
						{
						  int *param;

						  try
							 {
							   param = (int*)target;
							 }
						  catch (Exception &e)
							 {
							   LastReadParamResult = RP_E_INV_TYPE;
							   mfLastError = e.ToString();
							   return LastReadParamResult;
							 }

						  try
							 {
							   *param = val.ToInt();
							 }
						  catch (Exception &e)
							 {
							   LastReadParamResult = RP_E_TO_INT;
							   mfLastError = e.ToString();
							   return LastReadParamResult;
							 }

						  LastReadParamResult = RP_OK;
						  return LastReadParamResult;
						}
          case TT_TO_BOOL:
						{
						  bool *param;

						  try
							 {
							   param = (bool*)target;
							 }
						  catch (Exception &e)
							 {
							   LastReadParamResult = RP_E_INV_TYPE;
							   mfLastError = e.ToString();
							   return LastReadParamResult;
							 }

						  try
							 {
							   *param = (bool)val.ToInt();
							 }
						  catch (Exception &e)
							 {
							   LastReadParamResult = RP_E_TO_BOOL;
							   mfLastError = e.ToString();
							   return LastReadParamResult;
							 }

						  LastReadParamResult = RP_OK;
						  return LastReadParamResult;
						}
		   default:
				{
				  LastReadParamResult = RP_E_UNC_TYPE;
				  return LastReadParamResult;
				}
        }
	}

}
//---------------------------------------------------------------------------

const wchar_t *GetReadParamResult(ReadParamExitCodes code)
{
  switch (code)
	{
	  case RP_OK: return L"Read OK\n";
	  case RP_E_TO_STR: return L"Error conversion in String\n";
	  case RP_E_TO_INT: return L"Error conversion in integer\n";
	  case RP_E_TO_BOOL: return L"Error conversion in bollean\n";
	  case RP_E_INV_PTR: return L"Invalid target pointer\n";
	  case RP_E_NO_LINE: return L"No line in config file\n";
	  case RP_E_NO_FILE: return L"Config file not exist\n";
	  case RP_E_INV_TYPE: return L"Selected type not equal to target type\n";
	  case RP_E_UNC_TYPE: return L"Unknown type\n";
	  default: return L"Unknown error\n";
	}
}
//---------------------------------------------------------------------------

const wchar_t *GetLastReadParamResult()
{
  switch (LastReadParamResult)
	{
	  case RP_OK: return L"Read OK\n";
	  case RP_E_TO_STR: return L"Error conversion in String\n";
	  case RP_E_TO_INT: return L"Error conversion in integer\n";
	  case RP_E_TO_BOOL: return L"Error conversion in bollean\n";
	  case RP_E_INV_PTR: return L"Invalid target pointer\n";
	  case RP_E_NO_LINE: return L"No line in config file\n";
	  case RP_E_NO_FILE: return L"Config file not exist\n";
	  case RP_E_INV_TYPE: return L"Selected type not equal to target type\n";
	  case RP_E_UNC_TYPE: return L"Unknown type\n";
	  default: return L"Unknown error\n";
	}
}
//---------------------------------------------------------------------------

FILEINFO GetFileInfo(String path)
{
  FILEINFO INF;
  String file, ext, name;
  int file_handle;
  file = path;

//открываем файл
  file_handle = FileOpen(path, fmOpenRead);
//получаем длину файла и заносим в структуру
  INF.length = FileSeek(file_handle, 0, 2);
//закрываем файл
  FileClose(file_handle);

//определяем имя файла и расширение
  int pos;

  while (file.Pos("\\") != 0)
    {
      pos = file.Pos("\\");
    };

//обрезаем строку по последний знак "\"
  file = file.Delete(1, pos + 1);

  INF.name = file;
  pos = file.Pos(".");
  INF.name = INF.name.Delete(pos, INF.name.Length() - pos);

  INF.ext = file.Delete(1, pos + 1);

//получаем контрольную сумму
  //INF.crc = GetCRC(path);

  return INF;
}
//---------------------------------------------------------------------------

TDateTime GetFileDateTime(String file)
{
  OFSTRUCT of;
  FILETIME ft;
  SYSTEMTIME lf;
  HANDLE hFile = (HANDLE)OpenFile(AnsiString(file).c_str(), &of, OF_READ);

  GetFileTime(hFile, NULL, NULL, &ft);
  FileTimeToSystemTime(&ft, &lf);

  TDateTime fdate = TDateTime(lf.wYear,
							  lf.wMonth,
							  lf.wDay,
							  lf.wHour,
							  lf.wMinute,
							  lf.wSecond,
							  lf.wMilliseconds);

  CloseHandle(hFile);

  return fdate;
}
//---------------------------------------------------------------------------

void ShowLog(String message, TListBox *output)
{
  output->Items->Add("["
                  + DateToStr(Date())
                  +" "
                  +TimeToStr(Time())
                  + "]"
                  +" : "
                  +message);
}
//---------------------------------------------------------------------------

void ShowLog(String message, TMemo *output)
{
  output->Lines->Add("["
					 + DateToStr(Date())
					 + " "
					 + TimeToStr(Time())
					 + "]"
					 + " : "
				  	 + message);
}
//---------------------------------------------------------------------------

void SaveLog(String file, String rec)
{
//добавим к тексту дату и время добавления
  rec = "["
        + DateToStr(Date())
        + " "
        + TimeToStr(Time())
        + "]"
        + " : "
		+ rec
		+ "\r\n";
        
//сохраняем в файл
  AddToFile(file, rec);
}
//---------------------------------------------------------------------------

void SaveLog(String text, TStringList *log)
{
  String msg = "["
				+ DateToStr(Date())
				+ " "
				+ TimeToStr(Time())
				+ "]"
				+ " : "
				+ text;

  if (log)
  	log->Add(msg);
}
//---------------------------------------------------------------------------

bool IsTextIP(String text)
{
  bool result;
  int i, count;

  while (text.Pos(".") != 0)
    {
      i = text.Pos(".");
      text = text.Delete(1, i);
      count++;
    }

  if (count == 3)
    result = true;
  else if (count == 0)
    result = false;
  else
	 MessageBox(NULL,
				L"Введенное значение не является допустимым IP-адресом!",
				L"Ошибка",
				MB_OK | MB_ICONERROR);

  return result;  
}
//---------------------------------------------------------------------------

bool ExtractHostPort(String conn_str, String &host, int &port)
{
  TStringList *recp = new TStringList();
  bool result;

  try
	 {
	   try
		  {
			StrToList(recp, conn_str, ":");
			host = recp->Strings[0];
			port = recp->Strings[1].ToInt();
			result = true;
		  }
	   catch (Exception &e)
		  {
			SaveLog("exceptions.log", "ExtractHostPort(): " + e.ToString());
			result = false;
		  }
	 }
  __finally {delete recp;}

  return result;
}
//---------------------------------------------------------------------------

bool IsCorrect (int val, String type, int criteria1, int criteria2)
{
  bool success;

  if (type == "percent")
    {
      if (val >= 100)
        success = true;
    }

  else if (type == "inrange")
      {
        if ((val >= criteria1) && (val <= criteria2))
          success = true;
        else
          success = false;
      }

   else if (type == "morethan")
      {
        if (val > criteria1)
          success = true;
        else
          success = false;
      }

   else if (type == "lessthan")
      {
        if (val < criteria1)
          success = true;
        else
          success = false;
      }

   return success;   
}

//---------------------------------------------------------------------------

/*загружает в имэйдж джпег-картинку из файла
void LoadJPEGToImage(TImage *target, String file)
{
  if (FileExists(file))
	{
	  if (target == NULL)
		ShowMessage("LoadJPEGToImage - !null pointer!");
	  else
		{
		  TJPEGImage *new_image = new TJPEGImage();
		  new_image->LoadFromFile(file);
		  target->Picture->Assign(new_image);

		  delete new_image;
		  new_image = NULL;
		}
	}
  else
	ShowMessage("LoadJPEGToImage - !no_file!");
};*/
//---------------------------------------------------------------------------

String ParseString(const String &main_str,
						String target_str,
                        const String insert_str)
{
  String result = main_str;
  int pos = 0, count = 0;

  while (result.Pos(target_str) != 0)
    {
      pos = result.Pos(target_str);
      count = target_str.Length();
      result = result.Delete(pos, count);
      result = result.Insert(insert_str, pos);
	}

  return result;
}                       
//---------------------------------------------------------------------------

BOOL CALLBACK EnumWindowsProc(HWND hwnd, LPARAM lParam)
{
  if (!HWND_LST)
	return FALSE;

  DWORD dwPID;

  GetWindowThreadProcessId(hwnd, &dwPID);

  if (dwPID == (DWORD)lParam)
	{
	  HWND_LST->push_back(hwnd);
	}

  return TRUE;
}
//---------------------------------------------------------------------------

HWND FindHandleByName(const wchar_t *wnd_name)
{
  HWND wnd = FindWindowW(NULL, wnd_name);

  if (wnd)
	return wnd;
  else
    return NULL;
}
//---------------------------------------------------------------------------

DWORD FindPIDByHandle(HWND h)
{
  DWORD procpid;

  if (h)
	{
	  GetWindowThreadProcessId(h, &procpid);
	  HANDLE proc = OpenProcess(PROCESS_VM_READ, 0, procpid);

	  if (proc)
		{
		  CloseHandle(proc);

		  return procpid;
		}
	  else
		return -1;
	}
  else
	return 0;
}
//---------------------------------------------------------------------------

HWND FindHandleByPID(DWORD pid)
{
  std::vector<HWND> handles;

  HWND_LST = &handles;

  EnumWindows((WNDENUMPROC)EnumWindowsProc, pid);

  if (handles.size() > 0)
	return handles[0];
  else
	return 0;
}
//---------------------------------------------------------------------------

int FindAllHandlesByPID(DWORD pid, std::vector<HWND> *handle_list)
{
  if (!handle_list)
	return -1;

  HWND_LST = handle_list;

  EnumWindows((WNDENUMPROC)EnumWindowsProc, pid);

  return handle_list->size();
}
//---------------------------------------------------------------------------

DWORD GetProcessByExeName(wchar_t *name)
{
  DWORD Pid;
  PROCESSENTRY32 pe32;
  pe32.dwSize = sizeof(PROCESSENTRY32);

  HANDLE hProcessSnap = CreateToolhelp32Snapshot(TH32CS_SNAPALL, NULL);

  if (hProcessSnap == INVALID_HANDLE_VALUE)
	{
	  return 0;
	}

  if (Process32First(hProcessSnap, &pe32))
	{
	  do
		{
		  if (StrCmpW(pe32.szExeFile, name) == 0)
			{
			  CloseHandle(hProcessSnap);

			  return pe32.th32ProcessID;
			}
		}
	  while (Process32Next(hProcessSnap, &pe32));
	}

  CloseHandle(hProcessSnap);

  return 0;
}
//-------------------------------------------------------------------------

bool FindAllProcesessByExeName(const wchar_t *name, TStringList *pid_list)
{
  HANDLE Hnd = CreateToolhelp32Snapshot(TH32CS_SNAPPROCESS, 0);

  if (Hnd == INVALID_HANDLE_VALUE)
	return false;

  PROCESSENTRY32 ProcessEntry;
  ProcessEntry.dwSize = sizeof(ProcessEntry);
  bool fnd = Process32First(Hnd, &ProcessEntry);

  while (fnd)
   {
	 if (wcscmp(ProcessEntry.szExeFile, name) == 0)
	   pid_list->Add(ProcessEntry.th32ProcessID);

     fnd = Process32Next(Hnd, &ProcessEntry);
   }

  return true;
}
//---------------------------------------------------------------------------

void ErrorExit(LPTSTR lpszFunction)
{
    // Retrieve the system error message for the last-error code

    LPVOID lpMsgBuf;
    LPVOID lpDisplayBuf;
    DWORD dw = GetLastError();

    FormatMessage(
        FORMAT_MESSAGE_ALLOCATE_BUFFER |
        FORMAT_MESSAGE_FROM_SYSTEM |
        FORMAT_MESSAGE_IGNORE_INSERTS,
        NULL,
        dw,
        MAKELANGID(LANG_NEUTRAL, SUBLANG_DEFAULT),
        (LPTSTR) &lpMsgBuf,
        0, NULL );

    // Display the error message and exit the process

    lpDisplayBuf = (LPVOID)LocalAlloc(LMEM_ZEROINIT,
        (lstrlen((LPCTSTR)lpMsgBuf) + lstrlen((LPCTSTR)lpszFunction) + 40) * sizeof(TCHAR));
    StringCchPrintf((LPTSTR)lpDisplayBuf,
        LocalSize(lpDisplayBuf) / sizeof(TCHAR),
        TEXT("%s failed with error %d: %s"),
        lpszFunction, dw, lpMsgBuf);
    MessageBox(NULL, (LPCTSTR)lpDisplayBuf, TEXT("Error"), MB_OK);

    LocalFree(lpMsgBuf);
    LocalFree(lpDisplayBuf);
    ExitProcess(dw);
}
//---------------------------------------------------------------------------
