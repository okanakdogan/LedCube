#ifndef RECEIVER_H
#define RECEIVER_H


#ifdef _MSC_VER
// We'll define this to stop MSVC complaining about sprintf().
#define _CRT_SECURE_NO_WARNINGS
#pragma comment(lib, "Irrlicht.lib")
#endif

#include <irrlicht.h>
#include "driverChoice.h"
#include <iostream>
#include <vector>

using namespace irr;
using namespace core;
using namespace scene;
using namespace video;
using namespace gui;
using namespace std ;





//GLOBALS 
float punto = 0.1f ;
core::vector3df mousePosition;
vector<ISceneNode*> myNode;
vector<ISceneNode*> choosen;
ISceneNode * cubeObj;
scene::ISceneManager* smgr ;
video::IVideoDriver* driver;
operations currentOp ;


enum
{
    // I use this ISceneNode ID to indicate a scene node that is
    // not pickable by getSceneNodeAndCollisionPointFromRay()
    ID_IsNotPickable = 0,

    // I use this flag in ISceneNode IDs to indicate that the
    // scene node can be picked by ray selection.
    IDFlag_IsPickable = 1 << 0,

    // I use this flag in ISceneNode IDs to indicate that the
    // scene node can be highlighted.  In this example, the
    // homonids can be highlighted, but the level mesh can't.
    IDFlag_IsHighlightable = 1 << 1
};


class MyEventReceiver : public IEventReceiver
{
private:
	
    SEvent::SJoystickEvent JoystickState;
    // We use this array to store the current state of each key
	bool KeyIsDown[KEY_KEY_CODES_COUNT];
public:


	MyEventReceiver() 
	{ 
		for (u32 i=0; i<KEY_KEY_CODES_COUNT; ++i)
			KeyIsDown[i] = false;
	}
	MyEventReceiver(SAppContext & context_)   {
		Context = context_ ;
		for (u32 i=0; i<KEY_KEY_CODES_COUNT; ++i)
			KeyIsDown[i] = false;
	 }

    // We'll create a struct to record info on the mouse state

	// This is used to check whether a key is being held down
	virtual bool isKeyDown(EKEY_CODE keyCode) const
	{
		return KeyIsDown[keyCode];
	}
	

    struct SMouseState
    {
        core::position2di Position;
        bool LeftButtonDown;
        bool RightButtonDown;
        SMouseState() : LeftButtonDown(false) , RightButtonDown(false)  { }
    } MouseState;

    // This is the one method that we have to implement
    virtual bool OnEvent(const SEvent& event)
    {

    	// Remember whether each key is down or up
		if (event.EventType == irr::EET_KEY_INPUT_EVENT)
			KeyIsDown[event.KeyInput.Key] = event.KeyInput.PressedDown;

        // Remember the mouse state
        if (event.EventType == irr::EET_MOUSE_INPUT_EVENT)
        {
            switch(event.MouseInput.Event)
            {
            case EMIE_LMOUSE_PRESSED_DOWN:
                MouseState.LeftButtonDown = true;
                break;

            case EMIE_LMOUSE_LEFT_UP:
                MouseState.LeftButtonDown = false;
                break;

            case EMIE_RMOUSE_PRESSED_DOWN:
                MouseState.RightButtonDown = true;
                break;    
         	
         	case EMIE_RMOUSE_LEFT_UP:
                MouseState.RightButtonDown = false;
                break;
            case EMIE_MOUSE_MOVED:
                MouseState.Position.X = event.MouseInput.X;
                MouseState.Position.Y = event.MouseInput.Y;
                break;

            default:
                // We won't use the wheel
                break;
            }
        }

        if (event.EventType == irr::EET_JOYSTICK_INPUT_EVENT
            && event.JoystickEvent.Joystick == 0)
        {
            JoystickState = event.JoystickEvent;
        }

        return false;
    }

    const SEvent::SJoystickEvent & GetJoystickState(void) const
    {
        return JoystickState;
    }
    const SMouseState & GetMouseState(void) const
    {
        return MouseState;
    }
};

#endif