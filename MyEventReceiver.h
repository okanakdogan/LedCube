#ifndef MY_EVENT_RECEIVER_H
#define MY_EVENT_RECEIVER_H

#include"irrlicht.h"
#include"Interface.h"
#include"Objects.h"
#include"LedCube.h"
#include"DrawingObject.h"
#include<vector>
#include<iostream>

using namespace irr;
using namespace core;
using namespace scene;
using namespace video;
using namespace io;
using namespace gui;
using namespace std;

enum Operation_t 
{
	draw_id = 32,
	choose_id,
	move_id,
	rotate_id,
	scale_id,
	noop_id
};

		// to keep and access all objects informations

class MyEventReceiver : public IEventReceiver{
public:
	
	MyEventReceiver(IrrlichtDevice *deviceV, Interface *intfaceV, vector<Object> objects, CubeObject *cubeV);
	virtual bool OnEvent(const SEvent& event);
	void rotateButtonHandler();
	void moveButtonHandler();
	void scaleButtonHandler();
	void drawButtonHandler();
	void chooseButtonHandler();
	void changeCameraTypeButtonHandler();
	void changeModeButtonHandler();
	void addObjectButtonHandler();
	void mouseLeftButtonEventHandler();
	void mouseWheelEventHandler(int wheelValue);
	void exitButtonHandler();
	void clearDrawingEvent();

	void helpButtonHandler();

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

	void drawModeHandler();
	void makeGroupHandler();

	void mouseMoveHandler();

	vector<Object> objects;

	
private:
	vector3df setLineLength(line3df newLine ,float newLength);
	vector3df commonPointFounder();

	IrrlichtDevice *device;		// to access all device informations
	Interface *intface;			// to access all interface informations
	CubeObject *cube;			// to access all cube informations
	vector<Object> drawNodes;
	s32 ImageScrollBarOldPos;	// to find change position in scrollbar
	s32 ItemsScrollBarOldPos;	// to find change position in scrollbar
	bool fpsCameraType;			// to change camera type
	bool selectMode;			// to change select mode
	bool cubeVisible;			// to change cube visible
	bool drawMode;				// to change draw mode
	bool otomaticFocus;			// to change otomatic focus mode
	int currentOp;
	vector3df mousePosition;
	vector3df tempPosition;
	int drawCounter;
	float distanceToNode;
	vector<DrawingObject> drawingObj ;
	vector<Node> currentDraw;
};

#endif