/*!
Copyright 2020-2021 Maxim Noltmeer (m.noltmeer@gmail.com)

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

#include <memory>
#include "TCPRequester.h"
//---------------------------------------------------------------------------
#pragma package(smart_init)

TTCPRequester::TTCPRequester()
{
  FHost = "127.0.0.1";
  FPort = 0;

  CreateSender();
}
//---------------------------------------------------------------------------

TTCPRequester::TTCPRequester(const String &host, unsigned int port)
{
  FHost = host;
  FPort = port;

  CreateSender();
  Connect();
}
//---------------------------------------------------------------------------

void TTCPRequester::CreateSender()
{
  try
	 {
	   FSender = new TIdTCPClient(NULL);

	   if (FHost == "")
		 throw Exception("Host is not defined");

	   FSender->Host = FHost;
	   FSender->Port = FPort;
	   FSender->IPVersion = Id_IPv4;
	   FSender->ConnectTimeout = 1500;
	   FSender->ReadTimeout = 5000;
	 }
  catch (Exception &e)
	 {
	   FLastError = "Error creating Sender " + e.ToString();
	 }
}
//---------------------------------------------------------------------------

void TTCPRequester::FreeSender()
{
  if (FSender)
	{
	  if (FSender->Connected())
		{
		  FSender->Disconnect();
		  FSender->Socket->Close();
		}

	  delete FSender;
	}
}
//---------------------------------------------------------------------------

String TTCPRequester::GetHost()
{
  if (FSender)
	return FSender->Host;
  else
	return "";
}
//---------------------------------------------------------------------------

void TTCPRequester::SetHost(const String &host)
{
  if (FSender)
	FSender->Host = host;
}
//---------------------------------------------------------------------------

unsigned int TTCPRequester::GetPort()
{
  if (FSender)
	return FSender->Port;
  else
	return 0;
}
//---------------------------------------------------------------------------

void TTCPRequester::SetPort(unsigned int port)
{
  if (FSender)
	FSender->Port = port;
}
//---------------------------------------------------------------------------

void TTCPRequester::CheckSender()
{
  try
	 {
	   if (!FSender)
		 throw Exception("Sender not inilialised");

	   if (!FSender->Connected())
		 throw Exception("Sender not connected");
	 }
  catch (Exception &e)
	 {
	   throw e;
	 }
}
//---------------------------------------------------------------------------

bool TTCPRequester::Connect()
{
  bool res;

  try
	 {
	   FSender->Connect();
	   res = true;
	 }
  catch (Exception &e)
	 {
	   FLastError = "Connect: " + e.ToString();
       res = false;
	 }

  return res;
}
//---------------------------------------------------------------------------

bool TTCPRequester::Disconnect()
{
  bool res;

  try
	 {
	   FSender->Disconnect();
	   res = true;
	 }
  catch (Exception &e)
	 {
	   FLastError = "Disconnect: " + e.ToString();
       res = false;
	 }

  return res;
}
//---------------------------------------------------------------------------

int TTCPRequester::AskData(TMemoryStream *rw_buffer)
{
  int res = 1;

  try
	 {
	   CheckSender();

	   if (!rw_buffer)
		 throw Exception("Invalid buffer pointer");

	   rw_buffer->Position = 0;
	   FSender->IOHandler->Write(rw_buffer, rw_buffer->Size, true);
	 }
  catch (Exception &e)
	 {
	   res = 0;
	   FLastError = "Error sending data " + e.ToString();
	 }

  try
	 {
	   CheckSender();

	   rw_buffer->Clear();
	   FSender->IOHandler->ReadStream(rw_buffer);
	 }
  catch (Exception &e)
	 {
	   res = 0;
	   FLastError = "Error receiving data " + e.ToString();
	 }

  rw_buffer->Position = 0;

  return res;
}
//---------------------------------------------------------------------------

int TTCPRequester::SendData(TMemoryStream *rw_buffer)
{
  int res = 1;

  try
	 {
       CheckSender();

	   if (!rw_buffer)
		 throw Exception("Invalid buffer pointer");

	   rw_buffer->Position = 0;
	   FSender->IOHandler->Write(rw_buffer, rw_buffer->Size, true);
	 }
  catch (Exception &e)
	 {
	   res = 0;
	   FLastError = "Error sending data " + e.ToString();
	 }

  return res;
}
//---------------------------------------------------------------------------

int TTCPRequester::SendString(const String &data)
{
  std::unique_ptr<TStringStream> ms(new TStringStream(data, TEncoding::UTF8, true));
  //auto ms = std::make_unique<TStringStream>("", TEncoding::UTF8, true);
  int res = 1;

  try
	 {
       CheckSender();

	   ms->Position = 0;
	   FSender->IOHandler->Write(ms.get(), ms->Size, true);
	 }
  catch (Exception &e)
	 {
	   res = 0;
	   FLastError = "TTCPRequester: Error sending data " + e.ToString();
	 }

  return res;
}
//---------------------------------------------------------------------------

