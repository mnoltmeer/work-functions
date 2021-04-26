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
#ifndef CypherH
#define CypherH
//---------------------------------------------------------------------------
#include <vector>
#include <System.Classes.hpp>

enum CypherOperation {coCrypt = 0, coEncrypt = 1};

String LastErrorToString();

class TAESCypher //звичайний клас, шифрує/дешифрує дані при створенні
{
private:
	char *FPass;
	TMemoryStream *FData; //буфер у якому зберігаються дані (при шифровці та дешифровці)
	String FLastError;
    HCRYPTPROV hProv; //дескриптор криптопровайдера
	HCRYPTKEY hKey; //дескриптор ключа, созданного из хэш-кода
	HCRYPTHASH hHash; //дескриптор хэш-объекта
    bool LoadCryptSystem(const char *pass);
	void ReleaseCryptSystem();
	void DataCrypt(TMemoryStream *ms, const char *password, CypherOperation operation);

public:
	TAESCypher(TMemoryStream *data, const char *password, CypherOperation operation);
	TAESCypher(const String &data, const char *password);
    TAESCypher(const char *password);
	inline virtual ~TAESCypher(){if (FData) delete FData; if (FPass) delete[] FPass;}

	String DataToString();
	const wchar_t *DataToChar();
	std::vector<char> DataToVector();
	BYTE *DataToByte();
    void DataToFile(const String &file);

	void CryptFile(const String &file);
	void EncryptFile(const String &file);

	__property TMemoryStream *Data = {read = FData};
    __property String LastError = {read = FLastError};
};


class TSAESCypher //static клас, просто шифрує/дешифрує поток даних
{
public:
	static TMemoryStream *Crypt(TMemoryStream *data, const char *pass);
	static TMemoryStream *Encrypt(TMemoryStream *data, const char *pass);
};
#endif
