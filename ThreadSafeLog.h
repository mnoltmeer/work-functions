/*!
Copyright 2019-2021 Maxim Noltmeer (m.noltmeer@gmail.com)

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

#ifndef ThreadSafeLogH
#define ThreadSafeLogH

#include <System.Classes.hpp>
#include <System.SyncObjs.hpp>
#include <memory>

enum {NoTimeStamp = false, UseTimeStamp = true};
//---------------------------------------------------------------------------

class TThreadSafeLog
{
  private:
	std::unique_ptr<TStringList> FList;
	std::unique_ptr<TCriticalSection> FLock;

	int GetCount(){return FList->Count;}

  public:
	TThreadSafeLog();
	virtual ~TThreadSafeLog(){};

	void Add(const String &rec);
	void Add(const String &rec, bool timestamp);
	int IndexOf(const String &rec);
	String Get(int ind);
	void Remove(int ind);
	void Clear();
	void SaveToFile(const String &file);
    void SaveToStream(TStringStream *stream);
	void LoadFromFile(const String &file);
    String GetText();

	__property int Count = {read = GetCount};
};
//---------------------------------------------------------------------------
#endif
