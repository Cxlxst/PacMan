#ifdef _MSC_VER
#define _CRT_SECURE_NO_WARNINGS
#pragma comment(lib, "Irrlicht.lib")
#endif
 
#include <irrlicht.h>
#include "driverChoice.h"
 
using namespace irr;

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
 
        // Ceci est utilisé pour vérifier si une touche est enfoncée
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
        // Nous utilisons ce tableau pour stocker l'état de chaque touche.
        bool KeyIsDown[KEY_KEY_CODES_COUNT];
    };

int main()
    {
        // demande un pilote à l'utilisateur
        video::E_DRIVER_TYPE driverType=driverChoiceConsole();
        if (driverType==video::EDT_COUNT)
        return 1;
 
        // créer le moteur
        MyEventReceiver receiver;
 
        IrrlichtDevice* device = createDevice(driverType,
        core::dimension2d<u32>(640, 480), 16, false, false, false, &receiver);
 
        if (device == 0)
            return 1; // on ne peut pas créer le pilote sélectionné.
 
            video::IVideoDriver* driver = device->getVideoDriver();
            scene::ISceneManager* smgr = device->getSceneManager();
            scene::ISceneNode * node = smgr->addSphereSceneNode();
        if (node)
            {
                node->setPosition(core::vector3df(0,0,30));
                node->setMaterialTexture(0, driver->getTexture("../../Media/wall.bmp"));
                node->setMaterialFlag(video::EMF_LIGHTING, false);
            }
        scene::ISceneNode* n = smgr->addCubeSceneNode();
 
        if (n)
            {
                n->setMaterialTexture(0, driver->getTexture("../../Media/t351sml.jpg"));
                n->setMaterialFlag(video::EMF_LIGHTING, false);
                scene::ISceneNodeAnimator* anim =
                smgr->createFlyCircleAnimator(core::vector3df(0,0,30), 20.0f);
                
                if (anim)
                    {
                        n->addAnimator(anim);
                        anim->drop();
                    }
            }
        scene::IAnimatedMeshSceneNode* anms =
        smgr->addAnimatedMeshSceneNode(smgr->getMesh("../../Media/ninja.b3d"));
 
        if (anms)
            {
                scene::ISceneNodeAnimator* anim =
                smgr->createFlyStraightAnimator(core::vector3df(100,0,60),
                core::vector3df(-100,0,60), 3500, true);
                if (anim)
                    {
                        anms->addAnimator(anim);
                        anim->drop();
                    }
                anms->setMaterialFlag(video::EMF_LIGHTING, false);
 
                anms->setFrameLoop(0, 13);
                anms->setAnimationSpeed(15);
                anms->setScale(core::vector3df(2.f,2.f,2.f));
                anms->setRotation(core::vector3df(0,-90,0));
            }

        smgr->addCameraSceneNodeFPS();
        device->getCursorControl()->setVisible(false);
        device->getGUIEnvironment()->addImage(
        driver->getTexture("../../Media/irrlichtlogoalpha2.tga"),
        core::position2d<s32>(10,20));
 
        gui::IGUIStaticText* diagnostics = device->getGUIEnvironment()->addStaticText(L"", core::rect<s32>(10, 10, 400, 20));
        diagnostics->setOverrideColor(video::SColor(255, 255, 255, 0));
        int lastFPS = -1;
        u32 then = device->getTimer()->getTime();

        const f32 MOVEMENT_SPEED = 5.f;
 
        while(device->run())
            {
                // Calcul le temps delta d'une image.
                const u32 now = device->getTimer()->getTime();
                const f32 frameDeltaTime = (f32)(now - then) / 1000.f; // Temps en secondes
                then = now;
                core::vector3df nodePosition = node->getPosition();
 
                if(receiver.IsKeyDown(irr::KEY_KEY_W))
                nodePosition.Y += MOVEMENT_SPEED * frameDeltaTime;
                else if(receiver.IsKeyDown(irr::KEY_KEY_S))
                nodePosition.Y -= MOVEMENT_SPEED * frameDeltaTime;
 
                if(receiver.IsKeyDown(irr::KEY_KEY_A))
                nodePosition.X -= MOVEMENT_SPEED * frameDeltaTime;
                else if(receiver.IsKeyDown(irr::KEY_KEY_D))
                nodePosition.X += MOVEMENT_SPEED * frameDeltaTime;
 
                node->setPosition(nodePosition);
 
                driver->beginScene(true, true, video::SColor(255,113,113,133));
 
                smgr->drawAll(); // dessine la scene 3d
                device->getGUIEnvironment()->drawAll(); // dessine l'environnement gui (le logo)
 
                driver->endScene();
 
                int fps = driver->getFPS();
 
                if (lastFPS != fps)
                    {
                        core::stringw tmp(L"mouv pacman");
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