#include<iostream>
#include<vector>
#include<irrlicht.h>
#include"MyEventReceiver.h"
#include"Interface.h"

using namespace irr;
using namespace core;
using namespace scene;
using namespace video;
using namespace io;
using namespace gui;

using namespace std;

int main(){
	IrrlichtDevice *device = createDevice(EDT_SOFTWARE, FULLSCREEN, 16,false, false, false, NULL);

	IVideoDriver* driver = device->getVideoDriver();
	ISceneManager* smgr = device->getSceneManager();
	smgr->addCameraSceneNode();
	Interface * intface = new Interface(device,driver);

	vector<Object> objects;

	MyEventReceiver receiver(device,intface,objects);
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

		driver->beginScene(true, true, SCOL_WHITE);

		smgr->drawAll();
		intface->guienv->drawAll();

		driver->endScene();
	}
	device->drop();

	return 0;
}
