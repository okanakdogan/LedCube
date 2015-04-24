#include"Interface.h"


#define MODEL_LOGO_CONE_PATH "media/cone.png"
#define MODEL_LOGO_CUBE_PATH "media/cube.png"
#define MODEL_LOGO_CYLINDER_PATH "../media/cylinder.png"


Interface::Interface(IrrlichtDevice *device, IVideoDriver* driver){
	/* creating locations */
	l.CONE_IMAGE_LOCATION = recti(30, 0, 250, 130);
	l.CUBE_IMAGE_LOCATION = recti(30, 130, 250, 260);
	l.CYLINDER_IMAGE_LOCATION = recti(30, 260, 250, 390);
	l.PYRAMID_IMAGE_LOCATION = recti(30, 390, 250, 520);
	l.RECTANGLE_IMAGE_LOCATION = recti(30, 520, 250, 650);
	l.SQUARE_IMAGE_LOCATION = recti(30, 650, 250, 780);

	l.ROTATE_BUTTON_LOCATION = rect<s32>(0,0,100,50);
	l.MOVE_BUTTON_LOCATION = rect<s32>(100,0,200,50);
	l.SCALE_BUTTON_LOCATION = rect<s32>(200,0,300,50);
	l.CHANGE_CAMERA_TYPE_BUTTON_LOCATION = rect<s32>(0,100,300,150);
	l.CHANGE_MODE_BUTTON_LOCATION = rect<s32>(300,100,480,150);
	l.ADD_OBJECT_BUTTON_LOCATION = rect<s32>(690,0,850,50);
	l.EXIT_BUTTON_LOCATION = rect<s32>(690,100,780,150);

	l.IMAGE_SCROLL_BAR_LOCATION = rect<s32>(250, 0, 270, 700);
	l.ITEMS_SCROLL_BAR_LOCATION = rect<s32>(220, 0, 240, 160);
	/**********************/

	guienv = device->getGUIEnvironment();
	skin = guienv->getSkin();
	font = guienv->getFont("../media/font/myfont.xml");
	skin->setFont(font);
	skin->setFont(guienv->getBuiltInFont(), EGDF_TOOLTIP);

	guienv->addStaticText(L"THIS IS THE DRAWING SHAPE AREA",
		rect<int>(0,0,470,50), true)->setOverrideColor(SCOL_GREEN);

	IGUITabControl * rightTabControl= guienv->addTabControl(rect<s32>(FULLSCREEN.Width*0.80,0,FULLSCREEN.Width,FULLSCREEN.Height),0,true,true,-1);
    rightTab=rightTabControl->addTab(L"PRIMITIVES",-1);
	rightTab->setTextColor(SCOL_GREEN);

	ImageScrollbar = guienv->addScrollBar(false,l.IMAGE_SCROLL_BAR_LOCATION,rightTab,IMAGE_SCROLL_BAR_ID);
	ImageScrollbar->setMax(700);
	ImageScrollbar->setPos(0);
	rightTab->addChild(ImageScrollbar);

    IGUITabControl * menuTabControl= guienv->addTabControl(rect<s32>(250,FULLSCREEN.Height*0.70,FULLSCREEN.Width*0.80,FULLSCREEN.Height),0,true,true,-1);
    menuTab=menuTabControl->addTab(L"MENU",-1);
	menuTab->setTextColor(SCOL_GREEN);

	IGUITabControl * itemsTabControl= guienv->addTabControl(rect<s32>(0,FULLSCREEN.Height*0.70,FULLSCREEN.Width*0.184,FULLSCREEN.Height),0,true,true,-1);
    itemsTab=itemsTabControl->addTab(L"ITEMS",-1);
	itemsTab->setTextColor(SCOL_GREEN);

	ItemsScrollbar = guienv->addScrollBar(false,l.ITEMS_SCROLL_BAR_LOCATION,itemsTab,ITEMS_SCROLL_BAR_ID);
	ItemsScrollbar->setMax(100);
	ItemsScrollbar->setPos(0);
	itemsTab->addChild(ItemsScrollbar);

	for (s32 i=0; i<irr::gui::EGDC_COUNT ; ++i)
    {
        SColor col = guienv->getSkin()->getColor((EGUI_DEFAULT_COLOR)i);
        col.setAlpha(255);
        guienv->getSkin()->setColor((EGUI_DEFAULT_COLOR)i, col);
    }

	/* CREATING BUTTONS */
	RotateButton = guienv->addButton(l.ROTATE_BUTTON_LOCATION,menuTab,ROTATE_BUTTON_ID,L"Rotate",L"Rotates The Selected Object");
	MoveButton = guienv->addButton(l.MOVE_BUTTON_LOCATION,menuTab,MOVE_BUTTON_ID,L"Move",L"Moves The Selected Object");
	ScaleButton = guienv->addButton(l.SCALE_BUTTON_LOCATION,menuTab,SCALE_BUTTON_ID,L"Scale",L"Scales The Selected Object");
	ChangeCameraTypeButton = guienv->addButton(l.CHANGE_CAMERA_TYPE_BUTTON_LOCATION,menuTab,CHANGE_CAMERA_TYPE_BUTTON_ID,
													L"Change Camera Type(KEY C)",	L"Change Camera Type To Free");
	ChangeModeButton = guienv->addButton(l.CHANGE_MODE_BUTTON_LOCATION,menuTab,CHANGE_MODE_BUTTON_ID,L"Change Mode");
	AddObjectButton = guienv->addButton(l.ADD_OBJECT_BUTTON_LOCATION,menuTab,ADD_OBJECT_BUTTON_ID,L"Add Object",L"Add A Specific Object");
	ExitButton = guienv->addButton(l.EXIT_BUTTON_LOCATION,menuTab,EXIT_BUTTON_ID,L"Exit",L"Exit From Program");
	/*******************/

	/* CREATING IMAGES */
	coneImage = guienv->addImage(l.CONE_IMAGE_LOCATION, rightTab, CONE_IMAGE_ID);
	IGUIStaticText *st = guienv->addStaticText(L"Cone Object",rect<s32>(0,101,220,130),true,true,coneImage,-1,false);
	st->setOverrideColor(SCOL_BLUE);
	coneImage->addChild(st);
	coneImage->setToolTipText(L"To Add Cone Object CLICK!");
	coneImage->setImage(driver->getTexture("../media/cone.png"));

	cubeImage = guienv->addImage(l.CUBE_IMAGE_LOCATION, rightTab, CUBE_IMAGE_ID);
	st = guienv->addStaticText(L"Cube Object",rect<s32>(0,101,220,130),true,true,cubeImage,-1,false);
	st->setOverrideColor(SCOL_BLUE);
	cubeImage->addChild(st);
	cubeImage->setToolTipText(L"To Add Cube Object CLICK!");
	cubeImage->setImage(driver->getTexture("../media/cube.png"));

	cylinderImage = guienv->addImage(l.CYLINDER_IMAGE_LOCATION, rightTab, CYLINDER_IMAGE_ID);
	st = guienv->addStaticText(L"Cylinder Object",rect<s32>(0,101,220,130),true,true,cylinderImage,-1,false);
	st->setOverrideColor(SCOL_BLUE);
	cylinderImage->addChild(st);
	cylinderImage->setToolTipText(L"To Add Cylinder Object CLICK!");
	cylinderImage->setImage(driver->getTexture("../media/cylinder.png"));

	pyramidImage = guienv->addImage(l.PYRAMID_IMAGE_LOCATION, rightTab, PYRAMID_IMAGE_ID);
	st = guienv->addStaticText(L"Pyramid Object",rect<s32>(0,101,220,130),true,true,pyramidImage,-1,false);
	st->setOverrideColor(SCOL_BLUE);
	pyramidImage->addChild(st);
	pyramidImage->setToolTipText(L"To Add Pyramid Object CLICK!");
	pyramidImage->setImage(driver->getTexture("../media/pyramid.png"));

	rectangleImage = guienv->addImage(l.RECTANGLE_IMAGE_LOCATION, rightTab, RECTANGLE_IMAGE_ID);
	st = guienv->addStaticText(L"Rectangle Object",rect<s32>(0,101,220,130),true,true,rectangleImage,-1,false);
	st->setOverrideColor(SCOL_BLUE);
	rectangleImage->addChild(st);
	rectangleImage->setToolTipText(L"To Add Rectangle Object CLICK!");
	rectangleImage->setImage(driver->getTexture("../media/rectangle.png"));

	squareImage = guienv->addImage(l.SQUARE_IMAGE_LOCATION, rightTab, SQUARE_IMAGE_ID);
	st = guienv->addStaticText(L"Square Object",rect<s32>(0,101,220,130),true,true,squareImage,-1,false);
	st->setOverrideColor(SCOL_BLUE);
	squareImage->addChild(st);
	squareImage->setToolTipText(L"To Add Square Object CLICK!");
	squareImage->setImage(driver->getTexture("../media/square.png"));
	/***********************/
}

Interface::Interface(Interface* intfaceV){
	guienv = intfaceV->guienv;
	skin = intfaceV->skin;
	font = intfaceV->font;

	staticText = intfaceV->staticText;

	rightTab = intfaceV->rightTab;
	menuTab = intfaceV->menuTab;
	itemsTab = intfaceV->itemsTab;

	RotateButton = intfaceV->RotateButton;
	MoveButton = intfaceV->MoveButton;
	ScaleButton = intfaceV->ScaleButton;
	ChangeCameraTypeButton = intfaceV->ChangeCameraTypeButton;
	ChangeModeButton = intfaceV->ChangeModeButton;
	AddObjectButton = intfaceV->AddObjectButton;
	ExitButton = intfaceV->ExitButton;

	coneImage = intfaceV->coneImage;
	cubeImage = intfaceV->cubeImage;
	cylinderImage = intfaceV->cylinderImage;
	pyramidImage = intfaceV->pyramidImage;
	rectangleImage = intfaceV->rectangleImage;
	squareImage = intfaceV->squareImage;

	ImageScrollbar = intfaceV->ImageScrollbar;
	ItemsScrollbar = intfaceV->ItemsScrollbar;
}
