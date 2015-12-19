#pragma once

struct ActivityTypes
{
   enum __ActivityType
   {
      Empty,
      CalculateSines,
   };
   __ActivityType sAct;
};

class CLoadActivity
{
protected:
   ActivityTypes m_sActivity;

   void EmptyLoop();
   void CalculateSines();

public:
   CLoadActivity();
   ~CLoadActivity();
   void SetActivityType(ActivityTypes sAct);
   void RunActivity();
};