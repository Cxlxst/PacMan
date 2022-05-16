#ifdef _MSC_VER
#define _CRT_SECURE_NO_WARNINGS
#pragma comment(lib, "Irrlicht.lib")
#pragma comment(lib, "irrKlang.lib")
#endif

//irrKlang
#if defined(WIN32)
#include <conio.h>
#else
#include "../Lib/irrKlang-64bit-1.6.0/examples/common/conio.h"
#endif

#include <irrlicht.h>
#include <irrKlang.h>
#include <stdio.h>
#include "iostream"
#include "../Includes/MyEventReceiver.hpp"

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
  IGUIListBox    *listbox;
};

int main(int argc, const char **argv)
{
  //Création du moteur son
  ISoundEngine *engine = createIrrKlangDevice();

  MyEventReceiver receiver;
  
  // Création de la fenêtre
  IrrlichtDevice *device =
    createDevice( EDT_SOFTWARE, dimension2d<u32>(1920, 1080), 16,
                  false, false, false, &receiver);
  device->setWindowCaption(L"PacMan - Bêta");
  
  if (!device)
    return 1;

  IVideoDriver *driver = device->getVideoDriver();
  ISceneManager *smgr = device->getSceneManager();
  IGUIEnvironment *guienv = device->getGUIEnvironment();
  
  //Gameplay
  IAnimatedMesh *mesh = smgr-> getMesh("../Media/Map/map.obj");
  IAnimatedMesh *mesh2 = smgr->getMesh("../Media/Personnages/pacmod.obj");
  IAnimatedMesh *mesh3 = smgr->getMesh("../Media/Items/dot.obj");
  ISceneNode *dot = smgr->addSphereSceneNode();

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
      map->setMaterialTexture( 0, driver->getTexture("../Media/Map/texture_map.jpg"));
    }

  IAnimatedMeshSceneNode *pacman = smgr->addAnimatedMeshSceneNode( mesh2 );
  if (pacman)
    {
      smgr->addCameraSceneNode(0, vector3df(0,30,10), vector3df(0,0,0));
      pacman->setRotation(vector3df(0,-90,0));
      pacman->setPosition(vector3df(0,3,1));
      pacman->setScale(vector3df (0.3,0.3,0.3));
      pacman->setMaterialFlag(EMF_LIGHTING,false);
      pacman->setMaterialTexture( 0, driver->getTexture("../Media/Personnages/textpac.png"));

      pacman->setRotation(vector3df(0,-180,0));
    }

  if (dot)
    {
      dot->setPosition(vector3df(2,3,1));
      dot->setScale(vector3df(0.1f,0.1f,0.1f));
      dot->setMaterialFlag(EMF_LIGHTING,false);
      dot->setMaterialTexture( 0, driver->getTexture("../Media/Items/texture_dot.png"));
  }

  int lastFPS = -1;
  u32 then = device->getTimer()->getTime();
  const f32 MOVEMENT_SPEED = 5.f;

  int i = 0;
  int dot_size = 0;

  int check_menu = 1;
  int Menu = 1;
  int Regles = 0;
  int Jouer = 0;
  int Exit = 0;

  while(device->run())
    {
      driver->beginScene(true, true, SColor(255,100,101,140));

      if (Menu == 1)
      	{
	  //engine->play2D("../../Media/Son/music_menu.ogg");

	  //MENU_JOUER
	  if(check_menu >= 1 && check_menu <= 8)
	    {
	      guienv->clear();
	      device->getGUIEnvironment()->addImage(driver->getTexture("../Media/Menu/Menu_1.jpg"), position2d<s32>(0,0));

	      if(receiver.IsKeyDown(KEY_RETURN))
		{
		  guienv->clear();
		  Menu = 0;
		  Regles = 0;
		  Jouer = 1;
		}
	    }
	  
	  //MENU_REGLES
	  if(check_menu >= 9 && check_menu <=16)
	    {
	      guienv->clear();
	      device->getGUIEnvironment()->addImage(driver->getTexture("../Media/Menu/Menu_2.jpg"), position2d<s32>(0,0));
	      
	      if(receiver.IsKeyDown(KEY_RETURN))
		{
		  guienv->clear();
		  Menu = 0;
		  Regles = 1;
		}
	    }

	  //MENU_EXIT
	  if(check_menu >= 17 && check_menu <= 24)
	    {
	      guienv->clear();
	      device->getGUIEnvironment()->addImage(driver->getTexture("../Media/Menu/Menu_3.jpg"), position2d<s32>(0,0));
	      
	      if(receiver.IsKeyDown(KEY_RETURN))
		{
		  device->drop();     
		}
	    }
	}

      if (Regles == 1)
	{
	  guienv->clear();
	  device->getGUIEnvironment()->addImage(driver->getTexture("../Media/Map/texture_map.jpg"), position2d<s32>(0,0));
	}

      if(Regles == 1 && receiver.IsKeyDown(KEY_ESCAPE))
	{
	  guienv->clear();
	  Regles = 0;
	  Menu = 1;
	}

      if (Jouer == 1)
	{
	  guienv->clear();
	  
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
	  float zdot = dot->getPosition().Z;
	  float xdot = dot->getPosition().X;


	  if (pacman->getPosition().X >= dot->getPosition().X - 0.6 && pacman->getPosition().X <= dot->getPosition().X + 0.6 && pacman->getPosition().Z >= dot->getPosition().Z - 0.6
	      && pacman->getPosition().Z <= dot->getPosition().Z + 0.6)
	    {
	      dot->setPosition(vector3df(0,0,0));
	      engine->play2D("../Media/Son/manger.ogg");
	      //std::cout << "pac z : " <<zpac<< " pacx : " << xpac << std::endl;                     
	      //std::cout << "dot z : " <<zdot<< " dotx : " << xdot << std::endl;                       
	    }

	  pacman->setPosition(nodePosition);

	}

      if(Jouer == 1 && receiver.IsKeyDown(KEY_ESCAPE))
	{
	  guienv->clear();
	  Jouer = 0;
	  Menu = 1;
	}
	
	  
      if(receiver.IsKeyDown(KEY_DOWN))
	{
	  check_menu = check_menu + 1;
	  //std::cout << "check_menu = " << check_menu << std::endl;
	}
      
      if(receiver.IsKeyDown(KEY_UP))
	{
	  check_menu = check_menu - 1;
	  //std::cout << "check_menu = " << check_menu << std::endl;
	}
      
      if (check_menu == 25)
	check_menu = 1;
      if (check_menu == 0)
	check_menu = 24;
      
      smgr->drawAll();
      guienv->drawAll();
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
  
  device->drop();
  return 0;
}
