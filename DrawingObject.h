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
	DrawingObject(vector<Node> node_){
		isSelected = false;
		for(int i=0; i<node_.size(); i++)
			node.push_back(node_[i]);
	}
	void select(){
		for(int i=0; i<node.size(); i++){
			node[i].select();
		}
		isSelected = true;
	}
	void unSelect(){
		for(u32 i=0; i<node.size(); i++){
			node[i].unSelect();
		}
		isSelected = false;
	}
	bool isPointInside(vector3df point){
		for(u32 i=0; i<node.size(); i++){
			if(node[i].node->getTransformedBoundingBox().isPointInside(point))
				return true;
		}
		return false;
	}
	bool isCollisionDetach(line3df raytrace, vector3df *outCollisionPoint, triangle3df *outTriangle, ISceneNode *outNode,
							ISceneCollisionManager *colmgr){
		for(u32 i=0; i<node.size(); i++)
			if(colmgr->getCollisionPoint(raytrace, node[i].triangleSelector, *outCollisionPoint, *outTriangle, outNode))
				return true;
		return false;
	}
	void setPosition(vector3df point){
		vector3df commonPoint = getAbsolutePosition();
		vector3df difVec = point-commonPoint;
		for(u32 i=0; i<node.size(); i++){
			vector3df oldPosition = node[i].node->getAbsolutePosition();
			vector3df newPosition = oldPosition+difVec;
			node[i].node->setPosition(newPosition);
		}
	}
	vector3df getAbsolutePosition(){
		vector3df commonPoint = vector3df(0,0,0);
		for(int i=0; i<node.size(); i++){
			commonPoint+=node[i].node->getAbsolutePosition();
		}
		commonPoint/=node.size();
		return commonPoint;
	}
	void setScale(vector3df newScale){
		for(int i=0; i<node.size(); i++){
			node[i].node->setScale(newScale);
		}
	}
	void setVisible(bool isVisible){
		for(int i=0; i<node.size(); i++)
			node[i].node->setVisible(isVisible);
	}
	void remove(){
		for(int i=0; i<node.size(); i++)
			node[i].node->remove();
	}
	vector<Node> node;
	int isSelected ;
};

#endif
