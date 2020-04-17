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
#ifndef MyFuncH
#define MyFuncH

#include <System.Classes.hpp>
#include <System.RegularExpressions.hpp>
#include <Vcl.Controls.hpp>
#include <Vcl.StdCtrls.hpp>
#include <Vcl.ComCtrls.hpp>
#include <Vcl.Forms.hpp>
#include <Registry.hpp>
#include <IdFTP.hpp>
#include <Tlhelp32.h>
#include <vector>
#include <fstream>
#include <sstream>

#define POS_INT 0
#define POS_FLT 1
#define NEG_INT 2
#define NEG_FLT 3
#define NON_FLT -1
#define INC_CHR -2

#define STR_CONV_BUF_SIZE 12000 // the largest string you may have to convert

#define CHAROEM 0
#define OEMCHAR 1

#define DELIMEND true    //в конце разбиваемой строки есть символ-разделитель
#define NODELIMEND false //в конце разбиваемой строки нет символа-разделителя

#define WITH_DIRS true
#define WITHOUT_DIRS false

#define WITH_FULL_PATH true
#define WITHOUT_FULL_PATH false

//для функции CompareVersions()
#define VER_1_HIGHER 1
#define VER_2_HIGHER 2

enum TranslateType {TT_TO_STR = 0, TT_TO_INT = 1, TT_TO_BOOL = 2};

enum ReadParamExitCodes {RP_OK = 0,          //успешно
						 RP_E_TO_STR = -1,   //ошибка преобразования в String
						 RP_E_TO_INT = -2,   //в int
						 RP_E_TO_BOOL = -3,  //в bool
						 RP_E_INV_PTR = -4,  //неверный указатель на параметр
						 RP_E_NO_LINE = -5,  //нет строки в файле
						 RP_E_NO_FILE = -6,  //нет файла
						 RP_E_INV_TYPE = -7, //неверный параметр (не соотв. типу)
						 RP_E_UNC_TYPE = -8};//указан неизвестный тип

enum ReadFileExitCodes {RF_OK = 0,          //успешно
						RF_NO_OPEN = -1,    //ошибка открытия файла
						RF_READ_ERR = -2,   //ошибка чтения
						RF_ACCESS_ERR = -3};//ошибка обращения к файлу
//----------------------------------------------------------

//---------------------------------------------------------------------------
// Функции общего пользования
//---------------------------------------------------------------------------

//структура, содержащая свойства файла
struct FILEINFO
{
  String name;
  String ext;
  int length;
  //String atrb;
  //String crc;
};

static HWND g_hWnd;
static std::vector<HWND> *HWND_LST;
static ReadParamExitCodes LastReadParamResult;
static String mfLastError;
static wchar_t OperSymb[] = {'=', '+', '-', '*', '/'};
#define OPSYMCNT sizeof(OperSymb) / sizeof(wchar_t) //кол-во символов операций
static wchar_t NumSymb[10] = {'0', '1', '2', '3', '4', '5', '6', '7', '8', '9'};

void CPTransOut(UINT mode, const char *in_str, UINT str_len);

wchar_t* __fastcall UnicodeOf(const char* c);

char* __fastcall AnsiOf(const wchar_t* w);

void StartProcessByExeName(const String &file, const String &parent_dir);
void StartProcessByExeName(const String &file, const String &params, const String &parent_dir);
void StartProcessByExeName(const String &file);

bool AddAppAutoStart(const String &key_name, const String &app_path);
bool RemoveAppAutoStart(const String &key_name);
bool CheckAppAutoStart(const String &key_name);

TIdTCPClient *CreateSimpleTCPSender(const wchar_t *host, int port);
void FreeSimpleTCPSender(TIdTCPClient *sender);

//собирает информацию о файле, записывает в структуру
//и возвращает ее
  FILEINFO GetFileInfo(String path);

//выгружает в String содержимое текстового файла
  String LoadTextFile(String filepath);
//читает из потока строку, начиная с позиции pos
  String ReadStringFromBinaryStream(TFileStream *stream,
								   int pos, int read_size);
//читает из потока строку из текущей позиции
  String ReadStringFromBinaryStream(TFileStream *stream, int read_size);

//открывает файл (любой), выделяет память под него
//и задает указатель (void *buf) на область памяти
//возвращает размер файла в байтах
  int LoadFile(String filepath, void *buf);

  void SaveToFile(String file, String text);
  void AddToFile(String file, String text);

  int SaveVectorToFile(std::vector <String> *vecStr, String path);

  void WriteStringIntoBinaryStream(TFileStream *stream, const String &str);

  int GetFileSize(String file);

// получение версии файла
/*
источник: http://rxlib.ru/forums/index.php?act=ST&f=3&t=2305&s=ded848ac6abe6afafadeb852a5733406
Вот так пользоваться
int ApplicationVersion[4] = {0,0,0,0}; // массив с информацией о версии

if(GetAppVersion(Application->ExeName.c_str(), ApplicationVersion))
{ // занести в него информацию

  AppMajor = ApplicationVersion[0]; // версия обновляемого приложения
  AppMinor = ApplicationVersion[1];
  AppRelease = ApplicationVersion[2];
  AppBuild = ApplicationVersion[3];
}
*/
  bool GetAppVersion(const wchar_t *FileName, int *VerInfo);

//обертка над ф-ей выше, возвращает строку с версией файла в формате maj.min.rel.bld
  String GetVersionInString(const wchar_t *FileName);

//сравнивает версии двух файлов, возвращает
//0 - версии равны
//1 - первая больше
//2 - вторая больше
//-1 - произошла ошибка при получении версии
  int CompareVersions(const wchar_t *FileName1, const wchar_t *FileName2);

  TDateTime GetFileDateTime(String file);

//выгружает в list список файлов из указанного каталога
//mask - маска имени файла
//dirs - включать в список каталоги
//fullpath - выводить в список полные пути
  void GetFileList(TStringList *list, String path, String mask, bool dirs, bool fullpath);
//то же, но с использованием рег. выражений в маске файлов
  void GetFileListRegEx(TStringList *list, String path, String reg_exp, bool dirs, bool fullpath);

//выгружает в list список подкаталогов из указанного каталога
  void GetDirList(TStringList *list, String path, bool fullpath);

  int GetFileCount(String search_dir, String mask);
  int GetFileCountSubDirs(String search_dir, String mask);

  int GetFileCountRegEx(String search_dir, String reg_exp);
  int GetFileCountSubDirsRegEx(String search_dir, String reg_exp);

  int CopyAll(String fr_path, String to_path, TStringList *log);

  int CopyDirs(String source, String destin);

//удаляет из указанного каталога все файлы и подкаталоги
  void DeleteAllFromDir(String destin);

//ф-я принимает в качестве параметра указатель на список, в который
//сохраняет части строки отдленные символом-разделителем
  void StrToList(TStringList *list, String text, String delim);
  void StrToList(TStringList *list, String text);

//обертка предыдущей функции для работы с вектором String
  void StrToList(std::vector<String> *list, String text, String delim);

//конвертирует список типа TStringList в строку
//разделяя элементы списка символом-разделителем
  String ListToStr(TStringList *list, String delim);

//заносит лог события в файл
  void SaveLog(String file, String rec);

//возвращает значение параметра из указаной строки конфиг-файла
//ф-я считает значением то, что идет после символа "="
//поэтому файл должен иметь вид:
//**********************************
//название параметра 1=значение
//...
//название параметра n=значение
//**********************************
  String GetConfigLine(String conf_file, int index);
//то же, что и выше, но поиск идет по имени параметра
  String GetConfigLine(String conf_file, String param_name);

//ищет в конфиг-файле строку с заданым именем параметра и возвращает ее индекс
//имя параметра - это то, что находится слева от "="
//если параметр с указанным именем не найден, возвращает -1
  int GetConfigLineInd(String conf_file, String param);

//записывает значение параметра в указаную строку конфиг-файла
//ф-я считает значением то, что идет после символа "="
//поэтому файл должен иметь вид:
//**********************************
//название параметра 1=значение
//...
//название параметра n=значение
//**********************************
  bool SetConfigLine(String conf_file, int index, String value);

//то же, но поиск ведется по имени параметра
  bool SetConfigLine(String conf_file, String param, String value);

//добавляет параметр в конфиг-файл
  bool AddConfigLine(String conf_file, String param, String value);

//удаляет параметр из конфига
  bool RemConfigLine(String conf_file, String param);

//читает параметр из файла и записывает значение в target
//target приводится к указателю нужного типа согласно ttype
  int ReadParameter(String conf_file,
					String param_name,
					void *target,
					TranslateType ttype);
//возвращает текстовое описание результата ReadParameter()
  const wchar_t *GetReadParamResult(ReadParamExitCodes code);
  const wchar_t *GetLastReadParamResult();

//выводит лог события в листбокс
  void ShowLog(String message, TListBox *output);

//выводит лог события в мемо
  void ShowLog(String message, TMemo *output);
  void SaveLog(String file, String rec);
  void SaveLog(String text, TStringList *log);

//высчитывает контрольную сумму файла (MD5) и возвращает ее
  //String GetCRC(const String file);

//проверяет, является ли введенный текст айпишником
//!в доработке!
  bool IsTextIP(String text);

//извлекает из стандартной строки подключения типа 127.0.0.1:21
//имя хоста и порт, сохраняет значения по ссылкам host, port
  bool ExtractHostPort(String conn_str, String &host, int &port);

//определяет соотв ли введенный аргумент val критерию отбора
//type - тип отбора
//       percent - равно ли 100%
//       inrange - в диапазоне от criteria1 до criteria2
//       morethan - больше чем
//       lessthan - меньше чем
  bool IsCorrect (int val,
                   String type,
                   int criteria1,
				   int criteria2);

//ф-я парсит строку main_str и вставляет вместо символа symbol строку insert_str
  String ParseString(const String &main_str, String target_str, const String insert_str);

//вспомогательная ф-я, проверяет, что в строке содержится число
//возвращает 0 - если integer
//1 - float
//2 - отрицательный int
//3 - отрицательный float
//-1 - строка не является float
//-2 - недопустимые символы в строке
//признаком float считается как точка, так и запятая
  int IsStrNum(const wchar_t *somestring);

//проверяет, является ли symb символом операции, возвращает индекс символа в массиве спец. символов
//или -1, если совпадений не было
  int IsOper(wchar_t symb);

//то же, но для цифр
  int IsNum(wchar_t symb);

//проверяет, входит ли число val во множество m в позиции m_pos
  bool InSet(int *m, int m_pos, int val);
//---------------------------------------------------------------------------

//поиск хендла окна по имени
HWND FindHandleByName(const wchar_t *wnd_name);
int FindAllHandlesByPID(DWORD pid, std::vector<HWND> *handle_list);
DWORD FindPIDByHandle(HWND h);
HWND FindHandleByPID(DWORD pid);
DWORD GetProcessByExeName(wchar_t *name);
bool FindAllProcesessByExeName(const wchar_t *name, TStringList *pid_list);

BOOL CALLBACK EnumWindowsProc(HWND hwnd, LPARAM lParam);

//вывод последней виндовой ошибки
//взято с https://docs.microsoft.com/uk-ua/windows/desktop/Debug/retrieving-the-last-error-code
//пример использования
//if(!GetProcessId(NULL))
//  ErrorExit(TEXT("GetProcessId"));
void ErrorExit(LPTSTR lpszFunction);

//---------------------------------------------------------------------------
//import from somefunc.h
//LEGACY CODE :)
//---------------------------------------------------------------------------

//парсит строку заменяя в ней подстроку target_str на строку insert_str
inline std::wstring ParseStringW(const std::wstring main_str,
								 std::wstring target_str,
								 const std::wstring insert_str)
{
  std::wstring result = main_str;
  UINT pos, scount;

  while (result.find(target_str) != std::wstring::npos)
    {
      pos = result.find(target_str);
      scount = target_str.length();
      result.erase(pos, scount);
      result = result.insert(pos, insert_str);
    }

  return result;
};
//---------------------------------------------------------------------------

inline std::wstring &LoadFileToStringW(std::wstring path)
{
  wchar_t str[128];
  static std::wstring outstr;

  outstr = L"";

//открываем файл
  try
     {
	   wchar_t tmp[8192];
	   std::wifstream fin;

	   fin.open(AnsiOf(path.c_str()), std::ios_base::in);

       if (!fin.is_open())
         {
		   swprintf(str, L"Не удается открыть файл %s!\n", path.c_str());
		   CPTransOut(CHAROEM, AnsiOf(str), strlen(AnsiOf(str)) + 1);

		   outstr = L"-err-";
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
         }
     }
  catch(Exception &e)
     {
	   swprintf(str, L"Ошибка при обращении к файлу %s : %s\n", path.c_str(), e.ToString().c_str());
	   CPTransOut(CHAROEM, AnsiOf(str), strlen(AnsiOf(str)) + 1);
	   outstr = L"-err-";
	 }

  return outstr;
};
//---------------------------------------------------------------------------

inline bool LoadFileToVectorW(std::vector<std::wstring> *vecStr, std::wstring path)
{
  wchar_t str[128];
  bool res;

//открываем файл
  try
     {
       wchar_t tmp[4096];
       std::wifstream fin;

       fin.open(AnsiOf(path.c_str()), std::ios_base::in);

       if (!fin.is_open())
         {
           swprintf(str, L"Не удается открыть файл %s!\n", path.c_str());
		   CPTransOut(CHAROEM, AnsiOf(str), strlen(AnsiOf(str)) + 1);

		   res = false;
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

				   res = false;
                 }
			   else
				res = true;

               vecStr->push_back(std::wstring(tmp));
             }

		   fin.close();
         }
     }
  catch(Exception &e)
     {
       swprintf(str, L"Ошибка при обращении к файлу %s : %s\n", path.c_str(), e.ToString().c_str());
	   CPTransOut(CHAROEM, AnsiOf(str), strlen(AnsiOf(str)) + 1);

	   res = false;
	 }

  return res;
};
//---------------------------------------------------------------------------

inline const wchar_t *ReadFileToStrW(const wchar_t *path)
{
  wchar_t str[128];
  static std::wstring outstr;
  std::wstringstream in;
  outstr.clear();

//открываем файл
  try
     {
       std::wifstream fin;

       fin.open(AnsiOf(path), std::ios_base::in);

       if (!fin.is_open())
         {
		   outstr = L"-err-";
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
         }
     }
  catch(Exception &e)
     {
	   swprintf(str, L"Ошибка при обращении к файлу %s : %s\n", path, e.ToString().c_str());
	   CPTransOut(CHAROEM, AnsiOf(str), strlen(AnsiOf(str)) + 1);

	   outstr = L"-err-";
	 }

  return outstr.c_str();
};
//---------------------------------------------------------------------------

inline bool SaveStrToFileW(const wchar_t *msg, const wchar_t *path)
{
  wchar_t str[128];
  bool res;

//открываем файл
  try
     {
       std::wofstream fout(AnsiOf(path), std::ios_base::app | std::ios_base::ate);

       if (fout.fail())
         {
           swprintf(str, L"Не удается открыть файл %s!\n", path);
		   CPTransOut(CHAROEM, AnsiOf(str), strlen(AnsiOf(str)) + 1);

		   res = false;
         }
       else
         {
           fout << msg << std::endl;

           if (fout.fail())
             {
			   swprintf(str, L"Ошибка при чтении файла %s\n", path);
			   CPTransOut(CHAROEM, AnsiOf(str), strlen(AnsiOf(str)) + 1);

			   res = false;
			 }
		   else
			 res = true;

		   fout.close();
         }
     }
  catch(Exception &e)
     {
       swprintf(str, L"Ошибка при обращении к файлу %s : %s\n", path, e.ToString().c_str());
	   CPTransOut(CHAROEM, AnsiOf(str), strlen(AnsiOf(str)) + 1);

	   res = false;
	 }

  return res;
};
//---------------------------------------------------------------------------

inline bool SaveVectorToFileW(std::vector<std::wstring> *vecStr, std::wstring path)
{
  wchar_t str[128];

//открываем файл
  try
     {
       std::wofstream fout;
	   fout.open(AnsiOf(path.c_str()), std::ios_base::trunc | std::ios_base::out);

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
			   fout << vecStr->at(i) << std::endl;

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
  catch(Exception &e)
     {
	   swprintf(str, L"Ошибка при обращении к файлу %s : %s\n", path.c_str(), e.ToString().c_str());
	   CPTransOut(CHAROEM, AnsiOf(str), strlen(AnsiOf(str)) + 1);
     }

  return false;
};
//---------------------------------------------------------------------------

inline void StrToListW(std::vector <std::wstring> *vecList, std::wstring text, std::wstring delim, bool isdelimend)
{
 UINT pos;
 std::wstring str;

 vecList->clear();

//если разделитель не задан, по умолчанию используется признак конца строки
 if (delim == L"")
   delim = L"\n";

//разбиваем строку выражения на фрагменты используя символ-разделитель
//и заносим в вектор
  while (text.find(delim) != std::wstring::npos)
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

inline std::wstring GetConfigLineW(std::wstring conf_file, UINT index)
{
 std::wstring param;

//создадим вектор и загрузим в него содержимое конфиг-файла
 std::vector<std::wstring> vecList;

 if (!LoadFileToVectorW(&vecList, conf_file))
    return L"^err_open_file";

 if (vecList.size() != 0)
   {
//если список не пустой и в нем есть строка с указанным индексом
     if (vecList.size() > index)
       {
         param = vecList.at(index);
         if (param.find(L"=") != std::wstring::npos)
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

inline int GetConfigLineIndW(std::wstring conf_file, std::wstring param)
{
  int index = -1;
  int length, pos = 0;

//создадим вектор и загрузим в него содержимое конфиг-файла
 std::vector<std::wstring> vecList;

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

          if (param.find(L"=") != std::wstring::npos)
            pos = vecList.at(i).find(L"=");

          vecList.at(i).erase(pos, length - pos);

          if (vecList.at(i).find(param) != std::wstring::npos)
            {
              index = i;
            }

          i++;
        }
   }

  return index;
};
//---------------------------------------------------------------------------

inline std::wstring GetConfigLineW(std::wstring conf_file, std::wstring param_name)
{
  int length, pos = 0;

//создадим вектор и загрузим в него содержимое конфиг-файла
 std::vector<std::wstring> vecList;

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

          if (vecList.at(i).find(param_name) != std::wstring::npos)
            {
              return vecList.at(i).substr(pos + 1, length - pos);
            }

          i++;
        }
   }

  return L"^no_line";
};
//---------------------------------------------------------------------------

inline bool SetConfigLineW(std::wstring conf_file, UINT index, std::wstring value)
{
//создадим лист и загрузим в него содержимое конфиг-файла
 std::vector<std::wstring> vecList;
 LoadFileToVectorW(&vecList, conf_file);

 if (vecList.size() > 0)
   {
//если список не пустой и в нем есть строка с указанным индексом
     if (vecList.size() > index)
       {
         std::wstring str = vecList.at(index);
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

inline bool SetConfigLineW(std::wstring conf_file, std::wstring param, std::wstring value)
{
  return SetConfigLineW(conf_file,
                       GetConfigLineIndW(conf_file, param),
                       value);
};
//---------------------------------------------------------------------------

#endif
