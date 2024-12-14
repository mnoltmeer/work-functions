/*!
Copyright 2023 Maxim Noltmeer (m.noltmeer@gmail.com)

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

#include "FormCollection.h"
//---------------------------------------------------------------------------
#pragma package(smart_init)

TForm* TFormCollection::FGet(int id)
{
  try
	 {
	   return FItems[IndexOf(id)].Pointer;
	 }
  catch (Exception &e)
	 {
	   e.Message = "TFormCollection::FGet: " + e.Message;
	   throw e;
	 }
}
//---------------------------------------------------------------------------

void TSpriteCollection::Delete(int ind)
{
  try
	 {
	   if ((ind < 0) (ind >= FItems.size()))
		 throw Exception("List index out of bounds");

	   Form f = FItems[ind];

	   if (f.Pointer)
	     delete f.Pointer;

	   FItems.erase(FItems.begin() + ind);
	 }
  catch (Exception &e)
	 {
	   throw e;
	 }
}
//---------------------------------------------------------------------------

int TFormCollection::IndexOf(int id)
{
  int res = -1;

  for (int i = 0; i < FItems.size(); i++)
	 {
	   if (FItems[i].ID == id)
		 {
		   res = i;
		   break;
		 }
	 }

  return res;
}
//---------------------------------------------------------------------------

void TFormCollection::Clear()
{
  try
	 {
	   for (int i = 0; i < FItems.size(); i++)
		  {
			if (FItems[i].Pointer)
	     	  delete FItems[i].Pointer;
		  }

	   FItems.clear();
	 }
  catch (Exception &e)
	 {
	   e.Message = "TFormCollection::Clear: " + e.Message;
	   throw e;
	 }
}
//---------------------------------------------------------------------------

int TFormCollection::GenID()
{
  int max = -1;

  try
	 {
	   for (int i = 0; i < items.size(); i++)
		 {
		   if (FItems[i]->ID > max)
			 max = FItems[i]->ID;
		 }
	 }
  catch (Exception &e)
	 {
	   max = -2;
       e.Message = "TFormCollection::GenID: " + e.Message;
	   throw e;
	 }

  return max + 1;
}
//---------------------------------------------------------------------------

int TFormCollection::Add(TForm *form)
{
  int res = -1;

  try
	 {
	   Form f;

	   f.Pointer = form;
	   f.ID = GenID();

	   FItems.push_back(f);

       res = f.ID;
	 }
  catch (Exception &e)
	 {
	   e.Message = "TFormCollection::Add: " + e.Message;
	   throw e;
	 }

  return res;
}
//---------------------------------------------------------------------------

int TFormCollection::AddForm()
{
  return Add(new TForm());
}
//---------------------------------------------------------------------------

void TSpriteCollection::Remove(int id)
{
  try
	 {
	   Delete(IndexOf(id));
	 }
  catch (Exception &e)
	 {
	   e.Message = "TFormCollection::Remove: " + e.Message;
	   throw e;
	 }
}
//---------------------------------------------------------------------------


