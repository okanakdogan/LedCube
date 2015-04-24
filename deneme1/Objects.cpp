#include"Objects.h"

Object::Object(IrrlichtDevice *deviceV) : device(deviceV){
	mesh = NULL;
	node = NULL;
	isSelected = false;
	triangleSelector = NULL;
}

Object::Object(Object *other, IrrlichtDevice *deviceV) : device(deviceV){
	mesh = other->mesh;
	node = other->node;
	isSelected = false;
	triangleSelector = device->getSceneManager()->createTriangleSelector(node);
	node->setTriangleSelector(triangleSelector);
}

Object::Object(IAnimatedMesh* meshV, IAnimatedMeshSceneNode* nodeV, IrrlichtDevice *deviceV) : device(deviceV), mesh(meshV), node(nodeV){
	isSelected = false;
	triangleSelector = device->getSceneManager()->createTriangleSelector(node);
	node->setTriangleSelector(triangleSelector);
}

bool Object::addAnObject(const wchar_t *filename){
	mesh = device->getSceneManager()->getMesh(filename);
	if(mesh){
		node = device->getSceneManager()->addAnimatedMeshSceneNode(mesh);
		if(node){
			triangleSelector = device->getSceneManager()->createTriangleSelector(node);
			node->setTriangleSelector(triangleSelector);

			/*node scale --okan*/
			/*
			node->setScale(core::vector3df(0.1f,0.1f,0.1f));
            node->setPosition(core::vector3df(25,30,-10));
            */

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
