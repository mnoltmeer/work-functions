//---------------------------------------------------------------------------

#ifndef TCPRequesterH
#define TCPRequesterH

#include <System.Classes.hpp>
#include <IdTCPClient.hpp>
//---------------------------------------------------------------------------

class TTCPRequester
{
  private:
	TIdTCPClient *FSender;
	String FHost;
	unsigned int FPort;
	String FLastError;

	void CreateSender();
	void FreeSender();
	void CheckSender();

	String GetHost();
	void SetHost(const String &host);

	unsigned int GetPort();
	void SetPort(unsigned int port);

  public:
	TTCPRequester();
	TTCPRequester(const String &host, unsigned int port);
	inline virtual ~TTCPRequester(){FreeSender();}

    inline String GetLastError(){return FLastError;}

//надсилає запит до хоста, якщо сталась помилка повертає 0
//якщо запит був успішний у rw_bufer буде записано відповідь хосту
	int AskData(TMemoryStream *rw_buffer);
//надсилає дані хосту, якщо сталась помилка повертає 0
	int SendData(TMemoryStream *rw_buffer);
	int SendString(const String &data);

	bool Connect();
	bool Disconnect();

	__property String Host = {read = GetHost, write = SetHost};
    __property unsigned int Port = {read = GetPort, write = SetPort};
};
#endif
