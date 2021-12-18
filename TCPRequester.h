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
