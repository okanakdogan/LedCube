#include"MyEventReceiver.h"

MyEventReceiver::MyEventReceiver(IrrlichtDevice *deviceV, Interface *intfaceV, vector<Object> objectsV, CubeObject *cubeV) : device(deviceV){
	intface = new Interface(intfaceV);
	cube = cubeV;
	for(int i=0; i<objectsV.size(); i++){
		objects.push_back(objectsV[i]);
	}

	collisionFlag = false ;
	ImageScrollBarOldPos = intface->ImageScrollbar->getPos();
	fpsCameraType = false;
	selectMode = false;
	otomaticFocus = false;
	drawMode =false;
	currentOp =  noop_id ;
	distanceToNode = 250;
	tempPosition= vector3df(-1000,-1000,-1000);
	int coneCount=0;
	int cubeCount=0;
	int cylinderCount=0;
	int pyramidCount=0;
	int rectangleCount=0;
	int squareCount=0;
	int drawCount=0;
	int arrow=0;
	int dumble=0;
	int circle=0;
	int man=0;
	int heart=0;
	int mug=0;
	int wolf=0;
	shapeIds.push_back(coneCount);
	shapeIds.push_back(cubeCount);
	shapeIds.push_back(cylinderCount);
	shapeIds.push_back(pyramidCount);
	shapeIds.push_back(rectangleCount);
	shapeIds.push_back(squareCount);
	shapeIds.push_back(drawCount);
	shapeIds.push_back(arrow);
	shapeIds.push_back(dumble);
	shapeIds.push_back(circle);
	shapeIds.push_back(man);
	shapeIds.push_back(heart);
	shapeIds.push_back(mug);
	shapeIds.push_back(wolf);
}

bool MyEventReceiver::OnEvent(const SEvent& event){
	// if the event type mouse event
	if(event.EventType == EET_MOUSE_INPUT_EVENT){
		if(event.MouseInput.Event == EMIE_MOUSE_WHEEL)
			mouseWheelEventHandler(event.MouseInput.Wheel);
		if(event.MouseInput.isLeftPressed()){
			mouseLeftButtonEventHandler();
		}
		if(event.MouseInput.Event == EMIE_LMOUSE_LEFT_UP){
			tempPosition= vector3df(-1000,-1000,-1000);
			if(drawMode){
				DrawingObject newDrawingObject = DrawingObject(currentDraw);
				shapeIds[6]++;
				std::string s="drawing ";
				stringstream ss;
				ss<<shapeIds[6];
				s.append(ss.str());
				wstring widestr = wstring(s.begin(), s.end());
				intface->Listbox->addItem(widestr.c_str());
				//char str2[64];
				//strncpy(str2, s.c_str(), sizeof(str2));
				//stringc strc= str2;
				newDrawingObject.name=s;
				drawingObj.push_back(newDrawingObject);
				currentDraw.clear();
				//drawMode=false;
			}
		}
		if(event.MouseInput.Event == EMIE_MOUSE_MOVED && drawMode && otomaticFocus){
			mouseMoveHandler();
		}
	}else
	// if the event type key input
	if(event.EventType == EET_KEY_INPUT_EVENT){
		// if user press c key then change camera type
		if(event.KeyInput.Key == KEY_KEY_C && event.KeyInput.PressedDown){
			changeCameraTypeButtonHandler();
		}else
		if(event.KeyInput.Key == KEY_KEY_H && event.KeyInput.PressedDown){
			if(collisionFlag) collisionFlag = false;
			else collisionFlag = true;
		}

		else
		if(event.KeyInput.Key == KEY_KEY_O && event.KeyInput.PressedDown){
			if(otomaticFocus) otomaticFocus = false;
			else otomaticFocus = true;
		}else
		// if user press d key then delete selected item
		if(event.KeyInput.Key == KEY_KEY_D && event.KeyInput.PressedDown){

			for(int i=0; i<objects.size(); ++i){
				if(objects[i].isSelected){
					removeObjectHandler(i);
				}
			}
			for(int i=0; i<drawingObj.size(); i++){
				if(drawingObj[i].isSelected){
					std::string sv = drawingObj[i].name;
					for(int j=0;j<intface->Listbox->getItemCount();j++)
					{
						wstring ws=intface->Listbox->getListItem(j);
						std::string s=wtoa(ws);

						if(s.compare(sv)==0)
						{
							intface->Listbox->removeItem(j);
							break;
						}
					}
					drawingObj[i].remove();
					drawingObj.erase(drawingObj.begin()+i);
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
			//cube->setVisible(cubeVisible);
			for(int i=0; i<objects.size(); i++){
				objects[i].node->setVisible(cubeVisible);
			}
			for(int i=0; i<drawingObj.size(); i++){
				drawingObj[i].setVisible(cubeVisible);
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
			DrawingObject::groupIdCount++;
		}

		else if(event.KeyInput.Key == KEY_KEY_N && event.KeyInput.PressedDown){
			currentOp = noop_id;
			DrawingObject draw;
			draw.node  =  currentDraw;
			drawingObj.push_back(draw);
			drawMode = false;
			selectMode = false;

		}else if(event.KeyInput.Key == KEY_KEY_Q && event.KeyInput.PressedDown)
			clearDrawingEvent();
		else if(event.KeyInput.Key == KEY_KEY_A && event.KeyInput.PressedDown){
			for(int i=0; i<objects.size(); i++){
				if(objects[i].isSelected){
					if(objects[i].anim->animationMode) objects[i].anim->animationMode = false;
					else objects[i].anim->animationMode = true;
				}
			}
			for(int i=0; i<drawingObj.size(); i++){
				if(drawingObj[i].isSelected){
					if(drawingObj[i].anim->animationMode) drawingObj[i].anim->animationMode = false;
					else drawingObj[i].anim->animationMode = true;
				}
			}
		}
		else if(event.KeyInput.Key == KEY_SPACE && event.KeyInput.PressedDown){
			for(int i=0; i<objects.size(); i++){
				if(objects[i].isSelected){
					objects[i].anim->animationType = (objects[i].anim->animationType+1)%8;
					cout << objects[i].anim->animationType << endl;
				}
			}
			for(int i=0; i<drawingObj.size(); i++){
				if(drawingObj[i].isSelected){
					drawingObj[i].anim->animationType = (drawingObj[i].anim->animationType+1)%8;
				}
			}
		}
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
					case ARROW_IMAGE_ID:
						arrowImageHandler();
						return true;
					case CIRCLE_IMAGE_ID:
						circleImageHandler();
						return true;
					case DUMBLE_IMAGE_ID:
						dumbleImageHandler();
						return true;
					case MAN_IMAGE_ID:
						manImageHandler();
						return true;
					case KALP_IMAGE_ID:
						kalpImageHandler();
						return true;
					case KUPA_IMAGE_ID:
						kupaImageHandler();
						return true;
					case KURT_IMAGE_ID:
						kurtImageHandler();
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
	currentOp = rotate_id;
}

void MyEventReceiver::moveButtonHandler(){
	selectMode = false;
	drawMode = false;
	currentOp = move_id;
}

void MyEventReceiver::clearDrawingEvent()
{
	for(int i = 0 ; i < drawingObj.size(); i++)
	{
		for(int j=0;j<intface->Listbox->getItemCount();j++)
		{
			wstring ws=intface->Listbox->getListItem(j);
			std::string s=wtoa(ws);

			if(s.compare(drawingObj[i].name)==0)
			{
				//cout<< "i "<<i<<endl<<wtoa(intface->Listbox->getListItem(i))<<endl;
				//list<IGUIElement*> children = intface->Listbox->getChildren();
				intface->Listbox->removeItem(j);
				break;


			}
		}
		drawingObj[i].remove();
	}
	drawingObj.clear();
}

void MyEventReceiver::scaleButtonHandler(){
	selectMode = false;
	drawMode = false;
	currentOp = scale_id;
}

void MyEventReceiver::chooseButtonHandler(){
	selectMode = true;
	drawMode = false;
	if(intface->Listbox->getSelected() >= 0)
	{
		int index =intface->Listbox->getSelected();
		wstring ws=intface->Listbox->getListItem(index);
		std::string s= wtoa(ws);
		listObjectSelect(s);

		intface->Listbox->setSelected(-1);

	}
	currentOp = choose_id;
}

std::string MyEventReceiver::wtoa(const std::wstring& wide)
{
    std::string str;
    for(std::wstring::const_iterator it = wide.begin();
        it != wide.end();
        ++it)
    {
        str.push_back(static_cast<char>(*it));
    }
    return str;
}

void MyEventReceiver::listObjectSelect(std::string str){
	vector3df commonPoint;
	int k=0;

	if(str[0] == 'd' && str[1] == 'r' && str[2] == 'a' && str[3] == 'w')
		{
			for(int j=0;j<drawingObj.size(); j++)
			{
				if(str.compare(drawingObj[j].name)==0)
				{
					ICameraSceneNode *camera = device->getSceneManager()->getActiveCamera();
					camera->setTarget(drawingObj[j].getAbsolutePosition());
					drawingObj[j].select();
				} else
					drawingObj[j].unSelect();
			}
			for(int  i=0;i<objects.size();i++)
			{
				for(int k=0; k<objects[i].node->getMaterialCount(); k++)
					objects[i].node->getMaterial(k).EmissiveColor.set(255, 0, 0, 0);
				objects[i].isSelected =false;
			}
			return;
	}
	else
	for(int i=0; i<objects.size(); i++)
	{


		if(str.compare(objects[i].node->getName())==0){

			for(int k=0; k<objects[i].node->getMaterialCount(); k++)
				objects[i].node->getMaterial(k).EmissiveColor.set(255, 255, 255, 0);

			ICameraSceneNode *camera = device->getSceneManager()->getActiveCamera();
			//line3df ray(camera->getAbsolutePosition(),objects[i].node->getAbsolutePosition());

			/*vector3df cameraPosition= objects[i].node->getAbsolutePosition();
			vector3df temp(-400,0,0);*/
			//camera->setPosition(cameraPosition+temp);
			camera->setTarget(objects[i].node->getAbsolutePosition());
			objects[i].isSelected =true;
		}else{
			for(int k=0; k<objects[i].node->getMaterialCount(); k++)
				objects[i].node->getMaterial(k).EmissiveColor.set(255, 0, 0, 0);
			objects[i].isSelected =false;
		}

	}
	for(int j=0;j<drawingObj.size(); j++)
		drawingObj[j].unSelect();

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
	device->getGUIEnvironment()->addStaticText(L"N tusu : Secilen islemin iptalini saglar.",
						rect<s32>(20,50,980,80),true,false,window);
	device->getGUIEnvironment()->addStaticText(L"Ctrl+L :  butun objeleri gorunmez yapar",
						rect<s32>(20,80,980,110),true,false,window);
	device->getGUIEnvironment()->addStaticText(L"C tusu : kamera modu degistirir",
						rect<s32>(20,110,980,140),true,false,window);
	device->getGUIEnvironment()->addStaticText(L"H tusu : Collision acip kapama",
						rect<s32>(20,140,980,170),true,false,window);
	device->getGUIEnvironment()->addStaticText(L"Ctrl+P : cizme modunu degistirir",
						rect<s32>(20,170,980,200),true,false,window);
	device->getGUIEnvironment()->addStaticText(L"A  tusu : secili obje veya objelerin animasyonunu baslatir veya kapatir.",
						rect<s32>(20,200,980,230),true,false,window);
	device->getGUIEnvironment()->addStaticText(L"Space tusu :  secili obje veya objelerin animasyon turunu degistirir.",
						rect<s32>(20,230,980,260),true,false,window);
	device->getGUIEnvironment()->addStaticText(L"Q tusu :  sahnedeki butun DrawingObject objelerini temizler . ",
						rect<s32>(20,260,980,290),true,false,window);
	device->getGUIEnvironment()->addStaticText(L"D tusu :  secilen obje veya objeleri siler.",
						rect<s32>(20,290,980,320),true,false,window);
	device->getGUIEnvironment()->addStaticText(L"S tusu :  secim modunu  degistirir.",
						rect<s32>(20,320,980,350),true,false,window);
	device->getGUIEnvironment()->addStaticText(L"G tuþu : ekranda secili olan objeleri gruplar",
						rect<s32>(20,350,980,380),true,false,window);
	device->getGUIEnvironment()->addStaticText(L"Ok tusları :  kamera serbest modda iken  kameranin ileri-geri ve sol-sag hareketlerini yapar.",
						rect<s32>(20,380,980,410),true,false,window);
	device->getGUIEnvironment()->addStaticText(L"•	O tusu : Otomatik focus modunu degistirir.",
						rect<s32>(20,410,980,440),true,false,window);
}

// add a cone object from media/primitives


void MyEventReceiver::coneImageHandler(){
	Object *obj = new Object(device);
	bool isAdded = obj->addAnObject(L"media/primitives/cone/cone.obj");
	if(isAdded){
		shapeIds[0]++;
		std::string str="cone ";

		stringstream ss;
		ss<<shapeIds[0];
		str.append(ss.str());

		wstring widestr = wstring(str.begin(), str.end());
		intface->Listbox->addItem(widestr.c_str());

		char str2[64];
		strncpy(str2, str.c_str(), sizeof(str2));

		stringc strc= str2;
		obj->node->setName(strc);

		objects.push_back(*obj);
	}else{

	}
}

// add a cube object from media/primitives
void MyEventReceiver::cubeImageHandler(){
	Object *obj = new Object(device);
	bool isAdded = obj->addAnObject(L"media/primitives/cube/cube.obj");
	if(isAdded){
		shapeIds[1]++;
		std::string str="cube ";

		stringstream ss;
		ss<<shapeIds[1];
		str.append(ss.str());

		wstring widestr = wstring(str.begin(), str.end());
		intface->Listbox->addItem(widestr.c_str());

		char str2[64];
		strncpy(str2, str.c_str(), sizeof(str2));
		str2[sizeof(str2) - 1] = 0;

		stringc strc= str2;
		obj->node->setName(strc);

		objects.push_back(*obj);
	}else{

	}
}

// add a cylinder object from media/primitives
void MyEventReceiver::cylinderImageHandler(){
	Object *obj = new Object(device);
	bool isAdded = obj->addAnObject(L"media/primitives/cylinder/cylinder.obj");
	if(isAdded){
		shapeIds[2]++;
		std::string str="cylinder ";

		stringstream ss;
		ss<<shapeIds[2];
		str.append(ss.str());

		wstring widestr = wstring(str.begin(), str.end());
		intface->Listbox->addItem(widestr.c_str());

		char str2[64];
		strncpy(str2, str.c_str(), sizeof(str2));

		stringc strc= str2;
		obj->node->setName(strc);

		objects.push_back(*obj);
	}else{

	}
}

// add a pyramid object from media/primitives
void MyEventReceiver::pyramidImageHandler(){
	Object *obj = new Object(device);
	bool isAdded = obj->addAnObject(L"media/primitives/pyramid/pyramid.obj");
	if(isAdded){
		shapeIds[3]++;
		std::string str="pyramid ";

		stringstream ss;
		ss<<shapeIds[3];
		str.append(ss.str());

		wstring widestr = wstring(str.begin(), str.end());
		intface->Listbox->addItem(widestr.c_str());

		char str2[64];
		strncpy(str2, str.c_str(), sizeof(str2));

		stringc strc= str2;
		obj->node->setName(strc);

		objects.push_back(*obj);
	}else{

	}
}

// add a rectangle object from media/primitives
void MyEventReceiver::rectangleImageHandler(){
	Object *obj = new Object(device);
	bool isAdded = obj->addAnObject(L"media/primitives/rectangle/rectangle.obj");
	if(isAdded){
		shapeIds[4]++;
		std::string str="rectangle ";

		stringstream ss;
		ss<<shapeIds[4];
		str.append(ss.str());

		wstring widestr = wstring(str.begin(), str.end());
		intface->Listbox->addItem(widestr.c_str());

		char str2[64];
		strncpy(str2, str.c_str(), sizeof(str2));

		stringc strc= str2;
		obj->node->setName(strc);

		objects.push_back(*obj);
	}else{

	}
}

// add a square object from media/primitives
void MyEventReceiver::squareImageHandler(){
	Object *obj = new Object(device);
	bool isAdded = obj->addAnObject(L"media/primitives/square/square.obj");
	if(isAdded){
		shapeIds[5]++;
		std::string str="square ";

		stringstream ss;
		ss<<shapeIds[5];
		str.append(ss.str());


		wstring widestr = wstring(str.begin(), str.end());
		intface->Listbox->addItem(widestr.c_str());


		char str2[64];
		strncpy(str2, str.c_str(), sizeof(str2));

		stringc strc= str2;
		obj->node->setName(strc);

		objects.push_back(*obj);
	}else{

	}
}

// add a arrow object from media/primitives
void MyEventReceiver::arrowImageHandler(){
	Object *obj = new Object(device);
	bool isAdded = obj->addAnObject(L"media/primitives/Çizgili_OK/ok.obj");
	if (isAdded){
		shapeIds[7]++;
		std::string str="arrow ";

		stringstream ss;
		ss<<shapeIds[7];
		str.append(ss.str());


		wstring widestr = wstring(str.begin(), str.end());
		intface->Listbox->addItem(widestr.c_str());


		char str2[64];
		strncpy(str2, str.c_str(), sizeof(str2));

		stringc strc= str2;
		obj->node->setName(strc);
		objects.push_back(*obj);
	}
	else{

	}
}

// add a circle object from media/primitives
void MyEventReceiver::circleImageHandler(){
	Object *obj = new Object(device);
	bool isAdded = obj->addAnObject(L"media/primitives/simit/simit.obj");
	if (isAdded){
		shapeIds[8]++;
		std::string str="circle ";

		stringstream ss;
		ss<<shapeIds[8];
		str.append(ss.str());


		wstring widestr = wstring(str.begin(), str.end());
		intface->Listbox->addItem(widestr.c_str());


		char str2[64];
		strncpy(str2, str.c_str(), sizeof(str2));

		stringc strc= str2;
		obj->node->setName(strc);
		objects.push_back(*obj);
	}
	else{

	}
}
// add a dumbell object from media/primitives
void MyEventReceiver::dumbleImageHandler(){
	Object *obj = new Object(device);
	bool isAdded = obj->addAnObject(L"media/primitives/dumbell/dumbell.obj");
	if (isAdded){
		shapeIds[9]++;
		std::string str="dumble ";

		stringstream ss;
		ss<<shapeIds[9];
		str.append(ss.str());


		wstring widestr = wstring(str.begin(), str.end());
		intface->Listbox->addItem(widestr.c_str());


		char str2[64];
		strncpy(str2, str.c_str(), sizeof(str2));

		stringc strc= str2;
		obj->node->setName(strc);
		objects.push_back(*obj);
	}
	else{

	}
}

// add a man object from media/primitives
void MyEventReceiver::manImageHandler(){
	Object *obj = new Object(device);
	bool isAdded = obj->addAnObject(L"media/primitives/adam1/adam1.obj");
	if (isAdded){
		shapeIds[10]++;
		std::string str="man ";

		stringstream ss;
		ss<<shapeIds[10];
		str.append(ss.str());


		wstring widestr = wstring(str.begin(), str.end());
		intface->Listbox->addItem(widestr.c_str());


		char str2[64];
		strncpy(str2, str.c_str(), sizeof(str2));

		stringc strc= str2;
		obj->node->setName(strc);
		objects.push_back(*obj);
	}
	else{

	}
}

// add a kalp object from media/primitives
void MyEventReceiver::kalpImageHandler(){
	Object *obj = new Object(device);
	bool isAdded = obj->addAnObject(L"media/primitives/Kalp/Heart.obj");
	if (isAdded){
		shapeIds[11]++;
		std::string str="heart ";

		stringstream ss;
		ss<<shapeIds[11];
		str.append(ss.str());


		wstring widestr = wstring(str.begin(), str.end());
		intface->Listbox->addItem(widestr.c_str());


		char str2[64];
		strncpy(str2, str.c_str(), sizeof(str2));

		stringc strc= str2;
		obj->node->setName(strc);
		objects.push_back(*obj);
	}
	else{

	}
}

// add a kupa object from media/primitives
void MyEventReceiver::kupaImageHandler(){
	Object *obj = new Object(device);
	bool isAdded = obj->addAnObject(L"media/primitives/kupa/kupa.obj");
	if (isAdded){
		shapeIds[12]++;
		std::string str="mug ";

		stringstream ss;
		ss<<shapeIds[12];
		str.append(ss.str());


		wstring widestr = wstring(str.begin(), str.end());
		intface->Listbox->addItem(widestr.c_str());


		char str2[64];
		strncpy(str2, str.c_str(), sizeof(str2));

		stringc strc= str2;
		obj->node->setName(strc);
		objects.push_back(*obj);
	}
	else{

	}
}

// add a kurt object from media/primitives
void MyEventReceiver::kurtImageHandler(){
	Object *obj = new Object(device);
	bool isAdded = obj->addAnObject(L"media/primitives/kurt/kurt.obj");
	if (isAdded){

		shapeIds[13]++;
		std::string str="wolf ";

		stringstream ss;
		ss<<shapeIds[13];
		str.append(ss.str());


		wstring widestr = wstring(str.begin(), str.end());
		intface->Listbox->addItem(widestr.c_str());


		char str2[64];
		strncpy(str2, str.c_str(), sizeof(str2));

		stringc strc= str2;
		obj->node->setName(strc);

		objects.push_back(*obj);
	}
	else{

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


	for(int i=0;i<intface->Listbox->getItemCount();i++)
	{
		wstring ws=intface->Listbox->getListItem(i);
		std::string s=wtoa(ws);

		if(s.compare(objects[index].node->getName())==0)
		{
			//cout<< "i "<<i<<endl<<wtoa(intface->Listbox->getListItem(i))<<endl;
			//list<IGUIElement*> children = intface->Listbox->getChildren();
			intface->Listbox->removeItem(i);
			break;


		}
	}

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
					for(int k=0; k<objects[i].node->getMaterialCount(); k++)
						objects[i].node->getMaterial(k).EmissiveColor.set(255, 0,   0,   0);
				}else{
					for(int j=0; j<objects.size(); j++){
						if(objects[j].groupId == groupId){
							objects[j].isSelected = false;
							for(int k=0; k<objects[i].node->getMaterialCount(); k++)
								objects[j].node->getMaterial(k).EmissiveColor.set(255, 0,   0,   0);
						}
					}
				}
			}
			else{
				int groupId = objects[i].groupId;
				if(groupId == -1){
					objects[i].isSelected = true;
					for(int k=0; k<objects[i].node->getMaterialCount(); k++)
						objects[i].node->getMaterial(k).EmissiveColor.set(255, 255, 255, 0);
				}else{
					for(int j=0; j<objects.size(); j++){
						if(objects[j].groupId == groupId){
							objects[j].isSelected = true;
							for(int k=0; k<objects[i].node->getMaterialCount(); k++)
								objects[j].node->getMaterial(k).EmissiveColor.set(255, 255, 255, 0);
						}
					}
				}
			}
		}
	}
	vector3df outCollisionPoint = vector3df(1000,1000,1000);
	triangle3df outTriangle = triangle3df();
	ISceneNode *outNode;
	for(int i=0; i<drawingObj.size(); i++){
		if(drawingObj[i].isCollisionDetach(raytrace, &outCollisionPoint, &outTriangle, outNode,colmgr)){
			int groupId = drawingObj[i].groupId;
			if(drawingObj[i].isSelected){
				if(groupId == -1){
					drawingObj[i].unSelect();
				}else{
					for(int j=0; j<drawingObj.size(); j++){
						if(groupId == drawingObj[j].groupId){
							drawingObj[j].unSelect();
						}
					}
				}
			}else{
				if(groupId == -1){
					drawingObj[i].select();
				}else{
					for(int j=0; j<drawingObj.size(); j++){
						if(groupId == drawingObj[j].groupId){
							drawingObj[j].select();
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

		//if(otomaticFocus)
			mousePosition = setLineLength(ray,distanceToNode);
		//else
			//mousePosition = setLineLength(ray,250);
		if(tempPosition==vector3df(-1000,-1000,-1000))
			tempPosition=mousePosition;
		ISceneNode *cube1 = device->getSceneManager()->addCubeSceneNode(6.0f);
		cube1->setPosition(mousePosition);
		Node obj(cube1,device) ;
		currentDraw.push_back(obj);

		float distance= tempPosition.getDistanceFrom(mousePosition);

		int cubeCount=distance/2;

		float x_ = -(tempPosition.X-mousePosition.X);
		float y_ = -(tempPosition.Y-mousePosition.Y);
		float z_ = -(tempPosition.Z-mousePosition.Z);

		if(cubeCount != 0){
			float e = 1/(double)cubeCount ;

			for(int i = 0 ; i < cubeCount ; i++)
			{
				ISceneNode *cube = device->getSceneManager()->addCubeSceneNode(6.0f);
				cube->setPosition(vector3df(tempPosition.X+x_*e*i,tempPosition.Y+y_*e*i,tempPosition.Z+z_*e*i));
				Node obj(cube,device) ;
				currentDraw.push_back(obj);
			}
		}
	}else{
		ICameraSceneNode *camera = device->getSceneManager()->getActiveCamera();

		line3df ray = device->getSceneManager()->getSceneCollisionManager()->
			getRayFromScreenCoordinates(device->getCursorControl()->getPosition(),device->getSceneManager()->getActiveCamera());

		//if(otomaticFocus)
			mousePosition = setLineLength(ray,distanceToNode);
		//else
			//mousePosition = setLineLength(ray,250);

		if(tempPosition==vector3df(-1000,-1000,-1000))
			tempPosition=mousePosition;
		ISceneNode *cube1 = device->getSceneManager()->addCubeSceneNode(6.0f);
		cube1->setPosition(mousePosition);
		Node obj(cube1,device) ;
		currentDraw.push_back(obj);

		float distance= tempPosition.getDistanceFrom(mousePosition);

		int cubeCount=distance/2;

		float x_ = -(tempPosition.X-mousePosition.X);
		float y_ = -(tempPosition.Y-mousePosition.Y);
		float z_ = -(tempPosition.Z-mousePosition.Z);

		if(cubeCount != 0){
			float e = 1/(double)cubeCount ;

			for(int i = 0 ; i < cubeCount ; i++)
			{
				ISceneNode *cube = device->getSceneManager()->addCubeSceneNode(6.0f);
				cube->setPosition(vector3df(tempPosition.X+x_*e*i,tempPosition.Y+y_*e*i,tempPosition.Z+z_*e*i));
				Node obj(cube,device) ;
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
					if(wheelValue == 1){
						objects[i].node->setScale(vector3df(objScale.X+0.025,objScale.Y+0.025,objScale.Z+0.025));
					}else if(!(objScale.X < 0.1 && objScale.Y < 0.1 && objScale.Z < 0.1)){
						objects[i].node->setScale(vector3df(objScale.X-0.025,objScale.Y-0.025,objScale.Z-0.025));
					}
				}
			}
			for(int i=0; i<drawingObj.size(); i++){
				if(drawingObj[i].isSelected){
					vector3df objScale = drawingObj[i].node[0].node->getScale();
					if(wheelValue == 1){
						drawingObj[i].setScale(vector3df(objScale.X+0.025,objScale.Y+0.025,objScale.Z+0.025),-1);
					}else if(!(objScale.X < 0.1 && objScale.Y < 0.1 && objScale.Z < 0.1)){
						drawingObj[i].setScale(vector3df(objScale.X-0.025,objScale.Y-0.025,objScale.Z-0.025),+1);
					}
				}
			}
	}
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
				vector3df commonPoint = vector3df(0,0,0);
				int k=0;
				for(int i=0; i<objects.size(); i++){
					if(objects[i].isSelected){
						commonPoint += objects[i].node->getAbsolutePosition();
						k++;
					}
				}
				commonPoint /= k;
				vector3df difVec = newMousePos-commonPoint;
				for(int i = 0 ; i < objects.size(); i++){
					if(objects[i].isSelected){
						vector3df oldPosition = objects[i].node->getAbsolutePosition();
						vector3df newPosition = oldPosition + difVec;
						objects[i].node->setPosition(newPosition);
					}
				}
				commonPoint = vector3df(0,0,0);
				k=0;
				for(int i=0; i<drawingObj.size(); i++){
					if(drawingObj[i].isSelected){
						commonPoint += drawingObj[i].getAbsolutePosition();
						k++;
					}
				}
				commonPoint /= k;
				difVec = newMousePos - commonPoint;
				for(int i=0; i<drawingObj.size(); i++){
					if(drawingObj[i].isSelected){
						vector3df oldPosition = drawingObj[i].getAbsolutePosition();
						vector3df newPosition = vector3df(oldPosition + difVec);
						drawingObj[i].setPosition(newPosition);
					}
				}
			}else{
				line3df ray = colmgr->getRayFromScreenCoordinates(device->getCursorControl()->getPosition(),smgr->getActiveCamera());
				vector3df newMousePos = setLineLength(ray,250);
				vector3df commonPoint = vector3df(0,0,0);
				int k=0;
				for(int i=0; i<objects.size(); i++){
					if(objects[i].isSelected){
						commonPoint += objects[i].node->getAbsolutePosition();
						k++;
					}
				}
				commonPoint /= k;
				vector3df difVec = newMousePos-commonPoint;
				for(int i = 0 ; i < objects.size(); i++){
					if(objects[i].isSelected){
						vector3df oldPosition = objects[i].node->getAbsolutePosition();
						vector3df newPosition = oldPosition + difVec;
						objects[i].node->setPosition(newPosition);
					}
				}
				commonPoint = vector3df(0,0,0);
				k=0;
				for(int i=0; i<drawingObj.size(); i++){
					if(drawingObj[i].isSelected){
						commonPoint += drawingObj[i].getAbsolutePosition();
						k++;
					}
				}
				commonPoint /= k;
				difVec = newMousePos - commonPoint;
				for(int i=0; i<drawingObj.size(); i++){
					if(drawingObj[i].isSelected){
						vector3df oldPosition = drawingObj[i].getAbsolutePosition();
						vector3df newPosition = vector3df(oldPosition + difVec);
						drawingObj[i].setPosition(newPosition);
					}
				}
			}
			break ;
		case rotate_id:
			for(int i = 0 ; i < objects.size(); i++){
				if(objects[i].isSelected){
					if(fpsCameraType){
						line3df ray(device->getSceneManager()->getActiveCamera()->getAbsolutePosition(),
										device->getSceneManager()->getActiveCamera()->getTarget());
						mousePosition = setLineLength(ray,250);
					}
					else
					{
						line3df ray = colmgr->getRayFromScreenCoordinates(device->getCursorControl()->getPosition(),smgr->getActiveCamera());
						mousePosition = setLineLength(ray,250);
					}

					if(tempPosition!=vector3df(-1000,-1000,-1000)){
						if(mousePosition.X < tempPosition.X && mousePosition.Y < tempPosition.Y )
							objects[i].node->setRotation(objects[i].node->getPosition().rotationToDirection
								(core::vector3df(-mousePosition.Y/2, mousePosition.X/2, 0)));
						if(mousePosition.X >= tempPosition.X && mousePosition.Y >= tempPosition.Y)
							objects[i].node->setRotation(objects[i].node->getPosition().rotationToDirection
								(core::vector3df(mousePosition.Y/2, -mousePosition.X/2, 0)));
						if(mousePosition.X >= tempPosition.X && mousePosition.Y < tempPosition.Y)
							objects[i].node->setRotation(objects[i].node->getPosition().rotationToDirection
								(core::vector3df(mousePosition.Y/2, mousePosition.X/2, 0)));
						if(mousePosition.X < tempPosition.X && mousePosition.Y >= tempPosition.Y)
							objects[i].node->setRotation(objects[i].node->getPosition().rotationToDirection
								(core::vector3df(-mousePosition.Y/2, -mousePosition.X/2, 0)));

					}
				}
			}
			for(int i  = 0 ; i < drawingObj.size() ; i++)
			{
				if(drawingObj[i].isSelected)
				{
					//ISceneNode *myNode = device->getSceneManager()->addCubeSceneNode(10.0f);
					ISceneNode *myNode =  drawingObj[i].node[0].node;
					myNode->setPosition(drawingObj[i].node[0].node->getAbsolutePosition());
					for(int j  = 0 ; j < drawingObj[i].node.size() ; j++)
						 myNode->addChild(drawingObj[i].node[j].node);


					if(fpsCameraType){
						line3df ray(device->getSceneManager()->getActiveCamera()->getAbsolutePosition(),
										device->getSceneManager()->getActiveCamera()->getTarget());
						mousePosition = setLineLength(ray,250);
					}
					else
					{
						line3df ray = colmgr->getRayFromScreenCoordinates(device->getCursorControl()->getPosition(),smgr->getActiveCamera());
						mousePosition = setLineLength(ray,250);
					}
					vector3df rot  = myNode->getRotation();
					if(tempPosition!=vector3df(-1000,-1000,-1000)){
						if(mousePosition.X < tempPosition.X && mousePosition.Y < tempPosition.Y )
							 myNode->setRotation(vector3df(rot.X+5,0,0));
						if(mousePosition.X >= tempPosition.X && mousePosition.Y >= tempPosition.Y)
							 myNode->setRotation(vector3df(rot.X+5,0,0));
						if(mousePosition.X >= tempPosition.X && mousePosition.Y < tempPosition.Y)
							 myNode->setRotation(vector3df(rot.X+5,0,0));
						if(mousePosition.X < tempPosition.X && mousePosition.Y >= tempPosition.Y)
							 myNode->setRotation(vector3df(rot.X+5,0,0));

					}
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
	int groupId = -1;
	for(int i=0; i<objects.size(); ++i){
		if(objects[i].isSelected){
			groupId = Object::groupIdCount;
			objects[i].groupId = Object::groupIdCount;
		}
	}
	std::string str="";

	stringstream ss;
	ss<<groupId;
	str.append(ss.str());

	wstring widestr = wstring(str.begin(), str.end());
	intface->Listbox->addItem(widestr.c_str());

	for(int i=0; i<drawingObj.size(); i++){
		if(drawingObj[i].isSelected){
			drawingObj[i].groupId = DrawingObject::groupIdCount;
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

	for(int i=0; i<drawingObj.size(); i++){
		for(int i=0; i<drawingObj[i].node.size(); i++){
			bool isGet = drawingObj[i].isCollisionDetach(ray,&outCollisionPoint,&outTriangle, outNode,colmgr) ;
			if(isGet){
				line3df line = line3df(smgr->getActiveCamera()->getAbsolutePosition(), outCollisionPoint);
				distanceToNode = line.getLength();
				isSet = true;
				break;
			}

		}
	}
}
