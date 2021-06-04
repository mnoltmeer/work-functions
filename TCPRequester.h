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
	bool FLogging; //вмикає кидання виключень
	String FLastError;

	void CreateSender();
	void FreeSender();
    void CheckSender();

  public:
    TTCPRequester(const String &host, unsigned int port);
	TTCPRequester(const String &host, unsigned int port, bool logging);
	inline virtual ~TTCPRequester(){FreeSender();}

    inline String GetLastError(){return FLastError;}

//надсилає запит до хоста, якщо сталась помилка повертає 0
//якщо запит був успішний у rw_bufer буде записано відповідь хосту
	int AskData(TMemoryStream *rw_buffer);
//надсилає дані хосту, якщо сталась помилка повертає 0
	int SendData(TMemoryStream *rw_buffer);
	int SendString(const String &data);
};
#endif
