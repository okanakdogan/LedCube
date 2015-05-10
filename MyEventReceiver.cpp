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
	otomaticFocus = false;
	currentOp =  noop_id ;
	distanceToNode = 250;
	tempPosition= vector3df(-1000,-1000,-1000);
}

bool MyEventReceiver::OnEvent(const SEvent& event){
	// if the event type mouse event
	if(event.EventType == EET_MOUSE_INPUT_EVENT){
		if(event.MouseInput.Event == EMIE_MOUSE_WHEEL)
			mouseWheelEventHandler(event.MouseInput.Wheel);
		if(event.MouseInput.isLeftPressed())
			mouseLeftButtonEventHandler();
		if(event.MouseInput.Event == EMIE_LMOUSE_LEFT_UP)
			tempPosition= vector3df(-1000,-1000,-1000);
		if(event.MouseInput.Event == EMIE_MOUSE_MOVED && drawMode){
			mouseMoveHandler();
		}
	}else
	// if the event type key input
	if(event.EventType == EET_KEY_INPUT_EVENT){
		// if user press c key then change camera type
		if(event.KeyInput.Key == KEY_KEY_C && event.KeyInput.PressedDown){
			changeCameraTypeButtonHandler();
		}else
		if(event.KeyInput.Key == KEY_KEY_O && event.KeyInput.PressedDown){
			if(otomaticFocus) otomaticFocus = false;
			else otomaticFocus = true;
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
			if(selectMode){
				selectMode = false;
				currentOp = noop_id;
			}
			else{
				selectMode = true;
				currentOp = choose_id;
			}
		}else
		if(event.KeyInput.Key == KEY_KEY_L && event.KeyInput.Control && event.KeyInput.PressedDown){
			if(cubeVisible)	cubeVisible = false;
			else cubeVisible = true;
			cube->setVisible(cubeVisible);
			for(int i=0; i<objects.size(); i++){
				objects[i].node->setVisible(cubeVisible);
			}
		}else
		if(event.KeyInput.Key == KEY_KEY_P && event.KeyInput.Control && event.KeyInput.PressedDown){
			if(drawMode){
				drawMode = false;
				currentOp = noop_id;
			}
			else{
				drawMode = true;
				currentOp = draw_id;
			}
		}else
		if(event.KeyInput.Key == KEY_KEY_G && event.KeyInput.PressedDown){
			makeGroupHandler();
			Object::groupIdCount++;
		}

		else if(event.KeyInput.Key == KEY_KEY_N && event.KeyInput.PressedDown){
			currentOp = noop_id;
			DrawingObject draw ;
			draw.node  =  currentDraw ;
			drawingObj.push_back(draw)  ;
			drawMode = false;
			selectMode = false;

		}else if(event.KeyInput.Key == KEY_KEY_Q && event.KeyInput.PressedDown)
			clearDrawingEvent();
		else if(event.KeyInput.Key == KEY_KEY_E && event.KeyInput.PressedDown)
			cube->checkSetCollisionObject(objects);
	}
	else
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
					case DRAW_BUTTON_ID:
						drawButtonHandler();
						return true;
					case CHOOSE_BUTTON_ID:
						chooseButtonHandler();
						return true;	
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
					case HELP_BUTTON_ID:
						helpButtonHandler();
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
	selectMode = false;
	drawMode = false;
	/*
	if(drawNodes.size() != 0){
		for(int i = 1 ; i < drawNodes.size(); i++)
			drawNodes.at(0)->addChild(drawNodes.at(i));
		Object obj (device);
		obj.node = (IAnimatedMeshSceneNode*) drawNodes.at(0);
		objects.push_back(obj);
		drawNodes.clear();
	}*/
	currentOp = rotate_id;
}

void MyEventReceiver::moveButtonHandler(){
	selectMode = false;
	drawMode = false;
	/*
	if(drawNodes.size() != 0){
		Object *obj = new Object(device);
		obj->node = static_cast<IAnimatedMeshSceneNode*>(drawNodes[0]);

		for(int i = 1 ; i < drawNodes.size(); i++){
			obj->node->addChild(drawNodes[i]);
		}
		
		objects.push_back(*obj);
		cout << obj->node->getChildren().getSize() << endl;
		obj->node->getMaterial(0).EmissiveColor.set(255, 255, 255, 0);
		drawNodes.clear();
	}*/
	currentOp = move_id;
}

void MyEventReceiver::clearDrawingEvent()
{
	for(int i = 0 ; i < objects.size(); i++)
		objects[i].node->remove();
	objects.clear();
}

void MyEventReceiver::scaleButtonHandler(){
	selectMode = false;
	drawMode = false;
	/*
	if(drawNodes.size() != 0){
		for(int i = 1 ; i < drawNodes.size(); i++)
			drawNodes.at(0)->addChild(drawNodes.at(i));
		Object obj (device);
		obj.node = (IAnimatedMeshSceneNode*) drawNodes.at(0);
		objects.push_back(obj);
		drawNodes.clear();
	}*/
	currentOp = scale_id;
}

void MyEventReceiver::chooseButtonHandler(){
	selectMode = true;
	drawMode = false;
	/*
	if(drawNodes.size() != 0){
		for(int i = 1 ; i < drawNodes.size(); i++)
			drawNodes.at(0)->addChild(drawNodes.at(i));
		Object obj (device);
		obj.node = (IAnimatedMeshSceneNode*) drawNodes.at(0);
		objects.push_back(obj);
		drawNodes.clear();
	}*/
	currentOp = choose_id;
}

void MyEventReceiver::drawButtonHandler(){
	selectMode = false;
	drawMode = true;
	currentOp = draw_id;
}

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
	/*if(drawMode) drawMode = false;
	else drawMode = true;*/
}

// get a file browser from gui environment
void MyEventReceiver::addObjectButtonHandler(){
	device->getGUIEnvironment()->addFileOpenDialog(L"Please choose a file.", true, 0, -1, true);
}

// close the device
void MyEventReceiver::exitButtonHandler(){
	device->closeDevice();
}

// help button handler
void MyEventReceiver::helpButtonHandler(){
	IGUIWindow* window = device->getGUIEnvironment()->addWindow(
		rect<s32>(100,100,1100,600),
						false, // modal?
						L"LED CUBE SIMULATION INFORMATIONS");
	device->getGUIEnvironment()->addStaticText(L"---> For Change Camera Type Click C Key or ChangeCameraType Button",
						rect<s32>(20,50,980,80),true,false,window);
	device->getGUIEnvironment()->addStaticText(L"--->For Change Select Mode Click S Key And Click With Mouse Left Button To Object",
						rect<s32>(20,80,980,110),true,false,window);
	device->getGUIEnvironment()->addStaticText(L"--->For Delete Selected Object Press D Key",
						rect<s32>(20,110,980,140),true,false,window);
	device->getGUIEnvironment()->addStaticText(L"--->For Change Cube Visible Value Press Cntrl Key With L Key(Cntrl-L)",
						rect<s32>(20,140,980,170),true,false,window);
	device->getGUIEnvironment()->addStaticText(L"--->To Add A New Primitive Object Just Press To Primitive Object Image",
						rect<s32>(20,170,980,200),true,false,window);
	device->getGUIEnvironment()->addStaticText(L"--->For Change Cube Visible Value Press Cntrl Key With L Key(Cntrl-L)",
						rect<s32>(20,200,980,230),true,false,window);
	device->getGUIEnvironment()->addStaticText(L"--->To Rotate,Move,Scale The Selected Objects Press Rotate,Move,Scale Buttons",
						rect<s32>(20,230,980,260),true,false,window);
	device->getGUIEnvironment()->addStaticText(L"--->To Draw In 3d Just Press ChangeMode Button Or Press (Cntrl-P) Keys",
						rect<s32>(20,260,980,290),true,false,window);
	device->getGUIEnvironment()->addStaticText(L"--->To Add A New Object Press AddObject Button And Select .obj File In File Browser",
						rect<s32>(20,290,980,320),true,false,window);
}

// add a cone object from media/primitives


void MyEventReceiver::coneImageHandler(){
	Object *obj = new Object(device);
	bool isAdded = obj->addAnObject(L"media/primitives/cone/cone.obj");
	if(isAdded){
		objects.push_back(*obj);
	}else{

	}
}

// add a cube object from media/primitives
void MyEventReceiver::cubeImageHandler(){
	Object *obj = new Object(device);
	bool isAdded = obj->addAnObject(L"media/primitives/cube/cube.obj");
	if(isAdded){
		objects.push_back(*obj);
	}else{

	}
}

// add a cylinder object from media/primitives
void MyEventReceiver::cylinderImageHandler(){
	Object *obj = new Object(device);
	bool isAdded = obj->addAnObject(L"media/primitives/cylinder/cylinder.obj");
	if(isAdded){
		objects.push_back(*obj);
	}else{

	}
}

// add a pyramid object from media/primitives
void MyEventReceiver::pyramidImageHandler(){
	Object *obj = new Object(device);
	bool isAdded = obj->addAnObject(L"media/primitives/pyramid/pyramid.obj");
	if(isAdded){
		objects.push_back(*obj);
	}else{

	}
}

// add a rectangle object from media/primitives
void MyEventReceiver::rectangleImageHandler(){
	Object *obj = new Object(device);
	bool isAdded = obj->addAnObject(L"media/primitives/rectangle/rectangle.obj");
	if(isAdded){
		objects.push_back(*obj);
	}else{

	}
}

// add a square object from media/primitives
void MyEventReceiver::squareImageHandler(){
	Object *obj = new Object(device);
	bool isAdded = obj->addAnObject(L"media/primitives/square/square.obj");
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

// object select handler
void MyEventReceiver::objectSelectHandler(){
	ISceneManager* smgr = device->getSceneManager();
	ISceneCollisionManager *colmgr = smgr->getSceneCollisionManager();
	line3df raytrace = colmgr->getRayFromScreenCoordinates(device->getCursorControl()->getPosition(),smgr->getActiveCamera());
	for(int i=0; i<objects.size(); i++){
		vector3df outCollisionPoint = vector3df(1000,1000,1000);
		triangle3df outTriangle = triangle3df();
		ISceneNode *outNode;
		bool isGet = colmgr->getCollisionPoint(raytrace, objects[i].triangleSelector, outCollisionPoint, outTriangle, outNode);
		if(isGet/* && objects[i].node->getTransformedBoundingBox().isPointInside(outCollisionPoint)/*.isPointInside(outCollisionPoint)*/){
			if(objects[i].isSelected){
				int groupId = objects[i].groupId;
				if(groupId == -1){
					objects[i].isSelected = false;
					objects[i].node->getMaterial(0).EmissiveColor.set(255, 0,   0,   0);
				}else{
					for(int j=0; j<objects.size(); j++){
						if(objects[j].groupId == groupId){
							objects[j].isSelected = false;
							objects[j].node->getMaterial(0).EmissiveColor.set(255, 0,   0,   0);
						}
					}
				}
			}
			else{
				int groupId = objects[i].groupId;
				if(groupId == -1){
					objects[i].isSelected = true;
					objects[i].node->getMaterial(0).EmissiveColor.set(255, 255, 255, 0);
				}else{
					for(int j=0; j<objects.size(); j++){
						if(objects[j].groupId == groupId){
							objects[j].isSelected = true;
							objects[j].node->getMaterial(0).EmissiveColor.set(255, 255, 255, 0);
						}
					}
				}
			}
		}	
	}
}

vector3df MyEventReceiver::setLineLength(line3df line ,float newLength){
	
	vector3df lineStart = line.start;
	vector3df lineEnd = line.end;
	float x = lineEnd.X-lineStart.X;
	float y = lineEnd.Y-lineStart.Y;
	float z = lineEnd.Z-lineStart.Z;

	float uzunluk  = core::squareroot( (lineEnd.X-lineStart.X)*(lineEnd.X-lineStart.X) + (lineEnd.Y-lineStart.Y)*(lineEnd.Y-lineStart.Y) +(lineEnd.Z-lineStart.Z)*(lineEnd.Z-lineStart.Z) );
	
	return vector3df(lineStart.X + (x*newLength)/uzunluk,lineStart.Y + (y*newLength)/uzunluk,lineStart.Z + (z*newLength)/uzunluk);
}

// draw mode handler
void MyEventReceiver::drawModeHandler(){
	if(fpsCameraType){
		ICameraSceneNode *camera = device->getSceneManager()->getActiveCamera();
	
		line3df ray(camera->getAbsolutePosition(),camera->getTarget());

		if(otomaticFocus)		
			mousePosition = setLineLength(ray,distanceToNode);
		else
			mousePosition = setLineLength(ray,250);
		if(tempPosition==vector3df(-1000,-1000,-1000))
			tempPosition=mousePosition;
		ISceneNode *cube1 = device->getSceneManager()->addCubeSceneNode(4.0f);
		cube1->setPosition(mousePosition);

		float distance= tempPosition.getDistanceFrom(mousePosition);

		int cubeCount=distance/2;
		
		float x_ = tempPosition.X-mousePosition.X;
		float y_ = tempPosition.Y-mousePosition.Y;
		float z_ = tempPosition.Z-mousePosition.Z;
		
		
		if(cubeCount != 0){
			float e = 1/(double)cubeCount ;
			
			for(int i = 0 ; i < cubeCount ; i++)
			{
				ISceneNode *cube = device->getSceneManager()->addCubeSceneNode(4.0f);
				cube->setPosition(vector3df(tempPosition.X+x_*e*i,tempPosition.Y+y_*e*i,tempPosition.Z+z_*e*i));
				Node obj(cube) ;
				currentDraw.push_back(obj);
			}
		
		}
	

	
		
		
		cout << objects.size() << endl;
	}else{
		ICameraSceneNode *camera = device->getSceneManager()->getActiveCamera();
		

		line3df ray = device->getSceneManager()->getSceneCollisionManager()->
			getRayFromScreenCoordinates(device->getCursorControl()->getPosition(),device->getSceneManager()->getActiveCamera());
if(otomaticFocus)		
			mousePosition = setLineLength(ray,distanceToNode);
		else
			mousePosition = setLineLength(ray,250);
		if(tempPosition==vector3df(-1000,-1000,-1000))
			tempPosition=mousePosition;
		ISceneNode *cube1 = device->getSceneManager()->addCubeSceneNode(4.0f);
		cube1->setPosition(mousePosition);

		float distance= tempPosition.getDistanceFrom(mousePosition);

		int cubeCount=distance/2;
		
		float x_ = tempPosition.X-mousePosition.X;
		float y_ = tempPosition.Y-mousePosition.Y;
		float z_ = tempPosition.Z-mousePosition.Z;
		
		
		if(cubeCount != 0){
			float e = 1/(double)cubeCount ;
			
			for(int i = 0 ; i < cubeCount ; i++)
			{
				ISceneNode *cube = device->getSceneManager()->addCubeSceneNode(4.0f);
				cube->setPosition(vector3df(tempPosition.X+x_*e*i,tempPosition.Y+y_*e*i,tempPosition.Z+z_*e*i));
				Node obj(cube) ;
				currentDraw.push_back(obj);
			}
		
		}
		
	}
}

void MyEventReceiver::mouseWheelEventHandler(int wheelValue)
{
	switch(currentOp)
	{
		case scale_id :
			for(int i = 0 ; i < objects.size(); i++){
				if(objects[i].isSelected){
					vector3df objScale = objects[i].node->getScale();
					if(objScale.X < 0.1 && objScale.Y < 0.1 && objScale.Z < 0.1){
						if(wheelValue != 1)
							break;
					}
					if(wheelValue == 1){
						objects[i].node->setScale(vector3df(objScale.X+0.025,objScale.Y+0.025,objScale.Z+0.025));
					}else{
						objects[i].node->setScale(vector3df(objScale.X-0.025,objScale.Y-0.025,objScale.Z-0.025));
					}
				}
			}
	}
}


vector3df MyEventReceiver::commonPointFounder()
{
	vector<Object> objs;
	for(int i=0; i<objects.size(); i++)
		if(objects.at(i).isSelected)
			objs.push_back(objects.at(i));

	int size = objs.size();
	float x = 0.0f,y=0.0f,z=0.0f;
	for(int i= 0 ; i < size  ; i++)
	{
		x += objs[i].node->getAbsolutePosition().X;
		y += objs[i].node->getAbsolutePosition().Y;
		z += objs[i].node->getAbsolutePosition().Z;
	}
	x /= size;
	y /= size;
	z /= size;
	vector3df commonVector = vector3df(x,y,z);
	return commonVector;
}


	


void MyEventReceiver::mouseLeftButtonEventHandler()
{
	ISceneManager* smgr = device->getSceneManager();
	ISceneCollisionManager *colmgr = smgr->getSceneCollisionManager();
	line3df raytrace = colmgr->getRayFromScreenCoordinates(device->getCursorControl()->getPosition(),smgr->getActiveCamera());
	mousePosition = vector3df(raytrace.getVector().X,raytrace.getVector().Y,0);
	switch(currentOp)
	{
		case choose_id :
			objectSelectHandler();
			break ;
		case move_id:
			if(fpsCameraType){
				line3df ray(device->getSceneManager()->getActiveCamera()->getAbsolutePosition(),
										device->getSceneManager()->getActiveCamera()->getTarget());
				vector3df newMousePos = setLineLength(ray,250);
				vector3df commonPoint = commonPointFounder();
				float x_ = newMousePos.X - commonPoint.X; 
				float y_ = newMousePos.Y - commonPoint.Y; 
				float z_ = newMousePos.Z - commonPoint.Z;
				for(int i = 0 ; i < objects.size(); i++){
					if(objects[i].isSelected){
						vector3df oldPosition = objects[i].node->getAbsolutePosition();
						vector3df newPosition = vector3df(oldPosition.X+x_,oldPosition.Y+y_,oldPosition.Z+z_);
						objects[i].node->setPosition(newPosition);
					}
				}
			}else{
				line3df ray = colmgr->getRayFromScreenCoordinates(device->getCursorControl()->getPosition(),smgr->getActiveCamera());
				vector3df newMousePos = setLineLength(ray,250);
				vector3df commonPoint = commonPointFounder();
				float x_ = newMousePos.X - commonPoint.X; 
				float y_ = newMousePos.Y - commonPoint.Y; 
				float z_ = newMousePos.Z - commonPoint.Z;
				for(int i = 0 ; i < objects.size(); i++){
					if(objects[i].isSelected){
						vector3df oldPosition = objects[i].node->getAbsolutePosition();
						vector3df newPosition = vector3df(oldPosition.X+x_,oldPosition.Y+y_,oldPosition.Z+z_);
						objects[i].node->setPosition(newPosition);
					}
				}
			}
			break ;
		case rotate_id:
			for(int i = 0 ; i < objects.size(); i++){
				if(objects[i].isSelected){
					vector3df objRotate = objects[i].node->getRotation();
					vector3df rot = vector3df(0,0,0); //get current rotation (euler)
					if(mousePosition.X < tempPosition.X && mousePosition.Y < tempPosition.Y )
						rot = vector3df(2,2,0);
					if(mousePosition.X >= tempPosition.X && mousePosition.Y >= tempPosition.Y)
						rot = vector3df(-2,-2,0);
					if(mousePosition.X >= tempPosition.X && mousePosition.Y < tempPosition.Y)
						rot = vector3df(-2,2,0);
					if(mousePosition.X < tempPosition.X && mousePosition.Y >= tempPosition.Y)
						rot = vector3df(2,-2,0);

					matrix4 m;
					matrix4 mp;
					m.setRotationDegrees(objRotate); //set matrix to current rotation
					mp.setRotationDegrees(rot); //set second matrix to rotation to add
					m *= mp; //multipy them

					objRotate = m.getRotationDegrees(); //get rotation vector from matrix (euler)

					objects[i].node->setRotation(objRotate); //rotate node 
					break ;
				}
			}
			tempPosition = mousePosition  ;
			break ;	
		case draw_id:
        	drawModeHandler();
			tempPosition = mousePosition  ;
			break;
		default :
			break ;

	}
}

// make group handler
void MyEventReceiver::makeGroupHandler(){
	for(int i=0; i<objects.size(); ++i){
		if(objects[i].isSelected){
			objects[i].groupId = Object::groupIdCount;
		}
	}
}

void MyEventReceiver::mouseMoveHandler(){
	vector3df outCollisionPoint = vector3df(1000,1000,1000);
	triangle3df outTriangle = triangle3df();
	ISceneNode *outNode;
	ISceneManager* smgr = device->getSceneManager();
	ISceneCollisionManager *colmgr = smgr->getSceneCollisionManager();
	line3df ray = colmgr->getRayFromScreenCoordinates(device->getCursorControl()->getPosition(),smgr->getActiveCamera());
	bool isSet = false;
	for(int i=0; i<objects.size(); i++){
		bool isGet = colmgr->getCollisionPoint(ray, objects[i].triangleSelector, outCollisionPoint, outTriangle, outNode);
		if(isGet){
			line3df line = line3df(smgr->getActiveCamera()->getAbsolutePosition(), outCollisionPoint);
			distanceToNode = line.getLength();
			isSet = true;
			break;
		}
	}
}