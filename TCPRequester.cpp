//---------------------------------------------------------------------------

#pragma hdrstop

#include <memory>
#include "TCPRequester.h"
//---------------------------------------------------------------------------
#pragma package(smart_init)

TTCPRequester::TTCPRequester(const String &host, unsigned int port)
{
  FHost = host;
  FPort = port;
  FLogging = false;

  try
	 {
	   CreateSender();
	 }
  catch (Exception &e)
	 {
	   FLastError = e.ToString();
	 }
}
//---------------------------------------------------------------------------

TTCPRequester::TTCPRequester(const String &host, unsigned int port, bool logging)
{
  FHost = host;
  FPort = port;
  FLogging = logging;

  try
	 {
	   CreateSender();
	 }
  catch (Exception &e)
	 {
	   FLastError = e.ToString();
	 }
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

	   FSender->Connect();
	 }
  catch (Exception &e)
	 {
	   FLastError = "Error creating Sender " + e.ToString();

	   if (FLogging) throw Exception("TTCPRequester: " + FLastError);
	 }
}
//---------------------------------------------------------------------------

void TTCPRequester::FreeSender()
{
  if (FSender->Connected())
	{
	  FSender->Disconnect();
	  FSender->Socket->Close();
	}

  if (FSender) delete FSender;
}
//---------------------------------------------------------------------------

void TTCPRequester::CheckSender()
{
  if (!FSender)
	throw Exception("Sender not inilialised");

  if (!FSender->Connected())
    throw Exception("Sender not connected");
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

	   if (FLogging) throw Exception("TTCPRequester: " + FLastError);
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

	   if (FLogging) throw Exception("TTCPRequester: " + FLastError);
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

	   if (FLogging) throw Exception("TTCPRequester: " + FLastError);
	 }

  return res;
}
//---------------------------------------------------------------------------

int TTCPRequester::SendString(const String &data)
{
  auto ms = std::make_unique<TStringStream>("", TEncoding::UTF8, true);
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

	   if (FLogging) throw Exception(FLastError);
	 }

  return res;
}
//---------------------------------------------------------------------------

