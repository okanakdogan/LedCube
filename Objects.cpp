#include"Objects.h"
#include <iostream>
using namespace std ;

int Object::groupIdCount = 0;

Object::Object(IrrlichtDevice *deviceV) : device(deviceV){
	mesh = NULL;
	node = NULL;
	isSelected = false;
	triangleSelector = NULL;
	groupId = -1;
	anim = new Animation();
}

Object::Object(Object *other, IrrlichtDevice *deviceV) : device(deviceV){
	mesh = other->mesh;
	node = other->node;
	isSelected = false;
	triangleSelector = device->getSceneManager()->createTriangleSelector(node);
	node->setTriangleSelector(triangleSelector);
	groupId = -1;
	anim = new Animation();
}

Object::Object(IAnimatedMesh* meshV, IAnimatedMeshSceneNode* nodeV, IrrlichtDevice *deviceV) : device(deviceV), mesh(meshV), node(nodeV){
	isSelected = false;
	triangleSelector = device->getSceneManager()->createTriangleSelector(node);
	node->setTriangleSelector(triangleSelector);
	groupId = -1;
	anim = new Animation();
}

bool Object::addAnObject(const wchar_t *filename){
	mesh = device->getSceneManager()->getMesh(filename);
	if(mesh){
		node = device->getSceneManager()->addAnimatedMeshSceneNode(mesh);
		if(node){
			triangleSelector = device->getSceneManager()->createTriangleSelector(node);
			if(triangleSelector)
				node->setTriangleSelector(triangleSelector);
			triangleSelector->drop();
			//node->setDebugDataVisible(1);
			return true;
		}else{
			return false;
		}
	}else{
		return false;
	}
	return true;
}

void Object::removeObject(){
	node->remove();
}

// aabbox3d isPointInside kullanýlacak optimizasyon için
bool Object::isPointInside(vector3df point)
{
	node->updateAbsolutePosition();
	aabbox3df box = aabbox3df(node->getTransformedBoundingBox().MinEdge-1,node->getTransformedBoundingBox().MaxEdge+1);
	if(!box.isPointInside(point))
		return false;

	line3df ray = line3df(box.MinEdge,point);
	bool isCollision =true ;
	ISceneManager* smgr = device->getSceneManager();
	ISceneCollisionManager *colmgr = smgr->getSceneCollisionManager();
	ISceneNode *outNode;
	vector3df outCollisionPoint = vector3df();
    triangle3df outTriangle = triangle3df();
	vector3df edges[8];

	box.getEdges(edges);

	for(int i=0;i<8;++i){
        ray.setLine(edges[i],point);
		bool isCol = colmgr->getCollisionPoint(ray, triangleSelector, outCollisionPoint, outTriangle, outNode);
		if(!isCol)
			return false;
        isCollision=isCollision&&isCol;
    }
	return isCollision;
}