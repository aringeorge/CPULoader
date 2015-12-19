#include "LoadActivity.h"

#include <cmath>

CLoadActivity::CLoadActivity()
{
   m_sActivity.sAct = ActivityTypes::Empty;
}

CLoadActivity::~CLoadActivity()
{
}

void CLoadActivity::SetActivityType(ActivityTypes sAct)
{
   m_sActivity.sAct = sAct.sAct;
}

void CLoadActivity::EmptyLoop()
{
   // there is nothing here
   //__asm { nop };
}

void CLoadActivity::CalculateSines()
{
   int nArraySize = 32767;
   double fTwoPi = (2 * 3.141592);
   double fDivider = nArraySize / fTwoPi;
   double *fArrayOfData = new double[nArraySize];
   for (int i = 0; i < nArraySize; i++)
   {
      fArrayOfData[i] = sin(i / fDivider);
   }
   delete []fArrayOfData;
}

void CLoadActivity::RunActivity()
{
   switch (m_sActivity.sAct)
   {
   case ActivityTypes::Empty:
      EmptyLoop();
      break;
   case ActivityTypes::CalculateSines:
      CalculateSines();
      break;
   }
}