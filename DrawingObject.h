#ifndef DRAWING_OBJECT_H
#define DRAWING_OBJECT_H

#include"irrlicht.h"
#include"Node.h"

using namespace irr;
using namespace core;
using namespace scene;
using namespace video;
using namespace io;
using namespace gui;

class DrawingObject{

public :
	DrawingObject(){
		isSelected = false;
	}
	vector<Node> node;
	int isSelected ;

	
	



};

#endif