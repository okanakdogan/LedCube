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

struct Locations{
	recti CONE_IMAGE_LOCATION;
	recti CUBE_IMAGE_LOCATION;
	recti CYLINDER_IMAGE_LOCATION;
	recti PYRAMID_IMAGE_LOCATION;
	recti RECTANGLE_IMAGE_LOCATION;
	recti SQUARE_IMAGE_LOCATION;

	recti ROTATE_BUTTON_LOCATION;
	recti MOVE_BUTTON_LOCATION;
	recti SCALE_BUTTON_LOCATION;
	recti CHOOSE_BUTTON_LOCATION;
	recti DRAW_BUTTON_LOCATION;
	recti ADD_OBJECT_BUTTON_LOCATION;
	recti EXIT_BUTTON_LOCATION;
	recti DUMBLE_IMAGE_LOCATION ;
	recti MAN_IMAGE_LOCATION ;
	recti ARROW_IMAGE_LOCATION ;
	recti CIRCLE_IMAGE_LOCATION ;
	recti KALP_IMAGE_LOCATION;
	recti KUPA_IMAGE_LOCATION;
	recti KURT_IMAGE_LOCATION;

	recti HELP_BUTTON_LOCATION;

	recti IMAGE_SCROLL_BAR_LOCATION;
};

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

enum
{
	ROTATE_BUTTON_ID = 101,
	MOVE_BUTTON_ID, 
	SCALE_BUTTON_ID,
	ADD_OBJECT_BUTTON_ID,
	EXIT_BUTTON_ID,
	HELP_BUTTON_ID,
	CONE_IMAGE_ID,
	CUBE_IMAGE_ID,
	CYLINDER_IMAGE_ID,
	PYRAMID_IMAGE_ID,
	RECTANGLE_IMAGE_ID,
	SQUARE_IMAGE_ID,
	DUMBLE_IMAGE_ID,
	MAN_IMAGE_ID,
	ARROW_IMAGE_ID,
	CIRCLE_IMAGE_ID,
	KALP_IMAGE_ID,
	KUPA_IMAGE_ID,
	KURT_IMAGE_ID,
	IMAGE_SCROLL_BAR_ID,
	ITEMS_LIST_BOX_ID,
	DRAW_BUTTON_ID,
	CHOOSE_BUTTON_ID

};

class Interface{
public:
	Interface(IrrlichtDevice *device, IVideoDriver* driver);
	Interface(Interface* intfaceV);

	IGUIEnvironment* guienv;
	IGUISkin* skin;
	IGUIFont* font;
	
	IGUIStaticText *staticText;
	
	IGUITab * rightTab;
	IGUITab * menuTab;
	IGUITab * itemsTab;

	IGUIButton *RotateButton;
	IGUIButton *MoveButton;
	IGUIButton *ScaleButton;
	IGUIButton *DrawButton;
	IGUIButton *ChooseButton;
	IGUIButton *AddObjectButton;
	IGUIButton *ExitButton;
	IGUIButton *HelpButton;

	IGUIImage* coneImage;
	IGUIImage* cubeImage;
	IGUIImage* cylinderImage;
	IGUIImage* pyramidImage;
	IGUIImage* rectangleImage;
	IGUIImage* squareImage;
	IGUIImage* manImage ;
	IGUIImage* circleImage;
	IGUIImage* arrowImage;
	IGUIImage* dumbleImage;
	IGUIImage* kalpImage;
	IGUIImage* kupaImage;
	IGUIImage* kurtImage;

	IGUIScrollBar *ImageScrollbar;
	IGUIListBox    *Listbox;

	Locations l;
};

#endif