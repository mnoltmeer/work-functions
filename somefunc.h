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

#ifndef SOMEFUNC_H_INCLUDED
#define SOMEFUNC_H_INCLUDED

#include <windows.h>
#include <string.h>
#include <fstream>
#include <sstream>
#include <vector>
#include <iterator>
#include <time.h>

#define UINT unsigned int
#define CHAROEM 0
#define OEMCHAR 1

#define DELIMEND true    //в конце разбиваемой строки есть символ-разделитель
#define NODELIMEND false //в конце разбиваемой строки нет символа-разделителя

#define STR_CONV_BUF_SIZE 12000 // the largest string you may have to convert

#define OPSYMCNT sizeof(OperSymb) / sizeof(char) //кол-во символов операций

static char OperSymb[] = {'=', '+', '-', '*', '/'};
static char NumSymb[10] = {'0', '1', '2', '3', '4', '5', '6', '7', '8', '9'};

using namespace std;

inline wchar_t* UnicodeOf(const char* c)
{
    static wchar_t w[STR_CONV_BUF_SIZE];
	memset(w, 0, sizeof(w));
	MultiByteToWideChar(CP_ACP, MB_PRECOMPOSED, c, strlen(c), w, STR_CONV_BUF_SIZE);
	return(w);
}
//---------------------------------------------------------------------------

inline char* AnsiOf(const wchar_t* w)
{
	static char c[STR_CONV_BUF_SIZE];
	memset(c, 0, sizeof(c));
	WideCharToMultiByte(CP_ACP, 1024, w, wcslen(w), c, STR_CONV_BUF_SIZE, NULL, NULL);
	return(c);
}
//---------------------------------------------------------------------------

//ищет в параметрах main() указаное имя параметра
inline int IsInParams(char **prms, int pcnt, const char *name)
{
  for (int i = 0; i < pcnt; i++)
    {
      if (0 == strcmp(name, prms[i]))
        return i;
    }

  return -1;
}
//---------------------------------------------------------------------------

inline const char *GetCurrDateToStr()
{
  time_t rawtime;
  struct tm *timeinfo;

  time (&rawtime);
  timeinfo = localtime(&rawtime);

  static char dt[80];

  strftime(dt, sizeof(dt), "%Y-%m-%d", timeinfo);

  return dt;
}
//---------------------------------------------------------------------------

inline const char *GetCurrTimeToStr()
{
  time_t rawtime;
  struct tm *timeinfo;

  time (&rawtime);
  timeinfo = localtime(&rawtime);

  static char tt[80];

  strftime(tt, sizeof(tt), "%H:%M:%S", timeinfo);

  return tt;
}
//---------------------------------------------------------------------------

//проверяет, является ли symb символом операции, возвращает индекс символа в массиве спец. символов
//или -1, если совпадений не было
inline int IsOper(char symb)
{
  for (UINT i = 0; i < OPSYMCNT; i++)
    {
      if (symb == OperSymb[i])
        return i;
    }

  return -1;
}
//-------------------------------------------------------------------------------------------------

inline int IsOperW(wchar_t symb)
{
  for (UINT i = 0; i < OPSYMCNT; i++)
    {
      if (symb == (wchar_t)OperSymb[i])
        return i;
    }

  return -1;
}
//-------------------------------------------------------------------------------------------------

//то же, но для цифр
inline int IsNum(char symb)
{
  for (int i = 0; i < 10; i++)
    {
      if (symb == NumSymb[i])
        return i;
    }

  return -1;
}
//-------------------------------------------------------------------------------------------------

//то же, но для цифр
inline int IsNumW(wchar_t symb)
{
  for (int i = 0; i < 10; i++)
    {
      if (symb == (wchar_t)NumSymb[i])
        return i;
    }

  return -1;
}
//-------------------------------------------------------------------------------------------------


//вспомогательная ф-я, проверяет, что в строке содержится число
//возвращает 0 - если integer
//1 - float
//-1 - строка не является float
//-2 - недопустимые символы в строке
//признаком float считается как точка, так и запятая
inline int IsStrNum(const char *somestring)
{
  int result;
  UINT cmcnt = 0;
  UINT flt = 0;
  UINT i = 0, j = 0;

  if ('-' == somestring[0])
    {
      result = 0;
      j = 1;
    }

  for (i = j; i < strlen(somestring); i++)
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
         result = 0;
       else if ((',' == somestring[i]) || ('.' == somestring[i]))
         {
           flt = 1;

           if (cmcnt > 1)
             return -1;

           cmcnt++;
         }
       else
         return -2;
     }

  if (flt)
    return 1;
  else
    return result;
}
//-------------------------------------------------------------------------------------------------

inline int IsStrNumW(const wchar_t *somestring)
{
  int result;
  UINT cmcnt = 0;
  UINT flt = 0;
  UINT i = 0, j = 0;

  if ((wchar_t)'-' == somestring[0])
    {
      result = 0;
      j = 1;
    }

  for (i = j; i < wcslen(somestring); i++)
     {
       if (((wchar_t)'0' == somestring[i]) ||
           ((wchar_t)'1' == somestring[i]) ||
           ((wchar_t)'2' == somestring[i]) ||
           ((wchar_t)'3' == somestring[i]) ||
           ((wchar_t)'4' == somestring[i]) ||
           ((wchar_t)'5' == somestring[i]) ||
           ((wchar_t)'6' == somestring[i]) ||
           ((wchar_t)'7' == somestring[i]) ||
           ((wchar_t)'8' == somestring[i]) ||
           ((wchar_t)'9' == somestring[i]))
         result = 0;
       else if (((wchar_t)',' == somestring[i]) || ((wchar_t)'.' == somestring[i]))
         {
           flt = 1;

           if (cmcnt > 1)
             return -1;

           cmcnt++;
         }
       else
         return -2;
     }

  if (flt)
    return 1;
  else
    return result;
}
//-------------------------------------------------------------------------------------------------


//парсит строку заменяя в ней подстроку target_str на строку insert_str
inline string ParseString(const string main_str, string target_str, const string insert_str)
{
  string result = main_str;
  UINT pos, scount;

  while (result.find(target_str) != string::npos)
    {
      pos = result.find(target_str);
      scount = target_str.length();
      result.erase(pos, scount);
      result = result.insert(pos, insert_str);
    }

  //MessageBox(NULL, result.c_str(), "ParseString()", MB_OK);

  return result;
};
//---------------------------------------------------------------------------

//парсит строку заменяя в ней подстроку target_str на строку insert_str
inline wstring ParseStringW(const wstring main_str, wstring target_str, const wstring insert_str)
{
  wstring result = main_str;
  UINT pos, scount;

  while (result.find(target_str) != wstring::npos)
    {
      pos = result.find(target_str);
      scount = target_str.length();
      result.erase(pos, scount);
      result = result.insert(pos, insert_str);
    }

  return result;
};
//---------------------------------------------------------------------------


//выводит в вектор список файлов, находящихся в указанной папке
//ext - тип (расширение) файла
inline void GetFileList(vector <string> *vecList, string path, string ext)
{
  WIN32_FIND_DATA FindFileData;
  HANDLE hf;
  path = path + "\\*." + ext;

  hf = FindFirstFile(path.c_str(), &FindFileData);

  if (hf != INVALID_HANDLE_VALUE)
    {
        do
          {
            if (FindFileData.dwFileAttributes != FILE_ATTRIBUTE_DIRECTORY)
              vecList->push_back(string(FindFileData.cFileName));
          }
        while(FindNextFile(hf, &FindFileData) != 0);

        FindClose(hf);
    }

};
//---------------------------------------------------------------------------

//ф-я для вывода в консоль сообщений на кириллице
inline void CPTransOut(UINT mode, const char *in_str, UINT str_len)
{
  char *out_str = new char[str_len];

  switch (mode)
     {
       case CHAROEM:
          {
			CharToOemBuff(in_str, out_str, str_len);
            printf(out_str);
            delete[] out_str;
            out_str = NULL;
            break;
          }

       case OEMCHAR:
          {
            OemToCharBuff(in_str, out_str, str_len);
            printf(out_str);
            delete[] out_str;
            out_str = NULL;
            break;
          }
     }
};
//---------------------------------------------------------------------------

inline bool LoadFileToVector(vector<string> *vecStr, string path)
{
  char str[128];

//открываем файл
  try
     {
       char tmp[4096];
       ifstream fin;

       fin.open(path.c_str(), ios_base::in);

       if (!fin.is_open())
         {
           sprintf(str, "Не удается открыть файл %s!\n", path.c_str());
           CPTransOut(CHAROEM, str, strlen(str) + 1);

           return false;
         }
       else
         {
           fin.seekg(0);

           while (!fin.eof())
             {
               fin.getline(tmp, sizeof(tmp));

               if (fin.fail())
                 {
                   sprintf(str, "Ошибка при чтении файла %s\n", path.c_str());
                   CPTransOut(CHAROEM, str, strlen(str) + 1);

                   return false;
                 }

               vecStr->push_back(string(tmp));
             }

           fin.close();

           return true;
         }
     }
  catch(...)
     {
       sprintf(str, "Ошибка при обращении к файлу %s\n", path.c_str());
       CPTransOut(CHAROEM, str, strlen(str) + 1);

       return false;
     }
};
//---------------------------------------------------------------------------

inline bool LoadFileToVectorW(vector<wstring> *vecStr, wstring path)
{
  wchar_t str[128];

//открываем файл
  try
     {
       wchar_t tmp[4096];
       wifstream fin;

       fin.open(AnsiOf(path.c_str()), ios_base::in);

       if (!fin.is_open())
         {
           swprintf(str, L"Не удается открыть файл %s!\n", path.c_str());
           CPTransOut(CHAROEM, AnsiOf(str), strlen(AnsiOf(str)) + 1);

           return false;
         }
       else
         {
           fin.seekg(0);

           while (!fin.eof())
             {
               fin.getline(tmp, sizeof(tmp));

               if (fin.fail())
                 {
                   swprintf(str, L"Ошибка при чтении файла %s\n", path.c_str());
                   CPTransOut(CHAROEM, AnsiOf(str), strlen(AnsiOf(str)) + 1);

                   return false;
                 }

               vecStr->push_back(wstring(tmp));
             }

           fin.close();

           return true;
         }
     }
  catch(exception &e)
     {
       swprintf(str, L"Ошибка при обращении к файлу %s : %s\n", path.c_str(), e.what());
       CPTransOut(CHAROEM, AnsiOf(str), strlen(AnsiOf(str)) + 1);

       return false;
     }
};
//---------------------------------------------------------------------------

inline string &LoadFileToString(string path)
{
  char str[128];
  static string outstr;

  outstr = "";

//открываем файл
  try
     {
       char tmp[8192];
       ifstream fin;

       fin.open(path.c_str(), ios_base::in);

       if (!fin.is_open())
         {
           sprintf(str, "Не удается открыть файл %s!\n", path.c_str());
           CPTransOut(CHAROEM, str, strlen(str) + 1);
           outstr = "-err-";

           return outstr;
         }
       else
         {
           fin.seekg(0);

           while (!fin.eof())
             {
               fin.getline(tmp, sizeof(tmp));

               if (fin.fail())
                 {
                   sprintf(str, "Ошибка при чтении файла %s\n", path.c_str());
                   CPTransOut(CHAROEM, str, strlen(str) + 1);
                   outstr = "-err-";

                   return outstr;
                 }

               outstr = outstr.append(tmp);
             }

           fin.close();

           return outstr;
         }
     }
  catch(exception &e)
     {
       sprintf(str, "Ошибка при обращении к файлу %s : %s\n", path.c_str(), e.what());
       CPTransOut(CHAROEM, str, strlen(str) + 1);
       outstr = "-err-";

       return outstr;
     }
};
//---------------------------------------------------------------------------

inline wstring &LoadFileToStringW(wstring path)
{
  wchar_t str[128];
  static wstring outstr;

  outstr = L"";

//открываем файл
  try
     {
       wchar_t tmp[8192];
       wifstream fin;

       fin.open(AnsiOf(path.c_str()), ios_base::in);

       if (!fin.is_open())
         {
           swprintf(str, L"Не удается открыть файл %s!\n", path.c_str());
           CPTransOut(CHAROEM, AnsiOf(str), strlen(AnsiOf(str)) + 1);
           outstr = L"-err-";

           return outstr;
         }
       else
         {
           fin.seekg(0);

           while (!fin.eof())
             {
               fin.getline(tmp, sizeof(tmp));

               if (fin.fail())
                 {
                   swprintf(str, L"Ошибка при чтении файла %s\n", path.c_str());
                   CPTransOut(CHAROEM, AnsiOf(str), strlen(AnsiOf(str)) + 1);
                   outstr = L"-err-";

                   return outstr;
                 }

               outstr = outstr.append(tmp);
             }

           fin.close();

           return outstr;
         }
     }
  catch(exception &e)
     {
       swprintf(str, L"Ошибка при обращении к файлу %s : %s\n", path.c_str(), e.what());
       CPTransOut(CHAROEM, AnsiOf(str), strlen(AnsiOf(str)) + 1);
       outstr = L"-err-";

       return outstr;
     }
};
//---------------------------------------------------------------------------

inline const char *ReadFileToStr(const char *path)
{
  static string outstr;
  stringstream in;
  outstr.clear();

//открываем файл
  try
     {
       ifstream fin;

       fin.open(path, ios_base::in);

       if (!fin.is_open())
         {
           return "-err-";
         }
       else
         {
           char ch;

           while (!fin.eof())
             {
               fin.get(ch);
               in << ch;
             }

           fin.close();
           outstr = in.str();
           outstr.erase(outstr.length() - 1, 1);
           return outstr.c_str();
         }
     }
  catch(...)
     {
       return "-err-";
     }
};
//---------------------------------------------------------------------------

inline const wchar_t *ReadFileToStrW(const wchar_t *path)
{
  static wstring outstr;
  wstringstream in;
  outstr.clear();

//открываем файл
  try
     {
       wifstream fin;

       fin.open(AnsiOf(path), ios_base::in);

       if (!fin.is_open())
         {
           return L"-err-";
         }
       else
         {
           wchar_t ch;

           while (!fin.eof())
             {
               fin.get(ch);
               in << ch;
             }

           fin.close();
           outstr = in.str();
           outstr.erase(outstr.length() - 1, 1);
           return outstr.c_str();
         }
     }
  catch(...)
     {
       return L"-err-";
     }
};
//---------------------------------------------------------------------------

inline void *ReadFileToBuf(const char *path)
{
  stringstream in;
  static void *buf;

  static byte *res;

//открываем файл
  try
     {
       ifstream fin;
       ofstream s;

       fin.open(path, ios_base::in | ios_base::binary);

       fin.seekg(ios_base::beg);
       fin.seekg(ios_base::end);
       ULONG sz = fin.tellg();
       res = new byte[sz];

       if (fin.is_open())
         {
            fin.get((char*)res, sz);
           /*while (!fin.eof())
             {
               fin >> res;
             }*/

           fin.close();

           s.open("D:\\test.exe", ios_base::binary);
           s.write((char*)res, sz);
           s.close();

           in << "size buf = " << sizeof(buf);

MessageBox(NULL, in.str().c_str(), "", MB_OK);
           return buf;
         }
     }
  catch(...)
     {
       return NULL;
     }
};
//---------------------------------------------------------------------------

inline bool SaveStrToFile(const char *msg, const char *path)
{
  char str[128];

//открываем файл
  try
     {
       ofstream fout(path, ios_base::app | ios_base::ate);

       if (fout.fail())
         {
           sprintf(str, "Не удается открыть файл %s!\n", path);
           CPTransOut(CHAROEM, str, strlen(str) + 1);

           return false;
         }
       else
         {
           fout << msg << endl;

           if (fout.fail())
             {
               sprintf(str, "Ошибка при чтении файла %s\n", path);
               CPTransOut(CHAROEM, str, strlen(str) + 1);

               return false;
             }

           fout.close();
           return true;
         }
     }
  catch(...)
     {
       sprintf(str, "Ошибка при обращении к файлу %s\n", path);
       CPTransOut(CHAROEM, str, strlen(str) + 1);
     }

  return false;
};
//---------------------------------------------------------------------------

inline bool SaveStrToFileW(const wchar_t *msg, const wchar_t *path)
{
  wchar_t str[128];

//открываем файл
  try
     {
       wofstream fout(AnsiOf(path), ios_base::app | ios_base::ate);

       if (fout.fail())
         {
           swprintf(str, L"Не удается открыть файл %s!\n", path);
           CPTransOut(CHAROEM, AnsiOf(str), strlen(AnsiOf(str)) + 1);

           return false;
         }
       else
         {
           fout << msg << "\n";

           if (fout.fail())
             {
               swprintf(str, L"Ошибка при записи в файл %s\n", path);
               CPTransOut(CHAROEM, AnsiOf(str), strlen(AnsiOf(str)) + 1);

               return false;
             }

           fout.close();
           return true;
         }
     }
  catch(...)
     {
       swprintf(str, L"Ошибка при обращении к файлу %s\n", path);
       CPTransOut(CHAROEM, AnsiOf(str), strlen(AnsiOf(str)) + 1);
     }

  return false;
};
//---------------------------------------------------------------------------

inline bool SaveVectorToFile(vector<string> *vecStr, string path)
{
  char str[128];

//открываем файл
  try
     {
       ofstream fout;
       fout.open(path.c_str(), ios_base::trunc | ios_base::out);

       if (!fout.is_open())
         {
           sprintf(str, "Не удается открыть файл %s!\n", path.c_str());
           CPTransOut(CHAROEM, str, strlen(str) + 1);

           return false;
         }
       else
         {
           fout.seekp(0);

           for (UINT i = 0; i < vecStr->size(); i++)
             {
               fout << vecStr->at(i) << endl;

               if (fout.fail())
                 {
                   sprintf(str, "Ошибка при чтении файла %s\n", path.c_str());
                   CPTransOut(CHAROEM, str, strlen(str) + 1);

                   return false;
                 }
             }

           fout.close();
           return true;
         }
     }
  catch(exception &e)
     {
       sprintf(str, "Ошибка при обращении к файлу %s : %s\n", path.c_str(), e.what());
       CPTransOut(CHAROEM, str, strlen(str) + 1);
     }

  return false;
};
//---------------------------------------------------------------------------

inline bool SaveVectorToFileW(vector<wstring> *vecStr, wstring path)
{
  wchar_t str[128];

//открываем файл
  try
     {
       wofstream fout;
       fout.open(AnsiOf(path.c_str()), ios_base::trunc | ios_base::out);

       if (!fout.is_open())
         {
           swprintf(str, L"Не удается открыть файл %s!\n", path.c_str());
           CPTransOut(CHAROEM, AnsiOf(str), strlen(AnsiOf(str)) + 1);

           return false;
         }
       else
         {
           fout.seekp(0);

           for (UINT i = 0; i < vecStr->size(); i++)
             {
               fout << vecStr->at(i) << endl;

               if (fout.fail())
                 {
                   swprintf(str, L"Ошибка при чтении файла %s\n", path.c_str());
                   CPTransOut(CHAROEM, AnsiOf(str), strlen(AnsiOf(str)) + 1);

                   return false;
                 }
             }

           fout.close();
           return true;
         }
     }
  catch(exception &e)
     {
       swprintf(str, L"Ошибка при обращении к файлу %s : %s\n", path.c_str(), e.what());
       CPTransOut(CHAROEM, AnsiOf(str), strlen(AnsiOf(str)) + 1);
     }

  return false;
};
//---------------------------------------------------------------------------

inline void SaveLog(const char *file, const char *rec)
{
  char msg[4096];
//добавим к тексту дату и время добавления
  sprintf(msg,"[%s | %s]: %s", GetCurrDateToStr(), GetCurrTimeToStr(), rec);

  SaveStrToFile(msg, file);
}
//---------------------------------------------------------------------------

inline void SaveLogW(const wchar_t *file, const wchar_t *rec)
{
  wchar_t msg[4096];
  std::wstring dt = UnicodeOf(GetCurrDateToStr()), tm = UnicodeOf(GetCurrTimeToStr());
//добавим к тексту дату и время добавления
  swprintf(msg, L"[%s | %s]: %s", dt.c_str(), tm.c_str(), rec);

  SaveStrToFileW(msg, file);
}
//---------------------------------------------------------------------------

//ф-я принимает в качестве параметра указатель на вектор, в который
//сохраняет части строки отделенные символом-разделителем
//isdelimend - показывает завершает ли строку символ-разделитель
inline void StrToList(vector <string> *vecList, string text, string delim, bool isdelimend)
{
 UINT pos;
 string str;

 vecList->clear();

//если разделитель не задан, по умолчанию используется признак конца строки
 if (delim == "")
   delim = "\n";

//разбиваем строку выражения на фрагменты используя символ-разделитель
//и заносим в вектор
  while (text.find(delim) != string::npos)
    {
      pos = text.find(delim);
      str = text.substr(0, pos);
      text.erase(0, pos + 1);
      vecList->push_back(str);
    }

 if (!isdelimend)
   {
//заносим в вектор оставшуюся часть исходной строки
     vecList->push_back(text);
   }
};
//---------------------------------------------------------------------------

inline void StrToListW(vector <wstring> *vecList, wstring text, wstring delim, bool isdelimend)
{
 UINT pos;
 wstring str;

 vecList->clear();

//если разделитель не задан, по умолчанию используется признак конца строки
 if (delim == L"")
   delim = L"\n";

//разбиваем строку выражения на фрагменты используя символ-разделитель
//и заносим в вектор
  while (text.find(delim) != string::npos)
    {
      pos = text.find(delim);
      str = text.substr(0, pos);
      text.erase(0, pos + 1);
      vecList->push_back(str);
    }

 if (!isdelimend)
   {
//заносим в вектор оставшуюся часть исходной строки
     vecList->push_back(text);
   }
};
//---------------------------------------------------------------------------

//конвертирует список в строку
//разделяя элементы списка символом-разделителем
inline string ListToStr(vector<string> *vecList, string delim)
{
  string str_list;

//если разделитель не задан, по умолчанию используется признак конца строки
  if (delim == "")
    delim = "\n";

  if (vecList->size() > 0)
    {
      for (UINT i = 0; i < vecList->size(); i++)
        {
          str_list = str_list + vecList->at(i) + delim;
        }

/*UINT pos = 0; char s[256];
for (UINT i = 0; i < vecList->size(); i++)
{
pos += sprintf(s + pos, "vecList[%d] = '%s'\n", i, vecList->at(i).c_str());
}
MessageBox(NULL, s, "vecList at end of ListToStr()", MB_OK);*/
//MessageBox(NULL, str_list.c_str(), "str_list at end of ListToStr()", MB_OK);

    }
  else
    str_list = "ListToStr: !empty!";

  return str_list;
};
//---------------------------------------------------------------------------

//конвертирует список в строку
//разделяя элементы списка символом-разделителем
inline wstring ListToStrW(vector<wstring> *vecList, wstring delim)
{
  wstring str_list;

//если разделитель не задан, по умолчанию используется признак конца строки
  if (delim == L"")
    delim = L"\n";

  if (vecList->size() > 0)
    {
      for (UINT i = 0; i < vecList->size(); i++)
        {
          str_list = str_list + vecList->at(i) + delim;
        }
    }
  else
    str_list = L"ListToStr: !empty!";

  return str_list;
};
//---------------------------------------------------------------------------

inline string GetConfigLine(string conf_file, UINT index)
{
 string param;

//создадим вектор и загрузим в него содержимое конфиг-файла
 vector <string> vecList;

 if (!LoadFileToVector(&vecList, conf_file))
    return "^err_open_file";

 if (vecList.size() != 0)
   {
//если список не пустой и в нем есть строка с указанным индексом
     if (vecList.size() > index)
       {
         param = vecList.at(index);
         if (param.find("=") != string::npos)
           param = param.erase(1, param.find("="));
       }
     else
       param = "^no_line";
   }
 else
   param = "^no_data";

 return param;
};
//---------------------------------------------------------------------------

inline int GetConfigLineInd(string conf_file, string param)
{
  int index = -1;
  int length, pos = 0;

//создадим вектор и загрузим в него содержимое конфиг-файла
 vector <string> vecList;

 if (!LoadFileToVector(&vecList, conf_file))
    return -1;

 if (vecList.size() > 0)
   {
//если список не пуст, построчно сверим имена параметров с исходным
     UINT i = 0;

     while (i < vecList.size())
        {
//вырежем из строки значение параметра, оставив только имя
          length = vecList.at(i).length();

          if (param.find("=") != string::npos)
            pos = vecList.at(i).find("=");

          vecList.at(i).erase(pos, length - pos);

          if (vecList.at(i).find(param) != string::npos)
            {
              index = i;
            }

          i++;
        }
   }

  return index;
};
//---------------------------------------------------------------------------

inline string GetConfigLine(string conf_file, string param_name)
{
  int length, pos = 0;

//создадим вектор и загрузим в него содержимое конфиг-файла
 vector <string> vecList;

 if (!LoadFileToVector(&vecList, conf_file))
    return "^err_open_file";

 if (vecList.size() > 0)
   {
//если список не пуст, построчно сверим имена параметров с исходным
     UINT i = 0;

     while (i < vecList.size())
        {
//вырежем из строки значение параметра, оставив только имя
          length = vecList.at(i).length();
          pos = vecList.at(i).find("=");

          if (vecList.at(i).find(param_name) != string::npos)
            {
              return vecList.at(i).substr(pos + 1, length - pos);
            }

          i++;
        }
   }

  return "^no_line";
};
//---------------------------------------------------------------------------

inline bool SetConfigLine(string conf_file, UINT index, string value)
{
//создадим лист и загрузим в него содержимое конфиг-файла
 vector <string> vecList;
 LoadFileToVector(&vecList, conf_file);

 if (vecList.size() > 0)
   {
//если список не пустой и в нем есть строка с указанным индексом
     if (vecList.size() > index)
       {
         string str = vecList.at(index);
         int pos = str.find("=") + 1;
//уберем старый параметр
         str = str.erase(pos, str.length() - pos + 1);
//внесем новый
         vecList[index] = str + value;

         if (SaveVectorToFile(&vecList, conf_file))
           return true;
         else
           return false;
       }
     else
       return false;
   }
 else
   return false;
};
//---------------------------------------------------------------------------

inline bool SetConfigLine(string conf_file, string param, string value)
{
  return SetConfigLine(conf_file,
                       GetConfigLineInd(conf_file, param),
                       value);
};
//---------------------------------------------------------------------------

inline wstring GetConfigLineW(wstring conf_file, UINT index)
{
 wstring param;

//создадим вектор и загрузим в него содержимое конфиг-файла
 vector<wstring> vecList;

 if (!LoadFileToVectorW(&vecList, conf_file))
    return L"^err_open_file";

 if (vecList.size() != 0)
   {
//если список не пустой и в нем есть строка с указанным индексом
     if (vecList.size() > index)
       {
         param = vecList.at(index);
         if (param.find(L"=") != string::npos)
           param = param.erase(1, param.find(L"="));
       }
     else
       param = L"^no_line";
   }
 else
   param = L"^no_data";

 return param;
};
//---------------------------------------------------------------------------

inline int GetConfigLineIndW(wstring conf_file, wstring param)
{
  int index = -1;
  int length, pos = 0;

//создадим вектор и загрузим в него содержимое конфиг-файла
 vector<wstring> vecList;

 if (!LoadFileToVectorW(&vecList, conf_file))
    return -1;

 if (vecList.size() > 0)
   {
//если список не пуст, построчно сверим имена параметров с исходным
     UINT i = 0;

     while (i < vecList.size())
        {
//вырежем из строки значение параметра, оставив только имя
          length = vecList.at(i).length();

          if (param.find(L"=") != wstring::npos)
            pos = vecList.at(i).find(L"=");

          vecList.at(i).erase(pos, length - pos);

          if (vecList.at(i).find(param) != wstring::npos)
            {
              index = i;
            }

          i++;
        }
   }

  return index;
};
//---------------------------------------------------------------------------

inline wstring GetConfigLineW(wstring conf_file, wstring param_name)
{
  int length, pos = 0;

//создадим вектор и загрузим в него содержимое конфиг-файла
 vector<wstring> vecList;

 if (!LoadFileToVectorW(&vecList, conf_file))
    return L"^err_open_file";

 if (vecList.size() > 0)
   {
//если список не пуст, построчно сверим имена параметров с исходным
     UINT i = 0;

     while (i < vecList.size())
        {
//вырежем из строки значение параметра, оставив только имя
          length = vecList.at(i).length();
          pos = vecList.at(i).find(L"=");

          if (vecList.at(i).find(param_name) != wstring::npos)
            {
              return vecList.at(i).substr(pos + 1, length - pos);
            }

          i++;
        }
   }

  return L"^no_line";
};
//---------------------------------------------------------------------------

inline bool SetConfigLineW(wstring conf_file, UINT index, wstring value)
{
//создадим лист и загрузим в него содержимое конфиг-файла
 vector<wstring> vecList;
 LoadFileToVectorW(&vecList, conf_file);

 if (vecList.size() > 0)
   {
//если список не пустой и в нем есть строка с указанным индексом
     if (vecList.size() > index)
       {
         wstring str = vecList.at(index);
         int pos = str.find(L"=") + 1;
//уберем старый параметр
         str = str.erase(pos, str.length() - pos + 1);
//внесем новый
         vecList[index] = str + value;

         if (SaveVectorToFileW(&vecList, conf_file))
           return true;
         else
           return false;
       }
     else
       return false;
   }
 else
   return false;
};
//---------------------------------------------------------------------------

inline bool SetConfigLineW(wstring conf_file, wstring param, wstring value)
{
  return SetConfigLineW(conf_file,
                       GetConfigLineIndW(conf_file, param),
                       value);
};
//---------------------------------------------------------------------------

#endif // SOMEFUNC_H_INCLUDED
