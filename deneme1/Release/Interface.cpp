#include"Interface.h"

Interface::Interface(IrrlichtDevice *device, IVideoDriver* driver){
	guienv = device->getGUIEnvironment();
	IGUISkin* skin = guienv->getSkin();
	IGUIFont* font = guienv->getFont("../media/font/font.xml");
	skin->setFont(font);
	skin->setFont(guienv->getBuiltInFont(), EGDF_TOOLTIP);

	guienv->addStaticText(L"THIS IS THE DRAWING SHAPE AREA",
		rect<int>(0,0,470,50), true)->setOverrideColor(SCOL_GREEN);

	IGUITabControl * rightTabControl= guienv->addTabControl(rect<s32>(FULLSCREEN.Width*0.80,0,FULLSCREEN.Width,FULLSCREEN.Height),0,true,true,102);
    IGUITab * rightTab=rightTabControl->addTab(L"PRIMITIVES",-1);
	rightTab->setTextColor(SCOL_GREEN);

    IGUITabControl * menuTabControl= guienv->addTabControl(rect<s32>(250,FULLSCREEN.Height*0.70,FULLSCREEN.Width*0.80,FULLSCREEN.Height),0,true,true,103);
    IGUITab * menuTab=menuTabControl->addTab(L"MENU",-1);
	menuTab->setTextColor(SCOL_GREEN);
	
	IGUITabControl * itemsTabControl= guienv->addTabControl(rect<s32>(0,FULLSCREEN.Height*0.70,FULLSCREEN.Width*0.184,FULLSCREEN.Height),0,true,true,103);
    IGUITab * itemsTab=itemsTabControl->addTab(L"ITEMS",-1);
	itemsTab->setTextColor(SCOL_GREEN);

	IGUIListBox * listbox = guienv->addListBox(rect<s32>(20, 20, 200, 150));
	listbox->addItem(L"ITEM 1");
	listbox->addItem(L"ITEM 2");
	listbox->addItem(L"ITEM 3");
	itemsTab->addChild(listbox);

	for (s32 i=0; i<irr::gui::EGDC_COUNT ; ++i)
    {
        SColor col = guienv->getSkin()->getColor((EGUI_DEFAULT_COLOR)i);
        col.setAlpha(255);
        guienv->getSkin()->setColor((EGUI_DEFAULT_COLOR)i, col);
    }

	IGUIButton *RotateButton = guienv->addButton(rect<s32>(0,0,100,50),menuTab,-1,L"Rotate");
	IGUIButton *MoveButton = guienv->addButton(rect<s32>(100,0,200,50),menuTab,-1,L"Move");
	IGUIButton *ScaleButton = guienv->addButton(rect<s32>(200,0,300,50),menuTab,-1,L"Scale");
	IGUIButton *ChangeCameraTypeButton = guienv->addButton(rect<s32>(0,100,300,150),menuTab,-1,L"Change Camera Type");
	IGUIButton *ChangeModeButton = guienv->addButton(rect<s32>(300,100,480,150),menuTab,-1,L"Change Mode");
	IGUIButton *AddObjectButton = guienv->addButton(rect<s32>(690,0,850,50),menuTab,-1,L"Add Object");
	IGUIButton *ExitButton = guienv->addButton(rect<s32>(690,100,780,150),menuTab,-1,L"Exit");

	IGUIImage* coneImage = guienv->addImage(recti(50, 50, 150, 150), rightTab, rightTab->getID());
	coneImage->setScaleImage(true);
	coneImage->setImage(driver->getTexture("../media/cone.png"));

	IGUIImage* cubeImage = guienv->addImage(recti(50, 150, 150, 250), rightTab, rightTab->getID());
	cubeImage->setScaleImage(true);
	cubeImage->setImage(driver->getTexture("../media/cube.png"));

	IGUIImage* squareImage = guienv->addImage(recti(50, 250, 150, 350), rightTab, rightTab->getID());
	squareImage->setScaleImage(true);
	squareImage->setImage(driver->getTexture("../media/square.png"));
}