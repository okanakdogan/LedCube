#ifndef NODE_H
#define NODE_H

#include"irrlicht.h"

using namespace irr;
using namespace core;
using namespace scene;
using namespace video;
using namespace io;
using namespace gui;


 class Node
	{
	public :
		int isSelected;
		ISceneNode * node ;
		Node(){isSelected = false;}
		Node(ISceneNode * node_)
		{
			isSelected = false; 
			node = node_ ;
		}
	};

#endif