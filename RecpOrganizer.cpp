/*!
Copyright 2019, 2021 Maxim Noltmeer (m.noltmeer@gmail.com)
*/
//---------------------------------------------------------------------------
#include <memory>
#pragma hdrstop

#include "RecpOrganizer.h"
//---------------------------------------------------------------------------
#pragma package(smart_init)

TRecpientItemCollection::TRecpientItemCollection(String file)
{
  if (LoadFromFile(file))
    CollectionFile = file;
}
//---------------------------------------------------------------------------

TRecpientItemCollection::~TRecpientItemCollection()
{
  recp_items.clear();
}
//---------------------------------------------------------------------------

RecipientItem *TRecpientItemCollection::IRead(int ind)
{
  if (ind < recp_items.size())
	return &recp_items[ind];
  else
	{
	  throw Exception("recp_items: Out of bounds!");
    }
}
//---------------------------------------------------------------------------

void TRecpientItemCollection::IWrite(int ind, RecipientItem *itm)
{
  if (ind < recp_items.size())
	recp_items[ind] = *itm;
  else
	throw Exception("recp_items: Out of bounds!");
}
//---------------------------------------------------------------------------

int TRecpientItemCollection::IsChildItem(TTreeNode *n)
{
  RecipientItem *lnk = FindItem(n);

  if (!lnk)
	return 0;
  else
	return lnk->ParentNodeID;
}
//---------------------------------------------------------------------------

void TRecpientItemCollection::Remove(int id)
{
  RecipientItem *lnk = Find(id);

  if (lnk)
	{
	  delete lnk->Node;
	  recp_items.erase(recp_items.begin() + GetIndexOf(id));
    }
}
//---------------------------------------------------------------------------

void TRecpientItemCollection::Remove(TTreeNode *n)
{
  RecipientItem *lnk = Find(n);

  if (lnk)
	{
	  delete lnk->Node;
	  recp_items.erase(recp_items.begin() + GetIndexOf(lnk->ID));
    }
}
//---------------------------------------------------------------------------

RecipientItem *TRecpientItemCollection::Find(int id)
{
  for (int i = 0; i < recp_items.size(); i++)
	 {
	   if (recp_items[i].ID == id)
		 return &recp_items[i];
	 }

  return NULL;
}
//---------------------------------------------------------------------------

RecipientItem *TRecpientItemCollection::Find(TTreeNode *n)
{
  for (int i = 0; i < recp_items.size(); i++)
	 {
	   if (recp_items[i].Node == n)
		 return &recp_items[i];
	 }

  return NULL;
}
//---------------------------------------------------------------------------

RecipientItem *TRecpientItemCollection::FindItem(int id)
{
  for (int i = 0; i < recp_items.size(); i++)
	 {
	   if ((recp_items[i].ID == id) && (recp_items[i].ParentNodeID > 0))
		 return &recp_items[i];
	 }

  return NULL;
}
//---------------------------------------------------------------------------

RecipientItem *TRecpientItemCollection::FindItem(TTreeNode *n)
{
  for (int i = 0; i < recp_items.size(); i++)
	 {
	   if ((recp_items[i].Node == n) && (recp_items[i].ParentNodeID > 0))
		 return &recp_items[i];
	 }

  return NULL;
}
//---------------------------------------------------------------------------

RecipientItem *TRecpientItemCollection::FindItem(String name)
{
  for (int i = 0; i < recp_items.size(); i++)
	 {
	   if ((recp_items[i].Name == name) && (recp_items[i].ParentNodeID > 0))
		 return &recp_items[i];
	 }

  return NULL;
}
//---------------------------------------------------------------------------

RecipientItem *TRecpientItemCollection::FindGroup(String name)
{
  for (int i = 0; i < recp_items.size(); i++)
	 {
	   if ((recp_items[i].Name == name) && (recp_items[i].ParentNodeID == 0))
		 return &recp_items[i];
	 }

  return NULL;
}
//---------------------------------------------------------------------------

RecipientItem *TRecpientItemCollection::FindGroup(int id)
{
  for (int i = 0; i < recp_items.size(); i++)
	 {
	   if ((recp_items[i].ID == id) && (recp_items[i].ParentNodeID == 0))
		 return &recp_items[i];
	 }

  return NULL;
}
//---------------------------------------------------------------------------

RecipientItem *TRecpientItemCollection::FindGroup(TTreeNode *n)
{
  for (int i = 0; i < recp_items.size(); i++)
	 {
	   if ((recp_items[i].Node == n) && (recp_items[i].ParentNodeID == 0))
		 return &recp_items[i];
	 }

  return NULL;
}
//---------------------------------------------------------------------------

int TRecpientItemCollection::GetIndexOf(int id)
{
  for (int i = 0; i < recp_items.size(); i++)
	 {
	   if (recp_items[i].ID == id)
		 return i;
	 }

  return -1;
}
//---------------------------------------------------------------------------

bool TRecpientItemCollection::LoadFromFile(String file)
{
  if (!FileExists(file))
	return false;

  std::unique_ptr<TFileStream> fs(new TFileStream(file, fmOpenRead));
  //auto fs = std::make_unique<TFileStream>(file, fmOpenRead);
  RecipientItem itm;
  int text_len;
  wchar_t smb;

  while (fs->Position < fs->Size)
	{
	  text_len = 0;

	  fs->Position += fs->Read(&itm.ID, sizeof(int));
	  fs->Position += fs->Read(&itm.ParentNodeID, sizeof(int));
	  fs->Position += fs->Read(&text_len, sizeof(int));

	  if (text_len > 0)
		{
		  int l = 0;
		  std::unique_ptr<wchar_t[]> str(new wchar_t[text_len + 1]);
		  //auto str = std::make_unique<wchar_t[]>(text_len + 1);

		  for (int i = 0; i < text_len; i++)
			{
			  fs->Position += fs->Read(&smb, sizeof(wchar_t));
			  l += swprintf_s(str.get() + l, text_len + 1, L"%c", smb);
			}

		  itm.Name = str.get();
		}

      fs->Position += fs->Read(&text_len, sizeof(int));

	  if (text_len > 0)
		{
		  int l = 0;
		  std::unique_ptr<wchar_t[]> str(new wchar_t[text_len + 1]);
		  //auto str = std::make_unique<wchar_t[]>(text_len + 1);

		  for (int i = 0; i < text_len; i++)
			{
			  fs->Position += fs->Read(&smb, sizeof(wchar_t));
			  l += swprintf_s(str.get() + l, text_len + 1, L"%c", smb);
			}

		  itm.Host = str.get();
		}

	  fs->Position += fs->Read(&text_len, sizeof(int));

	  if (text_len > 0)
		{
		  int l = 0;
		  std::unique_ptr<wchar_t[]> str(new wchar_t[text_len + 1]);
		  //auto str = std::make_unique<wchar_t[]>(text_len + 1);

		  for (int i = 0; i < text_len; i++)
			{
			  fs->Position += fs->Read(&smb, sizeof(wchar_t));
			  l += swprintf_s(str.get() + l, text_len + 1, L"%c", smb);
			}

		  itm.Port = str.get();
		}

	  AddItem(itm);
	}

  return true;
}
//---------------------------------------------------------------------------

bool TRecpientItemCollection::ImportData(TRecpientItemCollection *source)
{
  bool res = false;

  if (!source)
	throw Exception("ImportData: Empty source!");
  else
	{
	  try
		 {
		   std::vector<int> id;
		   std::unique_ptr<TStringList> grps(new TStringList());
		   //auto grps = std::make_unique<TStringList>();

		   source->SelectGroups(grps.get()); //вибираємо список груп з імпорт. книги
		   RecipientItem itm;

		   for (int i = 0; i < grps->Count; i++) //додаємо неіснуючі групи
			  {
				RecipientItem *grp = FindGroup(grps->Strings[i]);

				if (!grp)
				  {
					grp = source->FindGroup(grps->Strings[i]);

					itm.ParentNodeID = 0;
					itm.ID = NextID();
					itm.Name = grp->Name;

					AddItem(itm);
				  }

//вибираємо з імпортуємої книги всі записи у кожній групі та додаємо у існуючу книгу
				id.clear();
				source->SelectRecipientsInGroup(&id, grps->Strings[i]);
				grp = FindGroup(grps->Strings[i]);
				RecipientItem *imp_itm, *double_itm;

				for (int j = 0; j < id.size(); j++)
				   {
					 imp_itm = source->FindItem(id[j]);
					 double_itm = FindItem(imp_itm->Name); //перевіримо, чи є такий запис у поточній книзі

					 itm.ParentNodeID = grp->ID;
					 itm.ID = NextID();
					 itm.Name = imp_itm->Name;
					 itm.Host = imp_itm->Host;
					 itm.Port = imp_itm->Port;

					 if (!double_itm) //запис не знайдено
					   AddItem(itm);
					 else if (double_itm && double_itm->ParentNodeID == 0) //знайдено, але це група
					   AddItem(itm);
				   }
			  }

           res = true;
		 }
	  catch (Exception &e)
		 {
		   res = false;
		   throw Exception("ImportData: import records " + e.ToString());
		 }
	}

  return res;
}
//---------------------------------------------------------------------------

bool TRecpientItemCollection::Save()
{
  return SaveToFile(CollectionFile);
}
//---------------------------------------------------------------------------

bool TRecpientItemCollection::SaveToFile(String file)
{
  std::unique_ptr<TFileStream> fs(new TFileStream(file, fmOpenWrite|fmCreate));
  //auto fs = std::make_unique<TFileStream>(file, fmOpenWrite|fmCreate);
  fs->Position = 0;
  int text_len = 0;
  wchar_t smb;

  for (int i = 0; i < recp_items.size(); i++)
	{
	  text_len = 0;

	  fs->Position += fs->Write(&recp_items[i].ID, sizeof(int));
	  fs->Position += fs->Write(&recp_items[i].ParentNodeID, sizeof(int));
	  text_len = recp_items[i].Name.Length();
	  fs->Position += fs->Write(&text_len, sizeof(int));

	  for (int j = 0; j < text_len; j++)
		{
		  smb = recp_items[i].Name.c_str()[j];
		  fs->Position += fs->Write(&smb, sizeof(wchar_t));
		}

	  text_len = recp_items[i].Host.Length();
	  fs->Position += fs->Write(&text_len, sizeof(int));

	  for (int j = 0; j < text_len; j++)
		{
		  smb = recp_items[i].Host.c_str()[j];
		  fs->Position += fs->Write(&smb, sizeof(wchar_t));
		}

	  text_len = recp_items[i].Port.Length();
	  fs->Position += fs->Write(&text_len, sizeof(int));

	  for (int j = 0; j < text_len; j++)
		{
		  smb = recp_items[i].Port.c_str()[j];
		  fs->Position += fs->Write(&smb, sizeof(wchar_t));
		}
	}

  return true;
}
//---------------------------------------------------------------------------

void TRecpientItemCollection::CreateTree(TTreeView *view)
{
  view->Items->Clear();

  for (int i = 0; i < recp_items.size(); i++)
	 {
	   if (recp_items[i].ParentNodeID == 0)
		 recp_items[i].Node = view->Items->Add(NULL, recp_items[i].Name);
	   else if (recp_items[i].ParentNodeID > 0)
		 {
		   RecipientItem *grp = FindGroup(recp_items[i].ParentNodeID);

		   if (grp && grp->Node)
			 recp_items[i].Node = view->Items->AddChild(grp->Node, recp_items[i].Name);
		 }
	   else
		 throw Exception("CreateTree: incorrect ParentNodeID");
	 }
}
//---------------------------------------------------------------------------

void TRecpientItemCollection::CreateSortedTree(TTreeView *view)
{
  std::vector<RecipientItem*> itms;

  view->Items->Clear();

  for (int i = 0; i < recp_items.size(); i++)
	 {
	   if (recp_items[i].ParentNodeID == 0)
		 {
		   recp_items[i].Node = view->Items->Add(NULL, recp_items[i].Name);

		   recp_items[i].Node->ImageIndex = -1;

		   itms.clear();
		   SelectRecipientsInGroup(&itms, recp_items[i].ID);
		   std::sort(itms.begin(), itms.end(), ItemComparer);

		   for (int j = 0; j < itms.size(); j++)
			  {
				RecipientItem *parent = FindGroup(itms[j]->ParentNodeID);

				if (parent)
				  itms[j]->Node = view->Items->AddChild(parent->Node, itms[j]->Name);
				else
				  throw Exception("CreateSortedTree: incorrect group ID!");
			  }
		 }
	 }
}
//---------------------------------------------------------------------------

int TRecpientItemCollection::NextID()
{
  int max = 0;

  for (int i = 0; i < recp_items.size(); i++)
	{
	  if (recp_items[i].ID > max)
		max = recp_items[i].ID;
	}

  return max + 1;
}
//---------------------------------------------------------------------------

int TRecpientItemCollection::Add(int parent_id, TTreeNode *node, String name)
{
  RecipientItem itm;

  itm.ID = NextID();
  itm.ParentNodeID = parent_id;
  itm.Node = node;
  itm.Name = name;

  AddItem(itm);

  return itm.ID;
}
//---------------------------------------------------------------------------

int TRecpientItemCollection::Add(int parent_id,
								 TTreeNode *node,
								 String name,
								 String host,
								 String port)
{
  RecipientItem itm;

  itm.ID = NextID();
  itm.ParentNodeID = parent_id;
  itm.Node = node;
  itm.Name = name;
  itm.Host = host;
  itm.Port = port;

  AddItem(itm);

  return itm.ID;
}
//---------------------------------------------------------------------------

void TRecpientItemCollection::SelectGroups(TStrings *list)
{
  if (!list)
	throw Exception("SelectGroups: incorrect pointer TStrings*");

  for (int i = 0; i < recp_items.size(); i++)
	{
	  if (recp_items[i].ParentNodeID == 0)
		list->Add(recp_items[i].Name);
	}
}
//---------------------------------------------------------------------------

void TRecpientItemCollection::SelectRecipients(std::vector<int> *id_list)
{
  if (!id_list)
	throw Exception("SelectRecipients: incorrect pointer std::vector<int>*");

  for (int i = 0; i < recp_items.size(); i++)
	{
	  if (recp_items[i].ParentNodeID > 0)
		id_list->push_back(recp_items[i].ID);
	}
}
//---------------------------------------------------------------------------

void TRecpientItemCollection::SelectRecipientsInGroup(std::vector<int> *id_list,
													  int group_id)
{
  if (!id_list)
	throw Exception("SelectRecipientsInGroup: incorrect pointer std::vector<int>*");

  for (int i = 0; i < recp_items.size(); i++)
	{
	  if (recp_items[i].ParentNodeID == group_id)
		id_list->push_back(recp_items[i].ID);
	}
}
//---------------------------------------------------------------------------

void TRecpientItemCollection::SelectRecipientsInGroup(std::vector<int> *id_list,
													  String group_name)
{
  if (!id_list)
	throw Exception("SelectRecipientsInGroup: incorrect pointer std::vector<int>*");

  RecipientItem *grp = FindGroup(group_name);

  if (!grp)
	throw Exception("SelectRecipientsInGroup: incorrect group name");

  for (int i = 0; i < recp_items.size(); i++)
	{
	  if (recp_items[i].ParentNodeID == grp->ID)
		id_list->push_back(recp_items[i].ID);
	}
}
//---------------------------------------------------------------------------

void TRecpientItemCollection::SelectRecipientsInGroup(TStringList *name_list,
													  String group_name)
{
  if (!name_list)
	throw Exception("SelectRecipientsInGroup: incorrect pointer TStringList*");

  RecipientItem *grp = FindGroup(group_name);

  if (!grp)
	throw Exception("SelectRecipientsInGroup: incorrect group name");

  for (int i = 0; i < recp_items.size(); i++)
	{
	  if (recp_items[i].ParentNodeID == grp->ID)
		name_list->Add(recp_items[i].Name);
	}
}
//---------------------------------------------------------------------------

void TRecpientItemCollection::SelectRecipientsInGroup(std::vector<RecipientItem*> *itms,
													  int group_id)
{
  if (!itms)
	throw Exception("SelectRecipientsInGroup: incorrect pointer std::vector<RecipientItem>*");

  for (int i = 0; i < recp_items.size(); i++)
	{
	  if (recp_items[i].ParentNodeID == group_id)
		itms->push_back(&recp_items[i]);
	}
}
//---------------------------------------------------------------------------

void TRecpientItemCollection::SelectRecipientsInGroup(std::vector<RecipientItem*> *itms,
													  String group_name)
{
  if (!itms)
	throw Exception("SelectRecipientsInGroup: incorrect pointer std::vector<RecipientItem*>*");

  RecipientItem *grp = FindGroup(group_name);

  if (!grp)
	throw Exception("SelectRecipientsInGroup: incorrect group name");

  for (int i = 0; i < recp_items.size(); i++)
	{
	  if (recp_items[i].ParentNodeID == grp->ID)
		itms->push_back(&recp_items[i]);
	}
}
//---------------------------------------------------------------------------

void TRecpientItemCollection::DeleteRecipientsInGroup(int group_id)
{
  std::vector<int> grp_list;

  SelectRecipientsInGroup(&grp_list, group_id);

  for (int i = 0; i < grp_list.size(); i++)
	{
	  Remove(grp_list[i]);
	}
}
//---------------------------------------------------------------------------

RecipientItem *TRecpientItemCollection::FindRecipientInGroup(int group_id,
															 String name,
															 String host,
															 String port)
{
  RecipientItem *res = NULL;

  std::vector<RecipientItem*> itms;

  SelectRecipientsInGroup(&itms, group_id);

  for (int i = 0; i < itms.size(); i++)
	 {
	   if ((itms[i]->Name == name) && (itms[i]->Host == host) && (itms[i]->Port == port))
		 {
		   res = itms[i];
		   break;
         }
	 }

  return res;
}
//---------------------------------------------------------------------------

RecipientItem *TRecpientItemCollection::FindRecipientInGroup(int group_id, const String &name)
{
  RecipientItem *res = NULL;

  std::vector<RecipientItem*> itms;

  SelectRecipientsInGroup(&itms, group_id);

  for (int i = 0; i < itms.size(); i++)
	 {
	   if (itms[i]->Name == name)
		 {
		   res = itms[i];
		   break;
         }
	 }

  return res;
}
//---------------------------------------------------------------------------

void TRecpientItemCollection::Clear()
{
  int ind = recp_items.size() - 1;

  while (ind >= 0)
	{
	  delete recp_items[ind].Node;
	  recp_items.erase(recp_items.begin() + ind);
	  ind --;
	}
}
//---------------------------------------------------------------------------


