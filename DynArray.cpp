// DynArray.cpp: implementation of the CDynArray class.
//
//////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "zsIRC.h"
#include "DynArray.h"

#ifdef _DEBUG
#undef THIS_FILE
static char THIS_FILE[]=__FILE__;
#define new DEBUG_NEW
#endif

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

CDynArray::CDynArray()
{
  pItems = NULL;
  nItems = 0;
}

CDynArray::~CDynArray()
{
  Flush();
}

void CDynArray::Add(ARRAYITEM a) {
  if (!nItems) {
    ARRAYITEM * pNew = new ARRAYITEM[1];
    pNew[0] = a;
    nItems = 1;
    pItems = pNew;
    return;
  }

  ARRAYITEM * pNew = new ARRAYITEM[nItems+1];
  CopyMemory(pNew,pItems,sizeof(ARRAYITEM)*nItems);
  pNew[nItems] = a;

  if (pItems) delete[] pItems;
  pItems = pNew;
  nItems++;
}

void CDynArray::Delete(int i) {
  if (!nItems) return;

  delete pItems[i];

  if (nItems == 1) {
    if (pItems) delete[] pItems;
    nItems = 0;
    return;
  } 
   
  ARRAYITEM * pNew = new ARRAYITEM[nItems-1];
  CopyMemory(pNew,pItems,sizeof(ARRAYITEM)*i);
  CopyMemory(pNew+i,pItems+i+1,sizeof(ARRAYITEM)*(nItems-i-1));

  if (pItems) delete[] pItems;
  pItems = pNew;
  nItems--;
  
}

ARRAYITEM CDynArray::operator[] (int i) {
  return pItems[i];
}

int CDynArray::Num() {
  return nItems;
}

void CDynArray::Flush() {
  int nMsg = Num();
  for (int i=0; i<nMsg; i++)
    Delete(0);
//  for (int i=0; i<nItems; i++)
//    delete pItems[i];

//  if (pItems) delete[] pItems;
  pItems = NULL;
  nItems = 0;
}

void CDynArray::Sort(DYNARRAY_SORTCALLBACK cb) {
  if (!nItems) return;

  qsort(pItems,nItems,sizeof(ARRAYITEM),cb);
}

int CDynArray::Find(ARRAYITEM a) {
  int nMsg = Num();
  for (int i=0; i<nMsg; i++)
    if (a == pItems[i])
      return i;
 
  return -1;
}