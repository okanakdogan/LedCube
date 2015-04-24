#ifndef INTERFACE_H
#define INTERFACE_H
#include"irrlicht.h"

using namespace irr;
using namespace core;
using namespace scene;
using namespace video;
using namespace io;
using namespace gui;

#define FULLSCREEN dimension2d<u32>(1366, 768)

#define SCOL_BLACK     SColor(255, 0,   0,   0)
#define SCOL_DARKBLUE  SColor(255, 0,   0,  139)
#define SCOL_BLUE      SColor(255, 0,   0,  255)
#define SCOL_CYAN      SColor(255, 0,  255, 255)
#define SCOL_DARKGRAY  SColor(255, 64,  64, 64)
#define SCOL_GRAY      SColor(255, 128,128, 128)
#define SCOL_DARKGREEN SColor(255, 0,  100,  0)
#define SCOL_GREEN     SColor(255, 0,  255,  0)
#define SCOL_LIGHTGRAY SColor(255, 192, 192,192 )
#define SCOL_MAGENTA   SColor(255, 255, 0,  255)
#define SCOL_ORANGE    SColor(255, 255, 192,128)
#define SCOL_PINK      SColor(255, 255, 176,176)
#define SCOL_DARKRED   SColor(255, 139, 0,   0)
#define SCOL_RED       SColor(255, 255, 0,   0)
#define SCOL_YELLOW    SColor(255, 255, 255, 0)
#define SCOL_WHITE     SColor(255, 255, 255, 255)
#define SCOL_BROWN     SColor(255, 165, 42, 42)
#define SCOL_GOLD      SColor(255, 255, 215, 0)

class Interface{
public:
	Interface(IrrlichtDevice *device, IVideoDriver* driver);
private:
	IGUIEnvironment* guienv;
	IGUISkin* skin;
	IGUIFont* font;
	
	IGUIStaticText *staticText;
	
	IGUITab * rightTab;
	IGUITab * menuTab;
	IGUITab * itemsTab;
	
	IGUIListBox * listbox;

	IGUIButton *RotateButton;
	IGUIButton *MoveButton;
	IGUIButton *ScaleButton;
	IGUIButton *ChangeCameraTypeButton;
	IGUIButton *ChangeModeButton;
	IGUIButton *AddObjectButton;
	IGUIButton *ExitButton;
};

#endif