#include <irrlicht.h>
#include "driverChoice.h"

using namespace irr;
using namespace core;
using namespace scene;
using namespace io;
using namespace gui;
using namespace video;

#ifdef _IRR_WINDOWS_
#pragma comment(lib, "Irrlicht.lib")
#pragma comment(linker,/subsystem:windows /ENTRY:int mainCRTStartup)
#endif
#ifdef _MSC_VER
#define _CRT_SECURE_NO_WARNINGS
#pragma comment(lib, "Irrlicht.lib")
#endif


class MyEventReceiver : public IEventReceiver
{
public:
  virtual bool OnEvent(const SEvent& event)
  {
    // Remember whether each key is down or up
    if (event.EventType == irr::EET_KEY_INPUT_EVENT)
      KeyIsDown[event.KeyInput.Key] = event.KeyInput.PressedDown;
    
    return false;
  }
  
  // This is used to check whether a key is being held down
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
  // We use this array to store the current state of each key
  bool KeyIsDown[KEY_KEY_CODES_COUNT];
};


int main()
{
  // ask user for driver
  video::E_DRIVER_TYPE driverType= driverChoiceConsole();
  if (driverType==video::EDT_COUNT)
    return 1;
  
  // create device
  MyEventReceiver receiver;

  IrrlichtDevice* device = createDevice(driverType, core::dimension2d<u32>(1920, 1280), 16, false, false, false, &receiver);

  if (device == 0)
    return 1; // could not create selected driver.

  video::IVideoDriver* driver = device->getVideoDriver();
  scene::ISceneManager* smgr = device->getSceneManager();

  IAnimatedMesh* mesh = smgr-> getMesh("../../Media/Map/map.obj");
  IAnimatedMesh* mesh2 = smgr->getMesh("../../Media/Personnages/pacman_model.obj");

  if (!mesh)
    {
      device->drop();
      return 1;
    }

  if (!mesh2)
    {
      device->drop();
      return 1;
    }

  IAnimatedMeshSceneNode* node = smgr->addAnimatedMeshSceneNode( mesh );
  if (node)
    {
      node->setMaterialFlag(EMF_LIGHTING,false);
      node->setMD2Animation(scene::EMAT_STAND);
      node->setMaterialTexture( 0, driver->getTexture("../../Media/Map/texture_map.jpg"));
    }

  IAnimatedMeshSceneNode* node2 = smgr->addAnimatedMeshSceneNode( mesh2 );
  if (node2)
    {
      smgr->addCameraSceneNode(0, vector3df(0,30,10), vector3df(0,0,0));
      irr::video::IRenderTarget::TargetType();
      //smgr->addCameraSceneNode(0, vector3df(10,50,-40), vector3df(0,0,0));
      //smgr->addCameraSceneNodeFPS();
      node2->setPosition(core::vector3df(0,3,0));
      node2->setMaterialFlag(EMF_LIGHTING,false);
      node2->setMD2Animation(scene::EMAT_STAND);
      node2->setMaterialTexture( 0, driver->getTexture("../../Media/Personnages/texture_pacman.jpg"));
      node2->setScale(core::vector3df (0.2,0.2,0.2));
    }

 

  
  gui::IGUIStaticText* diagnostics = device->getGUIEnvironment()->addStaticText(L"", core::rect<s32>(10, 10, 400, 20));
  diagnostics->setOverrideColor(video::SColor(255, 255, 255, 0));

  int lastFPS = -1;

  u32 then = device->getTimer()->getTime();

	
  const f32 MOVEMENT_SPEED = 5.f;

  while(device->run())
    {
      // Work out a frame delta time.
      const u32 now = device->getTimer()->getTime();
      const f32 frameDeltaTime = (f32)(now - then) / 1000.f; // Time in seconds
      then = now;
      
      core::vector3df nodePosition = node2->getPosition();

      if(receiver.IsKeyDown(irr::KEY_KEY_Z))
	nodePosition.Z += MOVEMENT_SPEED * frameDeltaTime;
      else if(receiver.IsKeyDown(irr::KEY_KEY_S))
	nodePosition.Z -= MOVEMENT_SPEED * frameDeltaTime;
      
      if(receiver.IsKeyDown(irr::KEY_KEY_D))
	nodePosition.X -= MOVEMENT_SPEED * frameDeltaTime;
      else if(receiver.IsKeyDown(irr::KEY_KEY_Q))
	nodePosition.X += MOVEMENT_SPEED * frameDeltaTime;
      
      node2->setPosition(nodePosition);
      
      driver->beginScene(true, true, video::SColor(255,113,113,133));
      
      smgr->drawAll(); // draw the 3d scene
      device->getGUIEnvironment()->drawAll(); // draw the gui environment (the logo)
      
      driver->endScene();
      
      int fps = driver->getFPS();
      
      if (lastFPS != fps)
	{
	  core::stringw tmp(L"Movement Example - Irrlicht Engine [");
	  tmp += driver->getName();
	  tmp += L"] fps: ";
	  tmp += fps;

	  device->setWindowCaption(tmp.c_str());
	  lastFPS = fps;
	}
    }
  
  //In the end, delete the Irrlicht device.
  device->drop();
  
  return 0;
}
