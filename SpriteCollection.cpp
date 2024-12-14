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

#include "SpriteCollection.h"
//---------------------------------------------------------------------------
#pragma package(smart_init)

TPicture* TSpriteCollection::FGet(const String &name)
{
  try
	 {
	   return FItems[IndexOf(name)].Image;
	 }
  catch (Exception &e)
	 {
	   e.Message = "TSpriteCollection::FGet: " + e.Message;
	   throw e;
	 }
}
//---------------------------------------------------------------------------

int TSpriteCollection::IndexOf(const String &name)
{
  int res = -1;

  for (int i = 0; i < FItems.size(); i++)
	 {
	   if (FItems[i].Name == name)
		 {
		   res = i;
		   break;
		 }
	 }

  return res;
}
//---------------------------------------------------------------------------

void TSpriteCollection::Clear()
{
  try
	 {
	   for (int i = 0; i < FItems.size(); i++)
		  {
			if (FItems[i].Image)
	     	  delete FItems[i].Image;
		  }

	   FItems.clear();
	 }
  catch (Exception &e)
	 {
	   e.Message = "TSpriteCollection::Clear: " + e.Message;
	   throw e;
	 }
}
//---------------------------------------------------------------------------

void TSpriteCollection::AddSprite(const String &img_file, const String &sprite_name)
{
  try
	 {
       if (IndexOf(sprite_name) >= 0)
		 throw Exception("Sprite with name '" + sprite_name + "' already exists!");

	   Sprite sprite;

	   sprite.Image = new TPicture();
	   sprite.Image->LoadFromFile(img_file);
	   sprite.Name = sprite_name;

	   FItems.push_back(sprite);
	 }
  catch (Exception &e)
	 {
	   e.Message = "TSpriteCollection::AddSprite: " + e.Message;
	   throw e;
	 }
}
//---------------------------------------------------------------------------

void TSpriteCollection::AddSprite(TPicture *image, const String &sprite_name)
{
  try
	 {
	   if (IndexOf(sprite_name) >= 0)
		 throw Exception("Sprite with name '" + sprite_name + "' already exists!");

	   Sprite sprite;

	   sprite.Image = image;
	   sprite.Name = sprite_name;

	   FItems.push_back(sprite);
	 }
  catch (Exception &e)
	 {
	   e.Message = "TSpriteCollection::AddSprite: " + e.Message;
	   throw e;
	 }
}
//---------------------------------------------------------------------------

void TSpriteCollection::RemoveSprite(const String &name)
{
  try
	 {
	   RemoveSprite(IndexOf(name));
	 }
  catch (Exception &e)
	 {
	   e.Message = "TSpriteCollection::RemoveSprite: " + e.Message;
	   throw e;
	 }
}
//---------------------------------------------------------------------------

void TSpriteCollection::RemoveSprite(int ind)
{
  try
	 {
	   if ((ind < 0) || (ind >= FItems.size()))
		 throw Exception("List index out of bounds");

	   Sprite sprite = FItems[ind];

	   if (sprite.Image)
	     delete sprite.Image;

	   FItems.erase(FItems.begin() + ind);
	 }
  catch (Exception &e)
	 {
	   throw e;
	 }
}
//---------------------------------------------------------------------------

