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

#define DELIMEND true    //� ����� ����������� ������ ���� ������-�����������
#define NODELIMEND false //� ����� ����������� ������ ��� �������-�����������

#define WITH_DIRS true
#define WITHOUT_DIRS false

#define WITH_FULL_PATH true
#define WITHOUT_FULL_PATH false

//��� ������� CompareVersions()
#define VER_1_HIGHER 1
#define VER_2_HIGHER 2

enum TranslateType {TT_TO_STR = 0, TT_TO_INT = 1, TT_TO_BOOL = 2};

enum ReadParamExitCodes {RP_OK = 0,          //�������
						 RP_E_TO_STR = -1,   //������ �������������� � String
						 RP_E_TO_INT = -2,   //� int
						 RP_E_TO_BOOL = -3,  //� bool
						 RP_E_INV_PTR = -4,  //�������� ��������� �� ��������
						 RP_E_NO_LINE = -5,  //��� ������ � �����
						 RP_E_NO_FILE = -6,  //��� �����
						 RP_E_INV_TYPE = -7, //�������� �������� (�� �����. ����)
						 RP_E_UNC_TYPE = -8};//������ ����������� ���

enum ReadFileExitCodes {RF_OK = 0,          //�������
						RF_NO_OPEN = -1,    //������ �������� �����
						RF_READ_ERR = -2,   //������ ������
						RF_ACCESS_ERR = -3};//������ ��������� � �����
//----------------------------------------------------------

//---------------------------------------------------------------------------
// ������� ������ �����������
//---------------------------------------------------------------------------

//���������, ���������� �������� �����
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
#define OPSYMCNT sizeof(OperSymb) / sizeof(wchar_t) //���-�� �������� ��������
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

//�������� ���������� � �����, ���������� � ���������
//� ���������� ��
  FILEINFO GetFileInfo(String path);

//��������� � String ���������� ���������� �����
  String LoadTextFile(String filepath);
//������ �� ������ ������, ������� � ������� pos
  String ReadStringFromBinaryStream(TFileStream *stream,
								   int pos, int read_size);
//������ �� ������ ������ �� ������� �������
  String ReadStringFromBinaryStream(TFileStream *stream, int read_size);

//��������� ���� (�����), �������� ������ ��� ����
//� ������ ��������� (void *buf) �� ������� ������
//���������� ������ ����� � ������
  int LoadFile(String filepath, void *buf);

  void SaveToFile(String file, String text);
  void AddToFile(String file, String text);

  int SaveVectorToFile(std::vector <String> *vecStr, String path);

  void WriteStringIntoBinaryStream(TFileStream *stream, const String &str);

  int GetFileSize(String file);

// ��������� ������ �����
/*
��������: http://rxlib.ru/forums/index.php?act=ST&f=3&t=2305&s=ded848ac6abe6afafadeb852a5733406
��� ��� ������������
int ApplicationVersion[4] = {0,0,0,0}; // ������ � ����������� � ������

if(GetAppVersion(Application->ExeName.c_str(), ApplicationVersion))
{ // ������� � ���� ����������

  AppMajor = ApplicationVersion[0]; // ������ ������������ ����������
  AppMinor = ApplicationVersion[1];
  AppRelease = ApplicationVersion[2];
  AppBuild = ApplicationVersion[3];
}
*/
  bool GetAppVersion(const wchar_t *FileName, int *VerInfo);

//������� ��� �-�� ����, ���������� ������ � ������� ����� � ������� maj.min.rel.bld
  String GetVersionInString(const wchar_t *FileName);

//���������� ������ ���� ������, ����������
//0 - ������ �����
//1 - ������ ������
//2 - ������ ������
//-1 - ��������� ������ ��� ��������� ������
  int CompareVersions(const wchar_t *FileName1, const wchar_t *FileName2);

  TDateTime GetFileDateTime(String file);

//��������� � list ������ ������ �� ���������� ��������
//mask - ����� ����� �����
//dirs - �������� � ������ ��������
//fullpath - �������� � ������ ������ ����
  void GetFileList(TStringList *list, String path, String mask, bool dirs, bool fullpath);
//�� ��, �� � �������������� ���. ��������� � ����� ������
  void GetFileListRegEx(TStringList *list, String path, String reg_exp, bool dirs, bool fullpath);

//��������� � list ������ ������������ �� ���������� ��������
  void GetDirList(TStringList *list, String path, bool fullpath);

  int GetFileCount(String search_dir, String mask);
  int GetFileCountSubDirs(String search_dir, String mask);

  int GetFileCountRegEx(String search_dir, String reg_exp);
  int GetFileCountSubDirsRegEx(String search_dir, String reg_exp);

  int CopyAll(String fr_path, String to_path, TStringList *log);

  int CopyDirs(String source, String destin);

//������� �� ���������� �������� ��� ����� � �����������
  void DeleteAllFromDir(String destin);

//�-� ��������� � �������� ��������� ��������� �� ������, � �������
//��������� ����� ������ ��������� ��������-������������
  void StrToList(TStringList *list, String text, String delim);
  void StrToList(TStringList *list, String text);

//������� ���������� ������� ��� ������ � �������� String
  void StrToList(std::vector<String> *list, String text, String delim);

//������������ ������ ���� TStringList � ������
//�������� �������� ������ ��������-������������
  String ListToStr(TStringList *list, String delim);

//������� ��� ������� � ����
  void SaveLog(String file, String rec);

//���������� �������� ��������� �� �������� ������ ������-�����
//�-� ������� ��������� ��, ��� ���� ����� ������� "="
//������� ���� ������ ����� ���:
//**********************************
//�������� ��������� 1=��������
//...
//�������� ��������� n=��������
//**********************************
  String GetConfigLine(String conf_file, int index);
//�� ��, ��� � ����, �� ����� ���� �� ����� ���������
  String GetConfigLine(String conf_file, String param_name);

//���� � ������-����� ������ � ������� ������ ��������� � ���������� �� ������
//��� ��������� - ��� ��, ��� ��������� ����� �� "="
//���� �������� � ��������� ������ �� ������, ���������� -1
  int GetConfigLineInd(String conf_file, String param);

//���������� �������� ��������� � �������� ������ ������-�����
//�-� ������� ��������� ��, ��� ���� ����� ������� "="
//������� ���� ������ ����� ���:
//**********************************
//�������� ��������� 1=��������
//...
//�������� ��������� n=��������
//**********************************
  bool SetConfigLine(String conf_file, int index, String value);

//�� ��, �� ����� ������� �� ����� ���������
  bool SetConfigLine(String conf_file, String param, String value);

//��������� �������� � ������-����
  bool AddConfigLine(String conf_file, String param, String value);

//������� �������� �� �������
  bool RemConfigLine(String conf_file, String param);

//������ �������� �� ����� � ���������� �������� � target
//target ���������� � ��������� ������� ���� �������� ttype
  int ReadParameter(String conf_file,
					String param_name,
					void *target,
					TranslateType ttype);
//���������� ��������� �������� ���������� ReadParameter()
  const wchar_t *GetReadParamResult(ReadParamExitCodes code);
  const wchar_t *GetLastReadParamResult();

//������� ��� ������� � ��������
  void ShowLog(String message, TListBox *output);

//������� ��� ������� � ����
  void ShowLog(String message, TMemo *output);
  void SaveLog(String file, String rec);
  void SaveLog(String text, TStringList *log);

//����������� ����������� ����� ����� (MD5) � ���������� ��
  //String GetCRC(const String file);

//���������, �������� �� ��������� ����� ����������
//!� ���������!
  bool IsTextIP(String text);

//��������� �� ����������� ������ ����������� ���� 127.0.0.1:21
//��� ����� � ����, ��������� �������� �� ������� host, port
  bool ExtractHostPort(String conn_str, String &host, int &port);

//���������� ����� �� ��������� �������� val �������� ������
//type - ��� ������
//       percent - ����� �� 100%
//       inrange - � ��������� �� criteria1 �� criteria2
//       morethan - ������ ���
//       lessthan - ������ ���
  bool IsCorrect (int val,
                   String type,
                   int criteria1,
				   int criteria2);

//�-� ������ ������ main_str � ��������� ������ ������� symbol ������ insert_str
  String ParseString(const String &main_str, String target_str, const String insert_str);

//��������������� �-�, ���������, ��� � ������ ���������� �����
//���������� 0 - ���� integer
//1 - float
//2 - ������������� int
//3 - ������������� float
//-1 - ������ �� �������� float
//-2 - ������������ ������� � ������
//��������� float ��������� ��� �����, ��� � �������
  int IsStrNum(const wchar_t *somestring);

//���������, �������� �� symb �������� ��������, ���������� ������ ������� � ������� ����. ��������
//��� -1, ���� ���������� �� ����
  int IsOper(wchar_t symb);

//�� ��, �� ��� ����
  int IsNum(wchar_t symb);

//���������, ������ �� ����� val �� ��������� m � ������� m_pos
  bool InSet(int *m, int m_pos, int val);
//---------------------------------------------------------------------------

//����� ������ ���� �� �����
HWND FindHandleByName(const wchar_t *wnd_name);
int FindAllHandlesByPID(DWORD pid, std::vector<HWND> *handle_list);
DWORD FindPIDByHandle(HWND h);
HWND FindHandleByPID(DWORD pid);
DWORD GetProcessByExeName(wchar_t *name);
bool FindAllProcesessByExeName(const wchar_t *name, TStringList *pid_list);

BOOL CALLBACK EnumWindowsProc(HWND hwnd, LPARAM lParam);

//����� ��������� �������� ������
//����� � https://docs.microsoft.com/uk-ua/windows/desktop/Debug/retrieving-the-last-error-code
//������ �������������
//if(!GetProcessId(NULL))
//  ErrorExit(TEXT("GetProcessId"));
void ErrorExit(LPTSTR lpszFunction);

//---------------------------------------------------------------------------
//import from somefunc.h
//LEGACY CODE :)
//---------------------------------------------------------------------------

//������ ������ ������� � ��� ��������� target_str �� ������ insert_str
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

//��������� ����
  try
     {
	   wchar_t tmp[8192];
	   std::wifstream fin;

	   fin.open(AnsiOf(path.c_str()), std::ios_base::in);

       if (!fin.is_open())
         {
		   swprintf(str, L"�� ������� ������� ���� %s!\n", path.c_str());
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
                   swprintf(str, L"������ ��� ������ ����� %s\n", path.c_str());
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
	   swprintf(str, L"������ ��� ��������� � ����� %s : %s\n", path.c_str(), e.ToString().c_str());
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

//��������� ����
  try
     {
       wchar_t tmp[4096];
       std::wifstream fin;

       fin.open(AnsiOf(path.c_str()), std::ios_base::in);

       if (!fin.is_open())
         {
           swprintf(str, L"�� ������� ������� ���� %s!\n", path.c_str());
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
                   swprintf(str, L"������ ��� ������ ����� %s\n", path.c_str());
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
       swprintf(str, L"������ ��� ��������� � ����� %s : %s\n", path.c_str(), e.ToString().c_str());
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

//��������� ����
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
	   swprintf(str, L"������ ��� ��������� � ����� %s : %s\n", path, e.ToString().c_str());
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

//��������� ����
  try
     {
       std::wofstream fout(AnsiOf(path), std::ios_base::app | std::ios_base::ate);

       if (fout.fail())
         {
           swprintf(str, L"�� ������� ������� ���� %s!\n", path);
		   CPTransOut(CHAROEM, AnsiOf(str), strlen(AnsiOf(str)) + 1);

		   res = false;
         }
       else
         {
           fout << msg << std::endl;

           if (fout.fail())
             {
			   swprintf(str, L"������ ��� ������ ����� %s\n", path);
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
       swprintf(str, L"������ ��� ��������� � ����� %s : %s\n", path, e.ToString().c_str());
	   CPTransOut(CHAROEM, AnsiOf(str), strlen(AnsiOf(str)) + 1);

	   res = false;
	 }

  return res;
};
//---------------------------------------------------------------------------

inline bool SaveVectorToFileW(std::vector<std::wstring> *vecStr, std::wstring path)
{
  wchar_t str[128];

//��������� ����
  try
     {
       std::wofstream fout;
	   fout.open(AnsiOf(path.c_str()), std::ios_base::trunc | std::ios_base::out);

       if (!fout.is_open())
         {
           swprintf(str, L"�� ������� ������� ���� %s!\n", path.c_str());
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
				   swprintf(str, L"������ ��� ������ ����� %s\n", path.c_str());
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
	   swprintf(str, L"������ ��� ��������� � ����� %s : %s\n", path.c_str(), e.ToString().c_str());
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

//���� ����������� �� �����, �� ��������� ������������ ������� ����� ������
 if (delim == L"")
   delim = L"\n";

//��������� ������ ��������� �� ��������� ��������� ������-�����������
//� ������� � ������
  while (text.find(delim) != std::wstring::npos)
    {
	  pos = text.find(delim);
      str = text.substr(0, pos);
      text.erase(0, pos + 1);
      vecList->push_back(str);
    }

 if (!isdelimend)
   {
//������� � ������ ���������� ����� �������� ������
     vecList->push_back(text);
   }
};
//---------------------------------------------------------------------------

inline std::wstring GetConfigLineW(std::wstring conf_file, UINT index)
{
 std::wstring param;

//�������� ������ � �������� � ���� ���������� ������-�����
 std::vector<std::wstring> vecList;

 if (!LoadFileToVectorW(&vecList, conf_file))
    return L"^err_open_file";

 if (vecList.size() != 0)
   {
//���� ������ �� ������ � � ��� ���� ������ � ��������� ��������
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

//�������� ������ � �������� � ���� ���������� ������-�����
 std::vector<std::wstring> vecList;

 if (!LoadFileToVectorW(&vecList, conf_file))
    return -1;

 if (vecList.size() > 0)
   {
//���� ������ �� ����, ��������� ������ ����� ���������� � ��������
     UINT i = 0;

     while (i < vecList.size())
        {
//������� �� ������ �������� ���������, ������� ������ ���
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

//�������� ������ � �������� � ���� ���������� ������-�����
 std::vector<std::wstring> vecList;

 if (!LoadFileToVectorW(&vecList, conf_file))
    return L"^err_open_file";

 if (vecList.size() > 0)
   {
//���� ������ �� ����, ��������� ������ ����� ���������� � ��������
     UINT i = 0;

     while (i < vecList.size())
        {
//������� �� ������ �������� ���������, ������� ������ ���
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
//�������� ���� � �������� � ���� ���������� ������-�����
 std::vector<std::wstring> vecList;
 LoadFileToVectorW(&vecList, conf_file);

 if (vecList.size() > 0)
   {
//���� ������ �� ������ � � ��� ���� ������ � ��������� ��������
     if (vecList.size() > index)
       {
         std::wstring str = vecList.at(index);
         int pos = str.find(L"=") + 1;
//������ ������ ��������
         str = str.erase(pos, str.length() - pos + 1);
//������ �����
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
