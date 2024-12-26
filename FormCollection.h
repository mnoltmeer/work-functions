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

#ifndef FormCollectionH
#define FormCollectionH
//---------------------------------------------------------------------------

#include <System.Classes.hpp>
#include <Vcl.Controls.hpp>
#include <Vcl.StdCtrls.hpp>
#include <Vcl.Forms.hpp>

#include <vector>
//---------------------------------------------------------------------------

struct Form
{
  TForm *Pointer;
  int ID;
};

class TFormCollection
{
private:
	std::vector<Form> FItems;

	TForm *FGet(int id);

    int IndexOf(int id);
    void Delete(int ind);
    void Clear();
    int GenID();

public:
	TFormCollection(){};
	inline virtual ~TFormCollection(){Clear();}

	int AddNewForm(TForm *owner);
	int AddNewForm(HWND parent_window);
	int Add(TForm *form);
	void Remove(int id);


	__property TForm *Items[int id] = {read = FGet};
};
#endif
