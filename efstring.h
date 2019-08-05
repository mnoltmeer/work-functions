/*!
Copyright 2019 Maxim Noltmeer (m.noltmeer@gmail.com)
Copyright 2015 Elsa Fenrich (elsa.fenrich@gmail.com)

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

#ifndef EFSTRING_H_INCLUDED
#define EFSTRING_H_INCLUDED

#include <stdio.h>
#include <string>

#define UINT unsigned int

//!             Ansi версии функций

//выводит на экран коды символов строки
void show_str_symb(const char *str)
{
  printf("string '%s' = \n", str);

  for (UINT i = 0; i < strlen(str); i++)
     printf("[%d] -> char(%d)\n", i, (unsigned int)str[i]);
};

//возвращает истину, если достигнут нуль-терминатор
bool strend(const char *str)
{
  return (*str == '\0');
};

//обнуляет строку
void nullstr(char *str)
{
  while (!strend(str))
    {
      *str = 0;
      str++;
    }
}

//выставляет указатель строки в позицию n_pos и возвращает его
char *setpstr(char *str, UINT n_pos)
{
  try
     {
       UINT proceed = 0;

       while (proceed < n_pos)
         {
           str++;
           proceed++;
         }

       return str;
     }
  catch(...)
     {
       return NULL;
     }
}

char *setpstrend(char *str)
{
  try
     {
       while (*str != '\0')
         {
           str++;
         }

       return str;
     }
  catch(...)
     {
       return NULL;
     }
}

//ищет символ c в строке str и возвращает его позицию
int strcpos(char *str, char c)
{
  int n_pos = 0;

  while (!strend(str))
    {
      if (*str == c)
        return n_pos;

      str++;
      n_pos++;
    }

  return -1;
};

//вставляет символ c_ins в позицию строки str, где находится символ c_move
bool strcinsc(char *str, char c_move, char c_ins)
{
  try
     {
       char *pstr = setpstr(str, strcpos(str, c_move));
       if (pstr)
         {
           *pstr = c_ins;
           return true;
         }
     }
  catch(...)
     {
       return false;
     }

   return false;
};

//вставляет символ c_ins в позицию n_pos строки str
bool strcinsn(char *str, char c_ins, UINT n_pos)
{
  try
     {
       *(str + n_pos) = c_ins;
       return true;
     }
  catch(...)
     {
       return false;
     }

};

//копирует символы из строки from_str в to_str пока не встретится символ с
int strcpywc(char *from_str, char *to_str, char c)
{
  int proceed = -1;

  char *pos = setpstr(from_str, strcpos(from_str, c));

  if (pos)
    {
      proceed = 0;
      while (from_str < pos)
        {
          *to_str = *from_str;
          from_str++;
          to_str++;
          proceed++;
        }

//добавим нуль-терминатор в конец выходной строки
      *to_str = '\0';
    }

  return proceed;
};

//копирует символы из строки from_str в to_str пока позиция не n_pos
UINT strcpywn(char *from_str, char *to_str, UINT n_pos)
{
  UINT proceed = 0;

  while (proceed < n_pos)
    {
      *to_str = *from_str;
      from_str++;
      to_str++;
      proceed++;
    }

//добавим нуль-терминатор в конец выходной строки
  *to_str = '\0';

  return proceed;
};

//копирует n_count символов из строки from_str в to_str начиная с позиции n_pos
void strncopy(char *from_str, char *to_str, UINT n_pos, UINT n_count)
{
//выставляем указатель на указанную позицию и копируем в to_str
  char *pos = from_str + n_pos;
  strcpywn(pos, to_str, n_count);
};

//копирует n_count символов из строки from_str в to_str начиная с позиции символа c
void strnccopy(char *from_str, char *to_str, char c, UINT n_count)
{
//выставляем указатель на указанную позицию и копируем в to_str
  char *pos = setpstr(from_str, strcpos(from_str, c));
  strcpywn(pos, to_str, n_count);
};

//определяет количество вхождений символа с в строку str
UINT strccount(const char *str, char c)
{
  UINT c_count = 0;

  while (!strend(str))
    {
      if (c == *str++)
        c_count++;
    }

  return c_count;
};

//определяет входит ли строка fnd_str в строку in_str, начиная с позиции n_pos
bool strincl(const char *in_str, const char *fnd_str, UINT n_pos)
{
  UINT scount = strlen(fnd_str);

  for (UINT i = 0; i < scount; i++)
    {
      if (in_str[n_pos + i] != fnd_str[i])
        return false;
    }

  return true;
}

//определяет входит ли строка fnd_str в строку in_str, начиная с позиции n_pos
bool striincl(const char *in_str, const char *fnd_str, UINT n_pos)
{
  UINT scount = strlen(fnd_str);

  std::string str = in_str;

  if (stricmp(str.substr(n_pos, scount).c_str(), fnd_str) == 0)
    return true;
  else
    return false;
}

//убирает пробелы из начала строки
const char *ltrim(const char *str)
{
  while ((' ' == *str) || (char(9) == *str))
    {
      str++;
    }

  return str;
}

//!---------------------------------------------------------------------------------------------------

//!             Unicode версии функций

//выводит на экран коды символов строки
void _wshow_str_symb(const wchar_t *str)
{
  wprintf(L"wstring '%s' = \n", str);

  for (UINT i = 0; i < wcslen(str); i++)
     wprintf(L"[%d] -> wchar_t(%d)\n", i, (unsigned int)str[i]);
};

//возвращает истину, если достигнут нуль-терминатор
bool _wstrend(const wchar_t *str)
{
  return (*str == '\0');
};

//обнуляет строку
void _wnullstr(wchar_t *str)
{
  while (!_wstrend(str))
    {
      *str = 0;
      str++;
    }
}

//выставляет указатель строки в позицию n_pos и возвращает его
wchar_t *_wsetpstr(wchar_t *str, UINT n_pos)
{
  try
     {
       UINT proceed = 0;

       while (proceed < n_pos)
         {
           str++;
           proceed++;
         }

       return str;
     }
  catch(...)
     {
       return NULL;
     }
}

wchar_t *_wsetpstrend(wchar_t *str)
{
  try
     {
       while (*str != '\0')
         {
           str++;
         }

       return str;
     }
  catch(...)
     {
       return NULL;
     }
}

//ищет символ c в строке str и возвращает его позицию
int _wstrcpos(wchar_t *str, wchar_t c)
{
  int n_pos = 0;

  while (!_wstrend(str))
    {
      if (*str == c)
        return n_pos;

      str++;
      n_pos++;
    }

  return -1;
};

//вставляет символ c_ins в позицию строки str, где находится символ c_move
bool _wstrcinsc(wchar_t *str, wchar_t c_move, wchar_t c_ins)
{
  try
     {
       wchar_t *pstr = _wsetpstr(str, _wstrcpos(str, c_move));
       if (pstr)
         {
           *pstr = c_ins;
           return true;
         }
     }
  catch(...)
     {
       return false;
     }

   return false;
};

//вставляет символ c_ins в позицию n_pos строки str
bool _wstrcinsn(wchar_t *str, wchar_t c_ins, UINT n_pos)
{
  try
     {
       *(str + n_pos) = c_ins;
       return true;
     }
  catch(...)
     {
       return false;
     }

};

//копирует символы из строки from_str в to_str пока не встретится символ с
int _wstrcpywc(wchar_t *from_str, wchar_t *to_str, wchar_t c)
{
  int proceed = -1;

  wchar_t *pos = _wsetpstr(from_str, _wstrcpos(from_str, c));

  if (pos)
    {
      proceed = 0;
      while (from_str < pos)
        {
          *to_str = *from_str;
          from_str++;
          to_str++;
          proceed++;
        }

//добавим нуль-терминатор в конец выходной строки
      *to_str = '\0';
    }

  return proceed;
};

//копирует символы из строки from_str в to_str пока позиция не n_pos
UINT _wstrcpywn(wchar_t *from_str, wchar_t *to_str, UINT n_pos)
{
  UINT proceed = 0;

  while (proceed < n_pos)
    {
      *to_str = *from_str;
      from_str++;
      to_str++;
      proceed++;
    }

//добавим нуль-терминатор в конец выходной строки
  *to_str = '\0';

  return proceed;
};

//копирует n_count символов из строки from_str в to_str начиная с позиции n_pos
void _wstrncopy(wchar_t *from_str, wchar_t *to_str, UINT n_pos, UINT n_count)
{
//выставляем указатель на указанную позицию и копируем в to_str
  wchar_t *pos = from_str + n_pos;
  _wstrcpywn(pos, to_str, n_count);
};

//копирует n_count символов из строки from_str в to_str начиная с позиции символа c
void _wstrnccopy(wchar_t *from_str, wchar_t *to_str, wchar_t c, UINT n_count)
{
//выставляем указатель на указанную позицию и копируем в to_str
  wchar_t *pos = _wsetpstr(from_str, _wstrcpos(from_str, c));
  _wstrcpywn(pos, to_str, n_count);
};

//определяет количество вхождений символа с в строку str
UINT _wstrccount(const wchar_t *str, wchar_t c)
{
  UINT c_count = 0;

  while (!_wstrend(str))
    {
      if (c == *str++)
        c_count++;
    }

  return c_count;
};

//определяет входит ли строка fnd_str в строку in_str, начиная с позиции n_pos
bool _wstrincl(const wchar_t *in_str, const wchar_t *fnd_str, UINT n_pos)
{
  UINT scount = wcslen(fnd_str);

  for (UINT i = 0; i < scount; i++)
    {
	  if (in_str[n_pos + i] != fnd_str[i])
        return false;
    }

  return true;
}

//определяет входит ли строка fnd_str в строку in_str, начиная с позиции n_pos
bool _wstriincl(const wchar_t *in_str, const wchar_t *fnd_str, UINT n_pos)
{
  UINT scount = wcslen(fnd_str);

  std::wstring str = in_str;

  if (_wcsicmp(str.substr(n_pos, scount).c_str(), fnd_str) == 0)
    return true;
  else
    return false;
}

//убирает пробелы из начала строки
const wchar_t *_wltrim(const wchar_t *str)
{
  while ((' ' == *str) || (wchar_t(9) == *str))
    {
      str++;
    }

  return str;
}
#endif // EFSTRING_H_INCLUDED
