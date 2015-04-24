#include"MyEventReceiver.h"

MyEventReceiver::MyEventReceiver(IrrlichtDevice *deviceV, Interface *intfaceV, vector<Object> objectsV) : device(deviceV){
	intface = new Interface(intfaceV);
	for(int i=0; i<objectsV.size(); i++){
		objects.push_back(objectsV[i]);
	}
	ImageScrollBarOldPos = intface->ImageScrollbar->getPos();
	ItemsScrollBarOldPos = intface->ItemsScrollbar->getPos();
	fpsCameraType = false;
	selectMode = false;
}

bool MyEventReceiver::OnEvent(const SEvent& event){
	if(event.EventType == EET_MOUSE_INPUT_EVENT){
		if(event.MouseInput.isLeftPressed())
			objectSelectHandler();
	}else
	if(event.EventType == EET_KEY_INPUT_EVENT){
		if(event.KeyInput.Key == KEY_KEY_C && event.KeyInput.PressedDown){
			changeCameraTypeButtonHandler();
		}else
		if(event.KeyInput.Key == KEY_KEY_D && event.KeyInput.PressedDown){
			if(!objects.empty()){
				removeObjectHandler(3);
			}
		}else
		if(event.KeyInput.Key == KEY_KEY_S && event.KeyInput.PressedDown){
			if(selectMode) selectMode = false;
			else selectMode = true;
		}
	}else
	if (event.EventType == EET_GUI_EVENT){
		s32 id = event.GUIEvent.Caller->getID();
		IGUIEnvironment* env = device->getGUIEnvironment();
		switch(event.GUIEvent.EventType){
			case EGET_SCROLL_BAR_CHANGED:
				if(id == IMAGE_SCROLL_BAR_ID)
					imageScrollBarHandler(event);
				else
				if(id == ITEMS_SCROLL_BAR_ID)
					itemsScrollBarHandler(event);
				break;
			case EGET_BUTTON_CLICKED:
				switch(id){
					case ROTATE_BUTTON_ID:
						rotateButtonHandler();
						return true;
					case MOVE_BUTTON_ID:
						moveButtonHandler();
						return true;
					case SCALE_BUTTON_ID:
						scaleButtonHandler();
						return true;
					case CHANGE_CAMERA_TYPE_BUTTON_ID:
						changeCameraTypeButtonHandler();
						return true;
					case CHANGE_MODE_BUTTON_ID:
						changeModeButtonHandler();
						return true;
					case ADD_OBJECT_BUTTON_ID:
						addObjectButtonHandler();
						return true;
					case EXIT_BUTTON_ID:
						exitButtonHandler();
						return true;
					default:
						return false;
				}
				break;
			case EGET_FILE_SELECTED:
				selectedFileHandler(event);
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

void MyEventReceiver::addObjectButtonHandler(){
	device->getGUIEnvironment()->addFileOpenDialog(L"Please choose a file.", true, 0, -1, true);
}

void MyEventReceiver::exitButtonHandler(){
	device->closeDevice();
}

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

void MyEventReceiver::selectedFileHandler(const SEvent& event){
	IGUIFileOpenDialog* dialog = (IGUIFileOpenDialog*)event.GUIEvent.Caller;
	Object *obj = new Object(device);
	bool isAdded = obj->addAnObject(dialog->getFileName());
	if(isAdded){
		objects.push_back(*obj);
	}else{

	}
}

void MyEventReceiver::removeObjectHandler(int index){
	if(index >= objects.size() || index < 0)
		return;
	objects[index].removeObject();
	objects.erase(objects.begin()+index);
}

void MyEventReceiver::objectSelectHandler(){
	ISceneManager* smgr = device->getSceneManager();
	ISceneCollisionManager *colmgr = smgr->getSceneCollisionManager();
	line3df raytrace = colmgr->getRayFromScreenCoordinates(device->getCursorControl()->getPosition(),smgr->getActiveCamera());
	for(int i=0; i<objects.size(); i++){
		objectSelectHandlerHelper(objects[i],raytrace,colmgr);
	}
}

void MyEventReceiver::objectSelectHandlerHelper(Object obj, line3df raytrace,ISceneCollisionManager *colmgr){
	vector3df outCollisionPoint = vector3df(100,100,100);// & ları * yaptim
	triangle3df outTriangle = triangle3df();//degisiklik
	ISceneNode *outNode;
	colmgr->getCollisionPoint(raytrace, obj.triangleSelector, outCollisionPoint, outTriangle, outNode);
	if(obj.node->getBoundingBox().isPointInside(outCollisionPoint)){
		if(obj.isSelected) obj.isSelected = false;
		else obj.isSelected = true;
		obj.removeObject();
	}
}
