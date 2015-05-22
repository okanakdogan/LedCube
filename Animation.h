#ifndef ANIMATION_H
#define ANIMATION_H

#include "irrlicht.h"
#include<iostream>

using namespace irr;
using namespace core;
using namespace scene;
using namespace video;
using namespace io;
using namespace gui;
using namespace std;

class Animation{
public:

	Animation();

	void update();
	vector3df animationVector();

	void GITGELX();
	void GITGELY();
	void GITGELZ();
	void GITGELXYZ();
	void SQUARE();
	void CIRCUIT();
	void TOPSEKME();

	vector3df animationSpeed;
	int animationType;
	bool animationMode;
	int x_type;
	int y_type;
	int z_type;
	int counter;
};

#endif