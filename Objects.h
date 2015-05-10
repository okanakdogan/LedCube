#ifndef OBJECTS_H
#define OBJECTS_H
#include"irrlicht.h"

using namespace irr;
using namespace core;
using namespace scene;
using namespace video;
using namespace io;
using namespace gui;

class Object{
public:
	Object(IrrlichtDevice *deviceV);
	Object(Object *other, IrrlichtDevice *deviceV);
	Object(IAnimatedMesh* meshV, IAnimatedMeshSceneNode* nodeV, IrrlichtDevice *deviceV);

	bool addAnObject(const wchar_t *filename);
	void removeObject();
	bool isPointInside(vector3df point);
	
	static int groupIdCount;
	IAnimatedMesh* mesh;
	IAnimatedMeshSceneNode* node;
	ITriangleSelector* triangleSelector;
	bool isSelected;
	int groupId;
	IrrlichtDevice *device;
};

#endif