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
		ITriangleSelector* triangleSelector;
		Node(){
			isSelected = false;
			triangleSelector = NULL;
		}
		Node(ISceneNode * node_, IrrlichtDevice *device_)
		{
			isSelected = false; 
			node = node_ ;
			triangleSelector = device_->getSceneManager()->createTriangleSelectorFromBoundingBox(node);
			node->setTriangleSelector(triangleSelector);
		}
		void select(){
			isSelected = true;
			for(int i=0; i<node->getMaterialCount(); i++)
				node->getMaterial(i).EmissiveColor.set(255, 255, 255, 0);
		}
		void unSelect(){
			isSelected = false;
			for(int i=0; i<node->getMaterialCount(); i++)
				node->getMaterial(i).EmissiveColor.set(255, 0,   0,   0);
		}
	};

#endif