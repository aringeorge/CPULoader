#include "LoadActivity.h"
#include "LoadRunner.h"

#if defined(__WIN__)
   #include <Windows.h>
#elif defined(__LIN__)
   #include <unistd.h>
#endif

#include <iostream>
#include <sstream>

using std::cout;
using std::endl;
using std::stringstream;

int main(int argc, char *argv[])
{
   if (argc != 3)
   {
      cout << "Error in command line." << endl;
      cout << "CommandLine:  " << argv[0] << " #Threads #DurationInMinutes" << endl;
      return -1;
   }
   int nNumActivities = 0;
   stringstream strStrVal;
   strStrVal.str(argv[1]);
   strStrVal >> nNumActivities;
   if ((nNumActivities < 1) || (nNumActivities > 16))
   {
      cout << "Error in argument list." << endl;
      cout << "Number of threads should be greater than 0 and less than 17" << endl;
      return -1;
   }
   int nWaitTime = 0;
   strStrVal.clear();
   strStrVal.str(argv[2]);
   strStrVal >> nWaitTime;
   if ((nWaitTime < 1) || (nWaitTime > 1440))
   {
      cout << "Error in argument list." << endl;
      cout << "Number of minutes should be greater than 0 and less than 1441" << endl;
      return -1;
   }
   
   CLoadActivity *sActivities = new CLoadActivity[nNumActivities];
   CLoadRunner *sRunners = new CLoadRunner[nNumActivities];
   for (int i = 0; i < nNumActivities; i++)
   {
      ActivityTypes sAct;
      sAct.sAct = ActivityTypes::CalculateSines;
      sActivities[i].SetActivityType(sAct);
      sRunners[i].ProcessingThreadProc(sActivities + i);
      sRunners[i].ProcessorAffinity(i + 1);
   }
   for (int i = 0; i < nNumActivities; i++)
   {
      sRunners[i].Start();
   }
   int lSecondsToSleep = nWaitTime * 60;
#if __LIN__
   int lSecsLeft = sleep(lSecondsToSleep);
#elif __WIN__
   int lSecsLeft = 0;
   Sleep(lSecondsToSleep * 1000);
#endif
   cout << "System ran for: " << lSecondsToSleep - lSecsLeft << " seconds" << endl;
   for (int i = 0; i < nNumActivities; i++)
   {
      sRunners[i].Stop();
   }
   delete []sActivities;
   delete []sRunners;
   return 0;
}