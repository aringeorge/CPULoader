#pragma once

#include "LoadActivity.h"

#if defined(__WIN__)
   #include <Windows.h>
#elif defined(__LIN__)
   #include <unistd.h>
   #include <pthread.h>
#endif

class CLoadRunner
{
protected:
   bool m_bRunning;
   unsigned long m_ulProcAffinity;
   CLoadActivity *m_sWorker;
   
#if defined(__WIN__)
   HANDLE m_hThreadHandle;
#elif defined(__LIN__)
   pthread_t m_hThreadHandle;
#endif
   

#if defined(__WIN__)
   static DWORD WINAPI LoadThread(LPVOID lpvData);
#elif defined(__LIN__)
   static void *LoadThread(void *lpvData);
#endif   
   

public:
   CLoadRunner();
   ~CLoadRunner();
   void ProcessorAffinity(int nProcessor);
   void ProcessingThreadProc(CLoadActivity *sActivity);
   void Start();
   void Stop();
};