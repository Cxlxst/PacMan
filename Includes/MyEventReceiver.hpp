#ifndef EVENT_HPP_
#define EVENT_HPP

#include <irrlicht.h>
#include <stdio.h>

using namespace irr;

class MyEventReceiver : public IEventReceiver
{
public:
  virtual bool OnEvent(const SEvent& event)
  {
    // Se souvient si la key est pressed ou non                             
    if (event.EventType == EET_KEY_INPUT_EVENT)
      KeyIsDown[event.KeyInput.Key] = event.KeyInput.PressedDown;

    return false;
  }

  // Check si la key est appuyé en continu ou non
  virtual bool IsKeyDown(EKEY_CODE keyCode) const
  {
    return KeyIsDown[keyCode];
  }

  MyEventReceiver()
  {
    for (u32 i=0; i<KEY_KEY_CODES_COUNT; ++i)
      KeyIsDown[i] = false;
  }

private:
  // Stock l'état de la key               
  bool KeyIsDown[KEY_KEY_CODES_COUNT];
};

#endif
