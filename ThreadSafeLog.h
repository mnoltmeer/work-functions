//---------------------------------------------------------------------------

#ifndef ThreadSafeLogH
#define ThreadSafeLogH

#include <System.Classes.hpp>
#include <System.SyncObjs.hpp>

enum {NoTimeStamp = true};
//---------------------------------------------------------------------------

class TThreadSafeLog
{
  private:
	TStringList *FList;
	TCriticalSection *FLock;

	int GetCount(){return FList->Count;}

  public:
	TThreadSafeLog(){FList = new TStringList(); FLock = new TCriticalSection();}
	virtual ~TThreadSafeLog(){delete FList; delete FLock;}

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
