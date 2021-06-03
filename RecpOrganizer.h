/*!
Copyright 2019, 2021 Maxim Noltmeer (m.noltmeer@gmail.com)
*/
//---------------------------------------------------------------------------

#ifndef RecpOrganizerH
#define RecpOrganizerH

#include <System.Classes.hpp>
#include <Vcl.Forms.hpp>
#include <Vcl.Controls.hpp>
#include <Vcl.StdCtrls.hpp>
#include <Vcl.Dialogs.hpp>
#include <algorithm>
#include <vector>
//---------------------------------------------------------------------------

struct RecipientItem
{
  int ID;
  int ParentNodeID;
  TTreeNode *Node;
  String Name;
  String Host;
  String Port;
};

class TRecpientItemCollection
{
   private:
	 std::vector<RecipientItem> recp_items;
     String CollectionFile;

	 RecipientItem *IRead(int ind);
	 void IWrite(int ind, RecipientItem *itm);

	 int GetCount(){return recp_items.size();}

	 int GetIndexOf(int id);
	 inline void AddItem(RecipientItem item){recp_items.push_back(item);}

   public:
	 TRecpientItemCollection(){};
     TRecpientItemCollection(String file);
	 virtual ~TRecpientItemCollection();

	 int IsChildItem(TTreeNode *n);
	 int Add(int parent_id, TTreeNode *node, String name);
	 int Add(int parent_id, TTreeNode *node, String name, String host, String port);
	 void Remove(int ind);
	 void Remove(TTreeNode *n);
	 RecipientItem *Find(int id);
	 RecipientItem *Find(TTreeNode *n);
	 RecipientItem *FindItem(int id);
	 RecipientItem *FindItem(TTreeNode *n);
	 RecipientItem *FindItem(String name);
	 RecipientItem *FindGroup(String name);
	 RecipientItem *FindGroup(int id);
     RecipientItem *FindGroup(TTreeNode *n);
	 bool LoadFromFile(String file);
	 bool ImportData(TRecpientItemCollection *source);
     bool Save();
	 bool SaveToFile(String file);
	 void CreateTree(TTreeView *view);
     void CreateSortedTree(TTreeView *view);
	 void SelectGroups(TStrings *list);
	 void SelectRecipients(std::vector<int> *id_list);
     void SelectRecipientsInGroup(std::vector<int> *id_list, int group_id);
	 void SelectRecipientsInGroup(std::vector<int> *id_list, String group_name);
	 void SelectRecipientsInGroup(TStringList *name_list, String group_name);
	 void SelectRecipientsInGroup(std::vector<RecipientItem*> *itms, int group_id);
	 void SelectRecipientsInGroup(std::vector<RecipientItem*> *itms, String group_name);
	 void DeleteRecipientsInGroup(int group_id);
	 //void DeleteRecipientsInGroup(String group_name);
	 RecipientItem *FindRecipientInGroup(int group_id, String name, String host, String port);
	 RecipientItem *FindRecipientInGroup(int group_id, const String &name);
	 int NextID();
     void Clear();

	 __property RecipientItem* Items[int ind] = {read = IRead, write = IWrite};
	 __property int Count = {read = GetCount};
};
//---------------------------------------------------------------------------

bool ItemComparer(RecipientItem* a, RecipientItem* b) {return (a->Name < b->Name);}
#endif
