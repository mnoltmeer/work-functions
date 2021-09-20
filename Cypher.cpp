/*
Copyright 2021 Maxim Noltmeer m.noltmeer@gmail.com

	TAESCypher is free software: you can redistribute it and/or modify
	it under the terms of the GNU General Public License as published by
	the Free Software Foundation, either version 3 of the License, or
	(at your option) any later version.

	TAESCypher is distributed in the hope that it will be useful,
	but WITHOUT ANY WARRANTY; without even the implied warranty of
	MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
	GNU General Public License for more details.

	You should have received a copy of the GNU General Public License
	along with TAESCypher.  If not, see <http://www.gnu.org/licenses/>.
*/

#include <windows.h>
#include <wincrypt.h>
#include <bcrypt.h>
#include <strsafe.h>
#include <memory>

#pragma hdrstop

#pragma warning(disable: 4018)

#include "Cypher.h"
//---------------------------------------------------------------------------

String LastErrorToString()
{
// Retrieve the system error message for the last-error code
  LPVOID lpMsgBuf;
  LPVOID lpDisplayBuf;
  DWORD dw = GetLastError();

  FormatMessage(FORMAT_MESSAGE_ALLOCATE_BUFFER |
				FORMAT_MESSAGE_FROM_SYSTEM |
				FORMAT_MESSAGE_IGNORE_INSERTS,
				NULL,
				dw,
				MAKELANGID(LANG_NEUTRAL, SUBLANG_DEFAULT),
				(LPTSTR) &lpMsgBuf,
				0, NULL );

// Display the error message and exit the process

  lpDisplayBuf = (LPVOID)LocalAlloc(LMEM_ZEROINIT,
									(lstrlen((LPCTSTR)lpMsgBuf) + 40) * sizeof(TCHAR));
  StringCchPrintf((LPTSTR)lpDisplayBuf,
				  LocalSize(lpDisplayBuf) / sizeof(TCHAR),
				  TEXT("%d: %s"),
				  dw, lpMsgBuf);


  String res = (LPTSTR)lpDisplayBuf;

  LocalFree(lpMsgBuf);
  LocalFree(lpDisplayBuf);

  return res;
}
//---------------------------------------------------------------------------
//---------------------------------------------------------------------------
//---------------------------------------------------------------------------

TMemoryStream *TSAESCypher::Crypt(TMemoryStream *data, const char *pass)
{
  try
	 {
	   std::unique_ptr<TAESCypher> cypher(new TAESCypher(data, pass, coCrypt));

	   data->LoadFromStream(cypher->Data);
	   data->Position = 0;
	 }
  catch (Exception &e)
	 {
	   throw e;
	 }

  return data;
}
//---------------------------------------------------------------------------

TMemoryStream *TSAESCypher::Encrypt(TMemoryStream *data, const char *pass)
{
  try
	 {
	   std::unique_ptr<TAESCypher> cypher(new TAESCypher(data, pass, coEncrypt));

	   data->LoadFromStream(cypher->Data);
	   data->Position = 0;
	 }
  catch (Exception &e)
	 {
	   throw e;
	 }

  return data;
}
//---------------------------------------------------------------------------
//---------------------------------------------------------------------------
//---------------------------------------------------------------------------

TAESCypher::TAESCypher(TMemoryStream *data, const char *password, CypherOperation operation)
{
  try
	 {
	   FPass = nullptr;
	   FData = new TMemoryStream();
	   FData->LoadFromStream(data);
	   DataCrypt(FData, password, operation);
	 }
  catch (Exception &e)
	 {
       FData->Clear();
	   FLastError = e.ToString();
	 }
}
//---------------------------------------------------------------------------

TAESCypher::TAESCypher(const String &data, const char *password)
{
  try
	 {
       FPass = nullptr;
	   FData = new TMemoryStream();
	   FData->Write(data.c_str(), data.Length() * sizeof(wchar_t));
	   DataCrypt(FData, password, coCrypt);
	 }
  catch (Exception &e)
	 {
       FData->Clear();
	   FLastError = e.ToString();
	 }
}
//---------------------------------------------------------------------------

TAESCypher::TAESCypher(const char *password)
{
  try
	 {
	   FData = new TMemoryStream();
	   FPass = new char[strlen(password) + 1];
	   strcpy(FPass, password);
	 }
  catch (Exception &e)
	 {
	   FLastError = e.ToString();
	 }
}
//---------------------------------------------------------------------------

bool TAESCypher::LoadCryptSystem(const char *pass)
{
  bool res = true;

  try
	 {
	   if (!CryptAcquireContext(&hProv, NULL, MS_ENH_RSA_AES_PROV, PROV_RSA_AES, 0))
		 {
		   if (GetLastError() == NTE_BAD_KEYSET)
			 {
			   if (!CryptAcquireContext(&hProv, NULL, MS_ENH_RSA_AES_PROV, PROV_RSA_AES, CRYPT_NEWKEYSET))
				 throw Exception("LoadCryptSystem:CryptAcquireContext error " + LastErrorToString());
			 }
		   else
             throw Exception("LoadCryptSystem:CryptAcquireContext error " + LastErrorToString());
		 }

	   if (!CryptCreateHash(hProv, CALG_SHA_256, 0, 0, &hHash))
		 throw Exception("LoadCryptSystem:CryptCreateHash error " + LastErrorToString());

	   if (!CryptHashData(hHash, (BYTE*)pass, strlen(pass), 0))
		 throw Exception("LoadCryptSystem:CryptHashData error " + LastErrorToString());

	   if (!CryptDeriveKey(hProv, CALG_AES_192, hHash, CRYPT_EXPORTABLE, &hKey))
		 throw Exception("LoadCryptSystem:CryptDeriveKey error " + LastErrorToString());
	 }
  catch (Exception &e)
	 {
	   throw e;
	   res = false;
	 }

  return res;
}
//---------------------------------------------------------------------------

void TAESCypher::ReleaseCryptSystem()
{
  CryptDestroyHash(hHash);
  CryptDestroyKey(hKey);
  CryptReleaseContext(hProv, 0);
}
//---------------------------------------------------------------------------

void TAESCypher::DataCrypt(TMemoryStream *ms, const char *password, CypherOperation operation)
{
  try
	 {
	   if (!LoadCryptSystem(password))
		 throw Exception("Initialising CryptSystem failed");

	   try
		  {
			DWORD size = ms->Size;
			std::vector<char> data(size + 10240);
			ms->Position = 0;
			ms->Read(data.data(), size);
			ms->Clear();

			if (operation == coCrypt)
			  {
				if (!CryptEncrypt(hKey, NULL, true, NULL, reinterpret_cast<BYTE*>(data.data()), &size, data.size()))
				  throw Exception("Crypting error: " + LastErrorToString());
				else
				  ms->Write(data.data(), size);
			  }
			else if (operation == coEncrypt)
			  {
				if (!CryptDecrypt(hKey, NULL, true, NULL, reinterpret_cast<BYTE*>(data.data()), &size))
				  throw Exception("Encrypting error: " + LastErrorToString());
				else
				  ms->Write(data.data(), size);
			  }
			else
			  throw Exception("Crypting error, unknown operation type");

			ms->Position = 0;
		  }
	   __finally{ReleaseCryptSystem();}
	 }
  catch (Exception &e)
	 {
	   throw e;
	 }
}
//---------------------------------------------------------------------------

void TAESCypher::CryptFile(const String &file)
{
  try
	 {
	   if (!FPass)
		 throw Exception("Crypt password ont defined!");

	   FData->LoadFromFile(file);
	   DataCrypt(FData, FPass, coCrypt);
	 }
  catch (Exception &e)
	 {
	   FLastError = e.ToString();
	 }
}
//---------------------------------------------------------------------------

void TAESCypher::EncryptFile(const String &file)
{
  try
	 {
       if (!FPass)
		 throw Exception("Crypt password ont defined!");

	   FData->LoadFromFile(file);
	   DataCrypt(FData, FPass, coEncrypt);
	 }
  catch (Exception &e)
	 {
	   FLastError = e.ToString();
	 }
}
//---------------------------------------------------------------------------

String TAESCypher::DataToString()
{
  String res;

  try
	 {
	   std::unique_ptr<wchar_t[]> buf(new wchar_t[Data->Size]);

	   Data->Read(buf.get(), Data->Size);
	   res = buf.get();
	 }
  catch (Exception &e)
	 {
	   res = "";
	   FLastError = e.ToString();
	 }

  return res;
}
//---------------------------------------------------------------------------

const wchar_t *TAESCypher::DataToChar()
{
  return DataToString().c_str();
}
//---------------------------------------------------------------------------

std::vector<char> TAESCypher::DataToVector()
{
  std::vector<char> res(Data->Size);

  try
	 {
       Data->Read(res.data(), Data->Size);
	 }
  catch (Exception &e)
	 {
	   res.clear();
	   FLastError = e.ToString();
	 }

  return res;
}
//---------------------------------------------------------------------------

BYTE *TAESCypher::DataToByte()
{
  return reinterpret_cast<BYTE*>(DataToVector().data());
}
//---------------------------------------------------------------------------

void TAESCypher::DataToFile(const String &file)
{
  try
	 {
	   Data->SaveToFile(file);
	 }
  catch (Exception &e)
	 {
	   FLastError = e.ToString();
	 }
}
//---------------------------------------------------------------------------

#pragma package(smart_init)
