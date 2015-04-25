#ifndef MY_EVENT_RECEIVER_H
#define MY_EVENT_RECEIVER_H

#include"irrlicht.h"
#include"Interface.h"
#include"Objects.h"
#include"LedCube.h"
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
	MyEventReceiver(IrrlichtDevice *deviceV, Interface *intfaceV, vector<Object> objects, CubeObject *cubeV);
	virtual bool OnEvent(const SEvent& event);
	void rotateButtonHandler();
	void moveButtonHandler();
	void scaleButtonHandler();
	void changeCameraTypeButtonHandler();
	void changeModeButtonHandler();
	void addObjectButtonHandler();
	void exitButtonHandler();

	void coneImageHandler();
	void cubeImageHandler();
	void cylinderImageHandler();
	void pyramidImageHandler();
	void rectangleImageHandler();
	void squareImageHandler();

	void imageScrollBarHandler(const SEvent& event);
	void itemsScrollBarHandler(const SEvent& event);
	
	void selectedFileHandler(const SEvent& event);

	void removeObjectHandler(int index);

	void objectSelectHandler();
	void objectSelectHandlerHelper(Object obj, line3df raytrace,ISceneCollisionManager *colmgr);

	void drawModeHandler();
private:
	IrrlichtDevice *device;		// to access all device informations
	Interface *intface;			// to access all interface informations
	CubeObject *cube;			// to access all cube informations
	vector<Object> objects;		// to keep and access all objects informations
	s32 ImageScrollBarOldPos;	// to find change position in scrollbar
	s32 ItemsScrollBarOldPos;	// to find change position in scrollbar
	bool fpsCameraType;			// to change camera type
	bool selectMode;			// to change select mode
	bool cubeVisible;			// to change cube visible
	bool drawMode;				// to change draw mode
};

#endif
