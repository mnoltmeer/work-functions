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
	bool FLogging; //����� ������� ���������
	String FLastError;

	void CreateSender();
	void FreeSender();
    void CheckSender();

  public:
    TTCPRequester(const String &host, unsigned int port);
	TTCPRequester(const String &host, unsigned int port, bool logging);
	inline virtual ~TTCPRequester(){FreeSender();}

    inline String GetLastError(){return FLastError;}

//������� ����� �� �����, ���� ������� ������� ������� 0
//���� ����� ��� ������� � rw_bufer ���� �������� ������� �����
	int AskData(TMemoryStream *rw_buffer);
//������� ��� �����, ���� ������� ������� ������� 0
	int SendData(TMemoryStream *rw_buffer);
	int SendString(const String &data);
};
#endif
