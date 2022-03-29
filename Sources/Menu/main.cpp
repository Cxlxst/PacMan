#ifndef _IRR_WINDOWS_
#pragma comment(lib, "Irrlicht.lib")
#pragma comment(linker, "/ssubsystem:windows /ENTRY:mainCRTStartup")
#endif

#include <iostream>
#include <irrlicht.h>

using namespace irr;
using namespace core;
using namespace scene;
using namespace video;
using namespace io;
using namespace gui;


// Structure qui stocke le contexte du receveur d'event
struct SAppContext
{
  IrrlichtDevice *device;
  s32             counter;
  IGUIListBox*    listbox;
};

  
// Receveur d'event
class MyEventReceiver : public IEventReceiver
{
public:
  virtual bool OnEvent(const SEvent& event)
  {
    // Se souvient si la touche est enfoncée ou relâchée
    if (event.EventType == irr::EET_KEY_INPUT_EVENT)
      KeyIsDown[event.KeyInput.Key] = event.KeyInput.PressedDown;
    
    return false;
  }
  
  // Vérifie si une touche est enfoncée
  virtual bool IsKeyDown(EKEY_CODE Key) const
  {
    return KeyIsDown[Key];
  }

private:
  // Nous utilisons ce tableau pour stocker l'état de chaque touche.
  bool KeyIsDown[KEY_KEY_CODES_COUNT];
};


int main()
{
  int check_menu = 1;

  MyEventReceiver receiver;
  
  // Création de la fenêtre
  IrrlichtDevice *device =
    createDevice( video::EDT_SOFTWARE, dimension2d<u32>(1920, 1080), 16,
                  false, false, false, &receiver);

  if (!device)
    return 1;

  device->setWindowCaption(L"PacMan - Irrlicht Engine Demo");

  IVideoDriver* driver = device->getVideoDriver();
  ISceneManager* smgr = device->getSceneManager();
  IGUIEnvironment* guienv = device->getGUIEnvironment();



  while(device->run())
    {
      driver->beginScene(true, true, SColor(255,100,101,140));

      //MENU_JOUER
      if(check_menu >= 1 && check_menu <= 8)
	 {
	   guienv->clear();
	   device->getGUIEnvironment()->addImage(driver->getTexture("Menu_1.jpg"), core::position2d<s32>(0,0));
	   gui::IGUIStaticText* diagnostics = device->getGUIEnvironment()->addStaticText(L"", core::rect<s32>(10, 10, 400, 20));
	   diagnostics->setOverrideColor(video::SColor(255, 255, 255, 0));
	 }
      
      //MENU_OPTIONS
       if(check_menu >= 9 && check_menu <=16)
	 {
	   guienv->clear();
	   device->getGUIEnvironment()->addImage(driver->getTexture("Menu_2.jpg"), core::position2d<s32>(0,0));
	   gui::IGUIStaticText* diagnostics = device->getGUIEnvironment()->addStaticText( L"", core::rect<s32>(10, 10, 400, 20));
	   diagnostics->setOverrideColor(video::SColor(255, 255, 255, 0));
	 }

       //MENU_EXIT
       if(check_menu >= 17 && check_menu <= 24)
	 {
	   guienv->clear();
	   device->getGUIEnvironment()->addImage(driver->getTexture("Menu_3.jpg"), core::position2d<s32>(0,0));
	   gui::IGUIStaticText* diagnostics = device->getGUIEnvironment()->addStaticText(L"", core::rect<s32>(10, 10, 400, 20));
	   diagnostics->setOverrideColor(video::SColor(255, 255, 255, 0));
	 }
   
       if(receiver.IsKeyDown(irr::KEY_DOWN))
	 {
	   check_menu = check_menu + 1;
	   std::cout << "check_menu = " << check_menu << std::endl;
	 }
       if(receiver.IsKeyDown(irr::KEY_UP))
	 {
	   check_menu = check_menu - 1;
	   std::cout << "check_menu = " << check_menu << std::endl;
	 }

       if (check_menu == 25)
	 check_menu = 1;
       if (check_menu == 0)
	 check_menu = 24;
       
       smgr->drawAll();
       guienv->drawAll();
       driver->endScene();
    }
  device->drop();
  
  return 0;
}
  

  
