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
}

Object::Object(Object *other, IrrlichtDevice *deviceV) : device(deviceV){
	mesh = other->mesh;
	node = other->node;
	isSelected = false;
	triangleSelector = device->getSceneManager()->createTriangleSelector(node);
	node->setTriangleSelector(triangleSelector);
	groupId = -1;
}

Object::Object(IAnimatedMesh* meshV, IAnimatedMeshSceneNode* nodeV, IrrlichtDevice *deviceV) : device(deviceV), mesh(meshV), node(nodeV){
	isSelected = false;
	triangleSelector = device->getSceneManager()->createTriangleSelector(node);
	node->setTriangleSelector(triangleSelector);
	groupId = -1;
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
			//node->setDebugDataVisible( irr::scene::E_DEBUG_SCENE_TYPE::EDS_BBOX);

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
	aabbox3df box = node->getTransformedBoundingBox();
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
	/*aabbox3d<f32> box = node->getBoundingBox();
	line3df ray = line3df(box.MinEdge,point);
	//cout<<box.getCenter().X<<endl;
	bool isCollision =true ;
	ISceneManager* smgr = device->getSceneManager();

	ISceneCollisionManager *colmgr = smgr->getSceneCollisionManager();
	ISceneNode *outNode;
	vector3df outCollisionPoint = vector3df();
    triangle3df outTriangle = triangle3df();

	vector3df edges[8];

    box.getEdges(edges);
    box.MinEdge+=vector3df(-1.0f,-1.0f,-1.0f);
    box.MaxEdge+=vector3df(1.0f,1.0f,1.0f);
	edges[0]=vector3df(box.MinEdge.X,box.MinEdge.Y,box.MinEdge.Z);

	edges[1]=vector3df(box.MinEdge.X,box.MinEdge.Y,box.MaxEdge.Z);
	edges[2]=vector3df(box.MinEdge.X,box.MaxEdge.Y,box.MinEdge.Z);
	edges[3]=vector3df(box.MinEdge.X,box.MaxEdge.Y,box.MaxEdge.Z);
	edges[4]=vector3df(box.MaxEdge.X,box.MinEdge.Y,box.MinEdge.Z);
	edges[5]=vector3df(box.MaxEdge.X,box.MinEdge.Y,box.MaxEdge.Z);
	edges[6]=vector3df(box.MaxEdge.X,box.MaxEdge.Y,box.MinEdge.Z);
	edges[7]=vector3df(box.MaxEdge.X,box.MaxEdge.Y,box.MaxEdge.Z);
	node->setDebugDataVisible( scene::E_DEBUG_SCENE_TYPE::EDS_BBOX);
	//cout<<edges[0].X<<" "<<edges[0].Y<<endl;
	for(int i=0;i<8;++i){
        edges[i]+=node->getPosition();
        ray.setLine(edges[i],point);
		bool isCol = colmgr->getCollisionPoint(ray, triangleSelector, outCollisionPoint, outTriangle, outNode);
		if(!isCol)
			return false;
        isCollision=isCollision&&isCol;
    }
	return isCollision;*/
}
