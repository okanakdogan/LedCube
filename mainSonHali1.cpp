#include<iostream>
#include<vector>
#include"irrlicht.h"
#include"deneme1\MyEventReceiver.h"
#include"deneme1\Interface.h"
#include"deneme1\LedCube.h"
#include "driverChoice.h"

using namespace irr;
using namespace core;
using namespace scene;
using namespace video;
using namespace io;
using namespace gui;

using namespace std;

int main(){
	video::E_DRIVER_TYPE driverType=driverChoiceConsole();
	if (driverType==video::EDT_COUNT)
		return 1;

	IrrlichtDevice *device = createDevice(driverType, FULLSCREEN, 16,false, false, false, NULL);

	IVideoDriver* driver = device->getVideoDriver();
	ISceneManager* smgr = device->getSceneManager();
	smgr->addCameraSceneNode(0, vector3df(100,80,-300), vector3df(100,20,20));
	Interface * intface = new Interface(device,driver);

	vector<Object> objects;

	CubeObject cube(device->getSceneManager(),device->getVideoDriver(),core::vector3df(150,100,-50),16);
    cube.loadData();

	MyEventReceiver receiver(device,intface,objects,&cube);
	device->setEventReceiver(&receiver);

	while(device->run())
	{
		stringw str = L"LED CUBE PROJECT   ";
		str += smgr->getActiveCamera()->getAbsolutePosition().X;
		str += "__";
		str += smgr->getActiveCamera()->getAbsolutePosition().Y;
		str += "__";
		str += smgr->getActiveCamera()->getAbsolutePosition().Z;
		str += "__&&__";
		str += smgr->getActiveCamera()->getTarget().X;
		str += "__";
		str += smgr->getActiveCamera()->getTarget().Y;
		str += "__";
		str += smgr->getActiveCamera()->getTarget().Z;
		device->setWindowCaption(str.c_str());

		driver->beginScene(true, true, SCOL_GRAY);

		smgr->drawAll();
		intface->guienv->drawAll();

		driver->endScene();
	}
	device->drop();

	return 0;
}
