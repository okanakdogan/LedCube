#include"MyEventReceiver.h"

MyEventReceiver::MyEventReceiver(IrrlichtDevice *deviceV, Interface *intfaceV, vector<Object> objectsV, CubeObject *cubeV) : device(deviceV){
	intface = new Interface(intfaceV);
	cube = cubeV;
	for(int i=0; i<objectsV.size(); i++){
		objects.push_back(objectsV[i]);
	}
	ImageScrollBarOldPos = intface->ImageScrollbar->getPos();
	ItemsScrollBarOldPos = intface->ItemsScrollbar->getPos();
	fpsCameraType = false;
	selectMode = false;
	cubeVisible = false;
	drawMode = false;
}

bool MyEventReceiver::OnEvent(const SEvent& event){
	// if the event type mouse event
	if(event.EventType == EET_MOUSE_INPUT_EVENT){
		if(event.MouseInput.isLeftPressed() && selectMode)
			objectSelectHandler();
		if(event.MouseInput.isLeftPressed() && drawMode && event.MouseInput.Control)
			drawModeHandler();
	}else
	// if the event type key input
	if(event.EventType == EET_KEY_INPUT_EVENT){
		// if user press c key then change camera type
		if(event.KeyInput.Key == KEY_KEY_C && event.KeyInput.PressedDown){
			changeCameraTypeButtonHandler();
		}else
		// if user press d key then delete selected item
		if(event.KeyInput.Key == KEY_KEY_D && event.KeyInput.PressedDown){
			if(!objects.empty()){
				for(int i=0; i<objects.size(); ++i){
					if(objects[i].isSelected){
						removeObjectHandler(i);
					}
				}
			}
		}else
		// if user press s key then change select mode
		if(event.KeyInput.Key == KEY_KEY_S && event.KeyInput.PressedDown){
			if(selectMode) selectMode = false;
			else selectMode = true;
		}else
		if(event.KeyInput.Key == KEY_KEY_L && event.KeyInput.Control && event.KeyInput.PressedDown){
			if(cubeVisible)	cubeVisible = false;
			else cubeVisible = true;
			cube->setVisible(cubeVisible);
		}else
		if(event.KeyInput.Key == KEY_KEY_P && event.KeyInput.Control && event.KeyInput.PressedDown){
			if(drawMode) drawMode = false;
			else drawMode = true;
		}
	}else
	// if the event type gui event
	if (event.EventType == EET_GUI_EVENT){
		// get caller id
		s32 id = event.GUIEvent.Caller->getID();
		// get gui environment
		IGUIEnvironment* env = device->getGUIEnvironment();
		// control event type
		switch(event.GUIEvent.EventType){
			// if gui event type is scroll bar change
			case EGET_SCROLL_BAR_CHANGED:
				// we have two scroll bar one is imagescrollbar the other one is itemsscrollbar
				if(id == IMAGE_SCROLL_BAR_ID)
					imageScrollBarHandler(event);
				else
				if(id == ITEMS_SCROLL_BAR_ID)
					itemsScrollBarHandler(event);
				break;
			// if gui event type is button click
			case EGET_BUTTON_CLICKED:
				switch(id){
					// if button id is rotate button
					case ROTATE_BUTTON_ID:
						rotateButtonHandler();
						return true;
					// if button id is move button
					case MOVE_BUTTON_ID:
						moveButtonHandler();
						return true;
					// if button id is scale button
					case SCALE_BUTTON_ID:
						scaleButtonHandler();
						return true;
					// if button id is change camera type button
					case CHANGE_CAMERA_TYPE_BUTTON_ID:
						changeCameraTypeButtonHandler();
						return true;
					// if button id is change mode button
					case CHANGE_MODE_BUTTON_ID:
						changeModeButtonHandler();
						return true;
					// if button id is add object button
					case ADD_OBJECT_BUTTON_ID:
						addObjectButtonHandler();
						return true;
					// if button id is exit button
					case EXIT_BUTTON_ID:
						exitButtonHandler();
						return true;
					default:
						return false;
				}
				break;
			// if gui event type is file selected in file browser
			case EGET_FILE_SELECTED:
				selectedFileHandler(event);
				break;
			// if gui event type is focused any element
			case EGET_ELEMENT_FOCUSED:
				// look up id is which element id
				switch(id){
					// if focused on cone image
					case CONE_IMAGE_ID:
						coneImageHandler();
						return true;
					// if focused on cube image
					case CUBE_IMAGE_ID:
						cubeImageHandler();
						return true;
					// if focused on cylinder image
					case CYLINDER_IMAGE_ID:
						cylinderImageHandler();
						return true;
					// if focused on pyramid image
					case PYRAMID_IMAGE_ID:
						pyramidImageHandler();
						return true;
					// if focused on rectangle image
					case RECTANGLE_IMAGE_ID:
						rectangleImageHandler();
						return true;
					// if focused on square image
					case SQUARE_IMAGE_ID:
						squareImageHandler();
						return true;
					default:
						return false;
				}
				break;
			default:
				return false;
		}
	}
	return false;
}

void MyEventReceiver::rotateButtonHandler(){
	// rotate button handler
}

void MyEventReceiver::moveButtonHandler(){
	// move button handler
}

void MyEventReceiver::scaleButtonHandler(){
	// scale button handler
}

// check the fpsCameraType variable and change camera with it
void MyEventReceiver::changeCameraTypeButtonHandler(){
	vector3df oldPosition = device->getSceneManager()->getActiveCamera()->getPosition();
	vector3df oldTarget = device->getSceneManager()->getActiveCamera()->getTarget();
	if(fpsCameraType){
		fpsCameraType = false;
		ICameraSceneNode *camera = device->getSceneManager()->addCameraSceneNode();
		camera->setPosition(oldPosition);
		camera->setTarget(oldTarget);
		device->getSceneManager()->setActiveCamera(camera);
	}else{
		fpsCameraType = true;
		device->getSceneManager()->setActiveCamera(device->getSceneManager()->addCameraSceneNodeFPS());
		device->getSceneManager()->getActiveCamera()->setPosition(oldPosition);
		device->getSceneManager()->getActiveCamera()->setTarget(oldTarget);
	}
}

void MyEventReceiver::changeModeButtonHandler(){
	// change mode button handler
}

// get a file browser from gui environment
void MyEventReceiver::addObjectButtonHandler(){
	device->getGUIEnvironment()->addFileOpenDialog(L"Please choose a file.", true, 0, -1, true);
}

// close the device
void MyEventReceiver::exitButtonHandler(){
	device->closeDevice();
}

// add a cone object from media/primitives
void MyEventReceiver::coneImageHandler(){
	Object *obj = new Object(device);
	bool isAdded = obj->addAnObject(L"../media/primitives/cone/cone.obj");
	if(isAdded){
		objects.push_back(*obj);
	}else{

	}
}

// add a cube object from media/primitives
void MyEventReceiver::cubeImageHandler(){
	Object *obj = new Object(device);
	bool isAdded = obj->addAnObject(L"../media/primitives/cube/cube.obj");
	if(isAdded){
		objects.push_back(*obj);
	}else{

	}
}

// add a cylinder object from media/primitives
void MyEventReceiver::cylinderImageHandler(){
	Object *obj = new Object(device);
	bool isAdded = obj->addAnObject(L"../media/primitives/cylinder/cylinder.obj");
	if(isAdded){
		objects.push_back(*obj);
	}else{

	}
}

// add a pyramid object from media/primitives
void MyEventReceiver::pyramidImageHandler(){
	Object *obj = new Object(device);
	bool isAdded = obj->addAnObject(L"../media/primitives/pyramid/pyramid.obj");
	if(isAdded){
		objects.push_back(*obj);
	}else{

	}
}

// add a rectangle object from media/primitives
void MyEventReceiver::rectangleImageHandler(){
	Object *obj = new Object(device);
	bool isAdded = obj->addAnObject(L"../media/primitives/rectangle/rectangle.obj");
	if(isAdded){
		objects.push_back(*obj);
	}else{

	}
}

// add a square object from media/primitives
void MyEventReceiver::squareImageHandler(){
	Object *obj = new Object(device);
	bool isAdded = obj->addAnObject(L"../media/primitives/square/square.obj");
	if(isAdded){
		objects.push_back(*obj);
	}else{

	}
}

// find change position from new position - old position
// and change all position of all element in right tab
void MyEventReceiver::imageScrollBarHandler(const SEvent& event){
	s32 newPos = intface->ImageScrollbar->getPos();
	s32 changePos = newPos - ImageScrollBarOldPos;
	ImageScrollBarOldPos = newPos;
	vector2d<int> changeVector = vector2d<s32>(0,-changePos);
	u32 childrenSize = intface->rightTab->getChildren().getSize();
	list<IGUIElement*> Images = intface->rightTab->getChildren();
	list<IGUIElement*>::Iterator imageIterator;
	for(imageIterator = Images.begin(); imageIterator != Images.end(); ++imageIterator){
		if((*imageIterator)->getID() != IMAGE_SCROLL_BAR_ID)
			(*imageIterator)->move(changeVector);
	}
}

// find change position from new position - old position
// and change all position of all element in items tab
void MyEventReceiver::itemsScrollBarHandler(const SEvent& event){
	s32 newPos = intface->ItemsScrollbar->getPos();
	s32 changePos = newPos - ItemsScrollBarOldPos;
	ItemsScrollBarOldPos = newPos;
	vector2d<int> changeVector = vector2d<s32>(0,-changePos);
	u32 childrenSize = intface->itemsTab->getChildren().getSize();
	list<IGUIElement*> Items = intface->itemsTab->getChildren();
	list<IGUIElement*>::Iterator itemsIterator;
	for(itemsIterator = Items.begin(); itemsIterator != Items.end(); ++itemsIterator){
		if((*itemsIterator)->getID() != ITEMS_SCROLL_BAR_ID)
			(*itemsIterator)->move(changeVector);
	}
}

// get file path from file browser and add a new object
void MyEventReceiver::selectedFileHandler(const SEvent& event){
	IGUIFileOpenDialog* dialog = (IGUIFileOpenDialog*)event.GUIEvent.Caller;
	Object *obj = new Object(device);
	bool isAdded = obj->addAnObject(dialog->getFileName());
	if(isAdded){
		objects.push_back(*obj);
	}else{

	}
}

// call object removeObject and erase from vector
void MyEventReceiver::removeObjectHandler(int index){
	if(index >= objects.size() || index < 0)
		return;
	objects[index].removeObject();
	objects.erase(objects.begin()+index);
}

//
void MyEventReceiver::objectSelectHandler(){
	ISceneManager* smgr = device->getSceneManager();
	ISceneCollisionManager *colmgr = smgr->getSceneCollisionManager();
	line3df raytrace = colmgr->getRayFromScreenCoordinates(device->getCursorControl()->getPosition(),smgr->getActiveCamera());
	for(int i=0; i<objects.size(); i++){
		objectSelectHandlerHelper(objects[i],raytrace,colmgr);
	}
}

//
void MyEventReceiver::objectSelectHandlerHelper(Object obj, line3df raytrace,ISceneCollisionManager *colmgr){
	vector3df outCollisionPoint = vector3df(10000,10000,10000);
	triangle3df outTriangle = triangle3df();
	ISceneNode *outNode;
	bool isGet = colmgr->getCollisionPoint(raytrace, obj.triangleSelector, outCollisionPoint, outTriangle, outNode);
	if(isGet && obj.node->getBoundingBox().isPointInside(outCollisionPoint)){
		if(obj.isSelected) obj.isSelected = false;
		else obj.isSelected = true;
		obj.removeObject();
	}
}

// draw mode handler
void MyEventReceiver::drawModeHandler(){
	
}
