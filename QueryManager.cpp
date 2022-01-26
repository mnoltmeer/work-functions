/*!
Copyright 2022 Maxim Noltmeer (m.noltmeer@gmail.com)

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

#include "QueryManager.h"
//---------------------------------------------------------------------------
#pragma package(smart_init)

TManagedQuery::TManagedQuery(const String &id, TFDConnection *conn)
{
  FID = id;
  FConn = conn;
  FTrans.reset(CreateNewTransactionObj(FConn));
  FQuery.reset(CreateNewQueryObj(FTrans.get(), FConn));
}
//---------------------------------------------------------------------------

TFDParam *TManagedQuery::FGetParam(String name)
{
  TFDParam *res;

  try
	 {
	   res = FQuery->ParamByName(name);
	 }
  catch (Exception &e)
	 {
	   res = NULL;
	   e.Message = "TManagedQuery::Params: " + e.Message;
	   throw e;
	 }

  return res;
}
//---------------------------------------------------------------------------

TField *TManagedQuery::FGetField(String name)
{
  TField *res;

  try
	 {
	   res = FQuery->FieldByName(name);
	 }
  catch (Exception &e)
	 {
	   res = NULL;
	   e.Message = "TManagedQuery::Fields: " + e.Message;
	   throw e;
	 }

  return res;
}
//---------------------------------------------------------------------------

bool TManagedQuery::Execute()
{
  bool res;

  try
	 {
	   if (!FConn->Connected)
		 throw Exception("No connection");

	   FTrans->StartTransaction();
       FQuery->Prepare();

	   if (Text.Trim().SubString(1, 6).UpperCase() == "SELECT")
		 {
		   FQuery->Open();
		   FQuery->FetchAll();
		   FRecCount = FQuery->RecordCount;
		 }
	   else
		 {
		   FQuery->ExecSQL();
		   FRecCount = FQuery->RowsAffected;
		 }

	   FTrans->Commit();
	 }
  catch (Exception &e)
	 {
	   FTrans->Rollback();
	   res = false;
	   e.Message = "TManagedQuery::Execute: " + e.Message;
	   throw e;
	 }

  return res;
}
//---------------------------------------------------------------------------

void TManagedQuery::Init()
{
  try
	 {
       FRecCount = -1;
	   FQuery->SQL->Clear();
	   FQuery->Params->Clear();
	 }
  catch (Exception &e)
	 {
	   e.Message = "TManagedQuery::Init: " + e.Message;
	   throw e;
	 }
}
//---------------------------------------------------------------------------

void TManagedQuery::Close()
{
  try
	 {
	   FQuery->Close();
	 }
  catch (Exception &e)
	 {
	   e.Message = "TManagedQuery::Close: " + e.Message;
	   throw e;
	 }
}
//---------------------------------------------------------------------------

TManagedQuery *TQueryManager::FGetItem(int ind)
{
  TManagedQuery *res;

  try
	 {
	   if ((ind < 0) || (ind >= Count))
		 throw Exception("Out of range");
	   else
		 res = FQueries[ind];
	 }
  catch (Exception &e)
	 {
	   res = NULL;
	   e.Message = "TQueryManager::Items: " + e.Message;
	   throw e;
	 }

  return res;
}
//---------------------------------------------------------------------------

void TQueryManager::FSetItem(int ind, TManagedQuery *val)
{
  try
	 {
	   if ((ind < 0) || (ind >= Count))
		 throw Exception("Out of range");
	   else if (!val)
		 throw Exception("Invalid TManagedQuery pointer");
	   else
		 {
		   delete FQueries[ind];
		   FQueries[ind] = val;
		 }
	 }
  catch (Exception &e)
	 {
	   e.Message = "TQueryManager::Items: " + e.Message;
	   throw e;
	 }
}
//---------------------------------------------------------------------------

TManagedQuery *TQueryManager::FGetItemID(String id)
{
  try
	 {
	   int ind = IndexOf(id);

	   if ((ind < 0) || (ind >= Count))
		 throw Exception("TQueryManager::Itmes: Out of bounds!");
	   else
		return FQueries[ind];
	 }
  catch (Exception &e)
	 {
	   throw e;
	 }
}
//---------------------------------------------------------------------------

void TQueryManager::FSetItemID(String id, TManagedQuery *val)
{
  try
	 {
	   int ind = IndexOf(id);

	   if ((ind < 0) || (ind >= Count))
		 throw Exception("Out of range");
	   else if (!val)
		 throw Exception("Invalid TManagedQuery pointer");
	   else
		 {
		   delete FQueries[ind];
		   FQueries[ind] = val;
		 }
	 }
  catch (Exception &e)
	 {
	   throw e;
	 }
}
//---------------------------------------------------------------------------

int TQueryManager::IndexOf(const String &id)
{
  for (int i = 0; i < Count; i++)
	 {
	   if (FQueries[i]->ID == id)
		 return i;
	 }

  return -1;
}
//---------------------------------------------------------------------------

void TQueryManager::Clear()
{
  for (int i = 0; i < Count; i++)
	 delete FQueries[i];

  FQueries.clear();
}
//---------------------------------------------------------------------------

TManagedQuery *TQueryManager::Add(const String &id, TFDConnection *conn)
{
  TManagedQuery *res;

  try
	 {
	   res = new TManagedQuery(id, conn);
	   FQueries.push_back(res);
	 }
  catch (Exception &e)
	 {
	   res = NULL;
	   e.Message = "TQueryManager::Add: " + e.Message;
	   throw e;
	 }

  return res;
}
//---------------------------------------------------------------------------

TManagedQuery *TQueryManager::Add(TManagedQuery *query)
{
  TManagedQuery *res;

  try
	 {
	   if (!query)
		 throw Exception("Invalid TManagedQuery pointer");

	   FQueries.push_back(query);
	   res = query;
	 }
  catch (Exception &e)
	 {
	   res = NULL;
	   e.Message = "TQueryManager::Add: " + e.Message;
	   throw e;
	 }

  return res;
}
//---------------------------------------------------------------------------

void TQueryManager::Remove(int ind)
{
  try
	 {
	   if ((ind < 0) || (ind >= Count))
		 throw Exception("Out of range");
	   else
		 {
		   delete FQueries[ind];
		   FQueries.erase(FQueries.begin() + ind);
		 }
	 }
  catch (Exception &e)
	 {
	   e.Message = "TQueryManager::Remove: " + e.Message;
	   throw e;
	 }
}
//---------------------------------------------------------------------------

void TQueryManager::Remove(const String &id)
{
  try
	 {
	   Remove(IndexOf(id));
	 }
  catch (Exception &e)
	 {
	   throw e;
	 }
}
//---------------------------------------------------------------------------

void TQueryManager::LoadFromFile(const String &xml)
{
  std::unique_ptr<TStringStream> data(new TStringStream("", TEncoding::UTF8, true));

  try
	 {
	   data->LoadFromFile(xml);
	   ImportFromStream(data.get());
	 }
  catch (Exception &e)
	 {
	   e.Message = "TQueryManager::LoadFromFile:: " + e.Message;
	   throw e;
	 }
}
//---------------------------------------------------------------------------

bool TQueryManager::ImportFromStream(TStringStream *ms)
{
  bool res = false;

  try
	 {
	   _di_IXMLDocument ixml = NewXMLDocument();

	   ms->Position = 0;

	   ixml->Active = true;
	   ixml->Encoding = "UTF-8";
	   ixml->Options = ixml->Options << doNodeAutoIndent;
	   ixml->LoadFromStream(ms, xetUTF_8);

	   _di_IXMLNode Document = ixml->DocumentElement;
	   _di_IXMLNode ImportedQuery;
	   String id;
	   TManagedQuery *new_query;

	   for (int i = 0; i < Document->ChildNodes->Count; i++)
		  {
			ImportedQuery = Document->ChildNodes->Nodes[i];
			id = ImportedQuery->GetAttribute("ID");

			if (IndexOf(id) != -1)
			  throw Exception("Duplicate ID = " + id);

			new_query = new TManagedQuery(id, FConn);

			new_query->Text = ImportedQuery->ChildNodes->Nodes[0]->NodeValue;

			Add(new_query);
		  }

	   res = true;
	 }
  catch (Exception &e)
	 {
	   throw e;
	 }

  return res;
}
//---------------------------------------------------------------------------

void TQueryManager::SaveToFile(const String &xml)
{
  std::unique_ptr<TStringStream> data(ExportToStream());

  try
	 {
	   data->SaveToFile(xml);
	 }
  catch (Exception &e)
	 {
	   throw e;
	 }
}
//---------------------------------------------------------------------------

TStringStream *TQueryManager::ExportToStream()
{
  TStringStream *data = new TStringStream("", TEncoding::UTF8, true);

  try
	 {
	   data->WriteString("<QueryManager>");

	   for (int i = 0; i < Count; i++)
		  {
			data->WriteString("\t<Query ID = '" + Items[i]->ID + "'>\r\n");

			data->WriteString("\t\t<Text>");
			data->WriteString(Items[i]->Text);
			data->WriteString("\t\t</Text>\r\n");

			data->WriteString("\t</Query>");
		  }

	   data->WriteString("</QueryManager>");

       data->Position = 0;
	 }
  catch (Exception &e)
	 {
	   e.Message = "TQueryManager::SaveToFile:: " + e.Message;
	   throw e;
	 }

  return data;
}
//---------------------------------------------------------------------------

TFDTransaction *CreateNewTransactionObj(TFDConnection *conn)
{
  TFDTransaction *t = new TFDTransaction(NULL);

  t->Connection = conn;
  t->Options->AutoCommit = false;
  t->Options->AutoStart = false;
  t->Options->AutoStop = false;
  t->Options->DisconnectAction = xdRollback;
  t->Options->Isolation = xiSnapshot;

  return t;
}
//---------------------------------------------------------------------------

TFDQuery *CreateNewQueryObj(TFDTransaction *t, TFDConnection *conn)
{
  TFDQuery * q = new TFDQuery(NULL);

  q->Connection = conn;
  q->Transaction = t;

  return q;
}
//---------------------------------------------------------------------------

