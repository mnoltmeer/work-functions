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

#ifndef SpriteCollectionH
#define SpriteCollectionH
//---------------------------------------------------------------------------

#include <System.Classes.hpp>
#include <Vcl.Imaging.pngimage.hpp>
#include <Vcl.Graphics.hpp>

#include <vector>
//---------------------------------------------------------------------------

struct Sprite
{
  TPicture *Image;
  String Name;
};

class TSpriteCollection
{
private:
	std::vector<Sprite> FItems;

	TPicture *FGet(const String &name);

    void Clear();
	int IndexOf(const String &name);

public:
	TSpriteCollection(){};
	inline virtual ~TSpriteCollection(){Clear();}

	void AddSprite(const String &img_file, const String &sprite_name);
	void AddSprite(TPicture *image, const String &sprite_name);
	void RemoveSprite(int ind);
	void RemoveSprite(const String &name);

	__property TPicture *Items[const String &name] = {read = FGet};
};
#endif
