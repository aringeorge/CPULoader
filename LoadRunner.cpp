#include "LoadRunner.h"

#include <iostream>

using std::cout;
using std::endl;

/*
 * Yes the IF-DEFS are that retarded.
 */

CLoadRunner::CLoadRunner()
{
   m_bRunning = false;
   m_ulProcAffinity = 0;
   m_sWorker = 0;
   
#if defined(__WIN__)
   m_hThreadHandle = INVALID_HANDLE_VALUE;
#elif defined(__LIN__)
   m_hThreadHandle = -1;
#endif
   
}

CLoadRunner::~CLoadRunner()
{
   if (m_bRunning == true)
   {
      m_bRunning = false;
      
#if defined(__WIN__)
      WaitForSingleObject(m_hThreadHandle, INFINITE);
#elif defined(__LIN__)
      pthread_join(m_hThreadHandle, NULL);
#endif
      
   }
}

#if defined(__WIN__)
DWORD WINAPI CLoadRunner::LoadThread(LPVOID lpvData)
#elif defined(__LIN__)
void *CLoadRunner::LoadThread(void *lpvData)
#endif

{
   CLoadRunner *sDat = (CLoadRunner *)(lpvData);
   while (sDat->m_bRunning == true)
   {
      sDat->m_sWorker->RunActivity();
   }
   
#if defined(__WIN__)
   return 0;
#elif defined(__LIN__)
   return NULL;
#endif
   
}

void CLoadRunner::ProcessorAffinity(int nProcessor)
{
#if defined(__WIN__)
   m_ulProcAffinity = 1 << (nProcessor - 1);
#elif defined(__LIN__)
   m_ulProcAffinity = (nProcessor - 1);
#endif
}

void CLoadRunner::ProcessingThreadProc(CLoadActivity *sActivity)
{
   m_sWorker = sActivity;
}

void CLoadRunner::Start()
{
   m_bRunning = true;
#if defined(__WIN__)
   m_hThreadHandle = CreateThread(NULL, 0, LoadThread, this, 0, NULL);
#elif defined(__LIN__)
   int lRet = pthread_create(&m_hThreadHandle, NULL, LoadThread, this);
#endif
   
#if defined(__WIN__)
   if (m_hThreadHandle == INVALID_HANDLE_VALUE)
#elif defined(__LIN__)
   if (lRet != 0)
#endif

   {
      
#if defined(__WIN__)
      MessageBox(NULL, "Create Thread Failed", "Error", MB_OK);
#elif defined(__LIN__)
      cout << "Create Thread Failed" << endl;
#endif
      
   }
   
#if defined(__WIN__)
   if (SetThreadAffinityMask(m_hThreadHandle, m_ulProcAffinity) == 0)
#elif defined(__LIN__)
   cpu_set_t sCPUSet;
   CPU_ZERO(&sCPUSet);
   CPU_SET(m_ulProcAffinity, &sCPUSet);
   if (pthread_setaffinity_np(m_hThreadHandle, sizeof(cpu_set_t), &sCPUSet) != 0)
#endif
      
   {
      
#if defined(__WIN__)
      MessageBox(NULL, "Set Thread Affinity Failed", "Error", MB_OK);
#elif defined(__LIN__)
      cout << "Set Thread Affinity Failed" << endl;
#endif
      
   }

}

void CLoadRunner::Stop()
{
   m_bRunning = false;
   
#if defined(__WIN__)
   WaitForSingleObject(m_hThreadHandle, INFINITE);
#elif defined(__LIN__)
   pthread_join(m_hThreadHandle, NULL);
#endif
      
}