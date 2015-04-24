#ifndef MY_EVENT_RECEIVER_H
#define MY_EVENT_RECEIVER_H

#include"irrlicht.h"
#include"Interface.h"
#include"Objects.h"
#include<vector>

using namespace irr;
using namespace core;
using namespace scene;
using namespace video;
using namespace io;
using namespace gui;
using namespace std;

class MyEventReceiver : public IEventReceiver{
public:
	MyEventReceiver(IrrlichtDevice *deviceV, Interface *intfaceV, vector<Object> objects);
	virtual bool OnEvent(const SEvent& event);
	void rotateButtonHandler();
	void moveButtonHandler();
	void scaleButtonHandler();
	void changeCameraTypeButtonHandler();
	void changeModeButtonHandler();
	void addObjectButtonHandler();
	void exitButtonHandler();

	void imageScrollBarHandler(const SEvent& event);
	void itemsScrollBarHandler(const SEvent& event);
	
	void selectedFileHandler(const SEvent& event);

	void removeObjectHandler(int index);

	void objectSelectHandler();
	void objectSelectHandlerHelper(Object obj, line3df raytrace,ISceneCollisionManager *colmgr);
private:
	IrrlichtDevice *device;
	Interface *intface;
	vector<Object> objects;
	s32 ImageScrollBarOldPos;
	s32 ItemsScrollBarOldPos;
	bool fpsCameraType;
	bool selectMode;
};

#endif