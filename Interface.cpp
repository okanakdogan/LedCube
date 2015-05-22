#include"Interface.h"

Interface::Interface(IrrlichtDevice *device, IVideoDriver* driver){
	/* creating locations */
	int value =130;
	l.CONE_IMAGE_LOCATION = recti(30, 0, 250, value);
	l.CUBE_IMAGE_LOCATION = recti(30, value, 250, value+130);
	l.CYLINDER_IMAGE_LOCATION = recti(30, value+130, 250, value+260);
	l.PYRAMID_IMAGE_LOCATION = recti(30, value+260, 250, value+390);
	l.RECTANGLE_IMAGE_LOCATION = recti(30, value+390, 250, value+520);
	l.SQUARE_IMAGE_LOCATION = recti(30, value+520, 250, value+650);
	l.ARROW_IMAGE_LOCATION = recti(30, value + 650, 250, value + 780);
	l.DUMBLE_IMAGE_LOCATION = recti(30, value + 780, 250, value + 910);
	l.MAN_IMAGE_LOCATION = recti(30, value + 910, 250, value + 1040);
	l.CIRCLE_IMAGE_LOCATION = recti(30, value+1040, 250, value+1170);
	l.KALP_IMAGE_LOCATION = recti(30, value + 1170, 250, value + 1300);
	l.KUPA_IMAGE_LOCATION = recti(30, value + 1300, 250, value + 1430);
	l.KURT_IMAGE_LOCATION = recti(30, value + 1430, 250, value + 1560);

	l.ROTATE_BUTTON_LOCATION = rect<s32>(0,0,100,50);
	l.MOVE_BUTTON_LOCATION = rect<s32>(100,0,200,50);
	l.SCALE_BUTTON_LOCATION = rect<s32>(200,0,300,50);
	l.DRAW_BUTTON_LOCATION = rect<s32>(300,0,400,50);
	l.CHOOSE_BUTTON_LOCATION = rect<s32>(400,0,500,50);
	l.HELP_BUTTON_LOCATION = rect<s32>(500,0,600,50);
	l.ADD_OBJECT_BUTTON_LOCATION = rect<s32>(690,0,850,50);
	l.EXIT_BUTTON_LOCATION = rect<s32>(690,100,780,150);

	l.IMAGE_SCROLL_BAR_LOCATION = rect<s32>(250, 0, 270, 700);
	/**********************/

	guienv = device->getGUIEnvironment();
	skin = guienv->getSkin();

	font = guienv->getFont("media/font/myfont.xml");

	skin->setFont(font);
	skin->setFont(guienv->getBuiltInFont(), EGDF_TOOLTIP);

	guienv->addStaticText(L"THIS IS THE DRAWING SHAPE AREA",
		rect<int>(0,0,470,30), true)->setOverrideColor(SCOL_GREEN);

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

	Listbox = guienv->addListBox(rect<s32>(0,FULLSCREEN.Height*0.75,FULLSCREEN.Width*0.174,FULLSCREEN.Height),0,ITEMS_LIST_BOX_ID,false);

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
	ChooseButton = guienv->addButton(l.CHOOSE_BUTTON_LOCATION,menuTab,CHOOSE_BUTTON_ID,L"Choose",L"Choose The Selected Object");
	DrawButton = guienv->addButton(l.DRAW_BUTTON_LOCATION,menuTab,DRAW_BUTTON_ID,L"Draw",L"Draw line");
	AddObjectButton = guienv->addButton(l.ADD_OBJECT_BUTTON_LOCATION,menuTab,ADD_OBJECT_BUTTON_ID,L"Add Object",L"Add A Specific Object");
	ExitButton = guienv->addButton(l.EXIT_BUTTON_LOCATION,menuTab,EXIT_BUTTON_ID,L"Exit",L"Exit From Program");

	HelpButton = guienv->addButton(l.HELP_BUTTON_LOCATION,menuTab,HELP_BUTTON_ID,L"Help",L"Learn More About Us");
	/*******************/

	/* CREATING IMAGES */
	coneImage = guienv->addImage(l.CONE_IMAGE_LOCATION, rightTab, CONE_IMAGE_ID);
	IGUIStaticText *st = guienv->addStaticText(L"Cone Object",rect<s32>(0,101,220,130),true,true,coneImage,-1,false);
	st->setOverrideColor(SCOL_BLUE);
	coneImage->addChild(st);
	coneImage->setToolTipText(L"To Add Cone Object CLICK!");


	coneImage->setImage(driver->getTexture("media/cone.png"));


	cubeImage = guienv->addImage(l.CUBE_IMAGE_LOCATION, rightTab, CUBE_IMAGE_ID);
	st = guienv->addStaticText(L"Cube Object",rect<s32>(0,101,220,130),true,true,cubeImage,-1,false);
	st->setOverrideColor(SCOL_BLUE);
	cubeImage->addChild(st);
	cubeImage->setToolTipText(L"To Add Cube Object CLICK!");


	cubeImage->setImage(driver->getTexture("media/cube.png"));


	cylinderImage = guienv->addImage(l.CYLINDER_IMAGE_LOCATION, rightTab, CYLINDER_IMAGE_ID);
	st = guienv->addStaticText(L"Cylinder Object",rect<s32>(0,101,220,130),true,true,cylinderImage,-1,false);
	st->setOverrideColor(SCOL_BLUE);
	cylinderImage->addChild(st);
	cylinderImage->setToolTipText(L"To Add Cylinder Object CLICK!");


	cylinderImage->setImage(driver->getTexture("media/cylinder.png"));


	pyramidImage = guienv->addImage(l.PYRAMID_IMAGE_LOCATION, rightTab, PYRAMID_IMAGE_ID);
	st = guienv->addStaticText(L"Pyramid Object",rect<s32>(0,101,220,130),true,true,pyramidImage,-1,false);
	st->setOverrideColor(SCOL_BLUE);
	pyramidImage->addChild(st);
	pyramidImage->setToolTipText(L"To Add Pyramid Object CLICK!");


	pyramidImage->setImage(driver->getTexture("media/pyramid.png"));


	rectangleImage = guienv->addImage(l.RECTANGLE_IMAGE_LOCATION, rightTab, RECTANGLE_IMAGE_ID);
	st = guienv->addStaticText(L"Rectangle Object",rect<s32>(0,101,220,130),true,true,rectangleImage,-1,false);
	st->setOverrideColor(SCOL_BLUE);
	rectangleImage->addChild(st);
	rectangleImage->setToolTipText(L"To Add Rectangle Object CLICK!");


	rectangleImage->setImage(driver->getTexture("media/rectangle.png"));


	squareImage = guienv->addImage(l.SQUARE_IMAGE_LOCATION, rightTab, SQUARE_IMAGE_ID);
	st = guienv->addStaticText(L"Square Object",rect<s32>(0,101,220,130),true,true,squareImage,-1,false);
	st->setOverrideColor(SCOL_BLUE);
	squareImage->addChild(st);
	squareImage->setToolTipText(L"To Add Square Object CLICK!");


	squareImage->setImage(driver->getTexture("media/square.png"));

	//*********************************************************************************************************************
	arrowImage = guienv->addImage(l.ARROW_IMAGE_LOCATION, rightTab, ARROW_IMAGE_ID);
	st = guienv->addStaticText(L"Arrow Object", rect<s32>(0, 101, 220, 130), true, true, arrowImage, -1, false);
	st->setOverrideColor(SCOL_BLUE);
	arrowImage->addChild(st);
	arrowImage->setToolTipText(L"To Add Arrow Object CLICK!");


	arrowImage->setImage(driver->getTexture("media/cizgili_ok.png"));

	//*********************************************************************************************************************
	dumbleImage = guienv->addImage(l.DUMBLE_IMAGE_LOCATION, rightTab, DUMBLE_IMAGE_ID);
	st = guienv->addStaticText(L"Dumble Object", rect<s32>(0, 101, 220, 130), true, true, dumbleImage, -1, false);
	st->setOverrideColor(SCOL_BLUE);
	dumbleImage->addChild(st);
	dumbleImage->setToolTipText(L"To Add Dumble Object CLICK!");


	dumbleImage->setImage(driver->getTexture("media/dumbell.png"));

	//*********************************************************************************************************************
	circleImage = guienv->addImage(l.CIRCLE_IMAGE_LOCATION, rightTab, CIRCLE_IMAGE_ID);
	st = guienv->addStaticText(L"Circle Object", rect<s32>(0, 101, 220, 130), true, true, circleImage, -1, false);
	st->setOverrideColor(SCOL_BLUE);
	circleImage->addChild(st);
	circleImage->setToolTipText(L"To Add Circle Object CLICK!");


	circleImage->setImage(driver->getTexture("media/circle.png"));


	//*********************************************************************************************************************
	manImage = guienv->addImage(l.MAN_IMAGE_LOCATION, rightTab, MAN_IMAGE_ID);
	st = guienv->addStaticText(L"Man Object", rect<s32>(0, 101, 220, 130), true, true, manImage, -1, false);
	st->setOverrideColor(SCOL_BLUE);
	manImage->addChild(st);
	manImage->setToolTipText(L"To Add Man Object CLICK!");


	manImage->setImage(driver->getTexture("media/man.png"));


	//*********************************************************************************************************************
	kalpImage = guienv->addImage(l.KALP_IMAGE_LOCATION, rightTab, KALP_IMAGE_ID);
	st = guienv->addStaticText(L"Kalp Object", rect<s32>(0, 101, 220, 130), true, true, kalpImage, -1, false);
	st->setOverrideColor(SCOL_BLUE);
	kalpImage->addChild(st);
	kalpImage->setToolTipText(L"To Add kalp Object CLICK!");


	kalpImage->setImage(driver->getTexture("media/kalp.png"));


	//*********************************************************************************************************************
	kupaImage = guienv->addImage(l.KUPA_IMAGE_LOCATION, rightTab, KUPA_IMAGE_ID);
	st = guienv->addStaticText(L"Kupa Object", rect<s32>(0, 101, 220, 130), true, true, kupaImage, -1, false);
	st->setOverrideColor(SCOL_BLUE);
	kupaImage->addChild(st);
	kupaImage->setToolTipText(L"To Add kupa Object CLICK!");


	kupaImage->setImage(driver->getTexture("media/kupa.png"));


	//*********************************************************************************************************************
	kurtImage = guienv->addImage(l.KURT_IMAGE_LOCATION, rightTab, KURT_IMAGE_ID);
	st = guienv->addStaticText(L"Kurt Object", rect<s32>(0, 101, 220, 130), true, true, kurtImage, -1, false);
	st->setOverrideColor(SCOL_BLUE);
	kurtImage->addChild(st);
	kurtImage->setToolTipText(L"To Add kurt Object CLICK!");


	kurtImage->setImage(driver->getTexture("media/kurt.png"));


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
	DrawButton = intfaceV->DrawButton;
	ChooseButton = intfaceV->ChooseButton;
	AddObjectButton = intfaceV->AddObjectButton;
	ExitButton = intfaceV->ExitButton;

	coneImage = intfaceV->coneImage;
	cubeImage = intfaceV->cubeImage;
	cylinderImage = intfaceV->cylinderImage;
	pyramidImage = intfaceV->pyramidImage;
	rectangleImage = intfaceV->rectangleImage;
	squareImage = intfaceV->squareImage;
	manImage = intfaceV->manImage;
	dumbleImage = intfaceV->dumbleImage;
	circleImage = intfaceV->circleImage;
	arrowImage = intfaceV->arrowImage;
	kalpImage = intfaceV->kalpImage;
	kupaImage = intfaceV->kupaImage;
	kurtImage = intfaceV->kurtImage;

	ImageScrollbar = intfaceV->ImageScrollbar;
	Listbox = intfaceV->Listbox;
}
