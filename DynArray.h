// DynArray.h: interface for the CDynArray class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_DYNARRAY_H__2F36DB61_69E1_4FA1_9FFB_88032AD4918F__INCLUDED_)
#define AFX_DYNARRAY_H__2F36DB61_69E1_4FA1_9FFB_88032AD4918F__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

typedef void * ARRAYITEM;

typedef int (__cdecl * DYNARRAY_SORTCALLBACK)(const void*,const void*);

class CDynArray  
{
public:
	CDynArray();
	virtual ~CDynArray();

  void Add(ARRAYITEM);
  void Delete(int);
  int Num();
  void Flush();
  void Sort(DYNARRAY_SORTCALLBACK);
  int Find(ARRAYITEM a);

  ARRAYITEM operator[] (int);
  
private:
  ARRAYITEM * pItems;
  int nItems;
};

#endif // !defined(AFX_DYNARRAY_H__2F36DB61_69E1_4FA1_9FFB_88032AD4918F__INCLUDED_)
