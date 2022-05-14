#ifdef _MSC_VER
#define _CRT_SECURE_NO_WARNINGS
#pragma comment(lib, "Irrlicht.lib")
#pragma comment(lib, "irrKlang.lib")
#endif

//irrKlang
#if defined(WIN32)
#include <conio.h>
#else
#include "../../Lib/irrKlang-64bit-1.6.0/examples/common/conio.h"
#endif

#include <irrlicht.h>
#include <irrKlang.h>
#include <stdio.h>
#include "iostream"
#include "../../Includes/MyEventReceiver.hpp"

using namespace irr;
using namespace core;
using namespace scene;
using namespace io;
using namespace gui;
using namespace video;
using namespace irrklang;


//Stock le contexte du MyEventReceiver
struct SAppContext
{
  IrrlichtDevice *device;
  s32             counter;
  IGUIListBox*    listbox;
};


int main(int argc, const char **argv)
{
  //Création du moteur de son
  ISoundEngine *engine = createIrrKlangDevice();

  //Création moteur de rendu (Menu)
  int check_menu = 1;
  
  MyEventReceiver receiver;
  
  //Création de la fenêtre
  IrrlichtDevice *device = createDevice(EDT_OPENGL, dimension2d<u32>(1920, 1280), 32, false, false, false, &receiver);

  if (device == 0)
    return 1;

  IVideoDriver *driver = device->getVideoDriver();
  ISceneManager *smgr = device->getSceneManager();

  IAnimatedMesh *mesh = smgr-> getMesh("../../Media/Map/map.obj");
  IAnimatedMesh *mesh2 = smgr->getMesh("../../Media/Personnages/pacmod.obj");
  IAnimatedMesh *mesh3 = smgr->getMesh("../../Media/Items/dot.obj");
  ISceneNode *doot = smgr->addSphereSceneNode();

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

   if (!mesh3)
    {
      device->drop();
      return 1;
    }
  
  IAnimatedMeshSceneNode *map = smgr->addAnimatedMeshSceneNode( mesh );
  if (map)
    {
      map->setMaterialFlag(EMF_LIGHTING,false);
      map->setMaterialTexture( 0, driver->getTexture("../../Media/Map/texture_map.jpg"));
    }
  
  IAnimatedMeshSceneNode *pacman = smgr->addAnimatedMeshSceneNode( mesh2 );
  if (pacman)
    {
      smgr->addCameraSceneNode(0, vector3df(0,30,10), vector3df(0,0,0));
      pacman->setRotation(vector3df(0,-90,0));
      pacman->setPosition(vector3df(0,3,1));
      pacman->setScale(vector3df (0.3,0.3,0.3));
      pacman->setMaterialFlag(EMF_LIGHTING,false);
      pacman->setMaterialTexture( 0, driver->getTexture("../../Media/Personnages/textpac.png"));

      pacman->setRotation(vector3df(0,-180,0));
    }

   if (doot){
    doot->setPosition(vector3df(2,3,1));
    doot->setScale(vector3df(0.1f,0.1f,0.1f));
    doot->setMaterialFlag(EMF_LIGHTING,false);
    doot->setMaterialTexture( 0, driver->getTexture("../../Media/Items/texture_dot.png"));
  }

  
  IGUIStaticText* diagnostics = device->getGUIEnvironment()->addStaticText(L"", rect<s32>(10, 10, 400, 20));
  diagnostics->setOverrideColor(SColor(255, 255, 255, 0));

  int lastFPS = -1;

  u32 then = device->getTimer()->getTime();

	
  const f32 MOVEMENT_SPEED = 5.f;

  
  engine->play2D("../../Media/Son/Pacman_Menu.ogg");

  int i = 0;
  int dot_size = 0;
  
  while(device->run())
      {	
	// Work out a frame delta time.
	const u32 now = device->getTimer()->getTime();
	const f32 frameDeltaTime = (f32)(now - then) / 1000.f; // Time in seconds
	then = now;
	
        vector3df nodePosition = pacman->getPosition();
	
	//Directions ZQSD
	if(receiver.IsKeyDown(KEY_KEY_S))
	  {
	    nodePosition.Z += MOVEMENT_SPEED * frameDeltaTime;
	    pacman->setRotation(vector3df(0,90,0));
	  }

	else if(receiver.IsKeyDown(KEY_KEY_Z))
	  {
	    nodePosition.Z -= MOVEMENT_SPEED * frameDeltaTime;
	    pacman->setRotation(vector3df(0,270,0));
	  }
	  
	if(receiver.IsKeyDown(KEY_KEY_D))
	  {
	    nodePosition.X -= MOVEMENT_SPEED * frameDeltaTime;
	    pacman->setRotation(vector3df(0,0,0));
	  }
	
	else if(receiver.IsKeyDown(KEY_KEY_Q))
	  {
	    nodePosition.X += MOVEMENT_SPEED * frameDeltaTime;
	    pacman->setRotation(vector3df(0,180,0));
	  }

	float zpac = pacman->getPosition().Z;
	float xpac = pacman->getPosition().X;
	float zdot = doot->getPosition().Z;
	float xdot = doot->getPosition().X;

	
	if (pacman->getPosition().X >= doot->getPosition().X - 0.6 && pacman->getPosition().X <= doot->getPosition().X + 0.6 && pacman->getPosition().Z >= doot->getPosition().Z - 0.6 && pacman->getPosition().Z <= doot->getPosition().Z + 0.6){
	  // std::cout << "pac z : " <<zpac<< " pacx : " << xpac << std::endl;
	  //std::cout << "dot z : " <<zdot<< " dotx : " << xdot << std::endl;
 

	  doot->setPosition(vector3df(0,0,0));
      }

	pacman->setPosition(nodePosition);
	driver->beginScene(true, true, SColor(255,113,113,133));
	
	smgr->drawAll(); // draw the 3d scene
	//guienv->drawAll();
	//device->getGUIEnvironment()->drawAll(); // draw the gui environment (the logo)
	driver->endScene();
	
	int fps = driver->getFPS();
	
	if (lastFPS != fps)
	  {
	    stringw tmp(L"Movement Example - Irrlicht Engine [");
	    tmp += driver->getName();
	    tmp += L"] fps: ";
	    tmp += fps;
	    
	    device->setWindowCaption(tmp.c_str());
	    lastFPS = fps;
	  }
      }
  
    // Supprime le moteur Irrlich
    device->drop();

    return 0;
}
