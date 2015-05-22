#include"Animation.h"

Animation::Animation(){
	animationSpeed = vector3df(0,0,0);
	animationMode = false;
	x_type = 1;
	y_type = 1;
	z_type = 1;
	counter = 400;
	animationType = 0;
}

void Animation::update(){
	switch(animationType){
		case 0:
			animationSpeed = vector3df(0,0,0);
			break;
		case 1:
			GITGELX();
			break;
		case 2:
			GITGELY();
			break;
		case 3:
			GITGELZ();
			break;
		case 4:
			GITGELXYZ();
			break;
		case 5:
			SQUARE();
			break;
		case 6:
			CIRCUIT();
			break;
		case 7:
			TOPSEKME();
			break;
	}
}

vector3df Animation::animationVector(){
	float x = animationSpeed.X*x_type;
	float y = animationSpeed.Y*y_type;
	float z = animationSpeed.Z*z_type;
	return vector3df(x,y,z);
}

void Animation::GITGELX(){
	if(animationSpeed != vector3df(1,0,0))
		animationSpeed = vector3df(1,0,0);
	if(counter == 0){
		counter = 400;
		x_type *= -1;
	}
	counter--;
}
void Animation::GITGELY(){
	if(animationSpeed != vector3df(0,1,0))
		animationSpeed = vector3df(0,1,0);
	if(counter == 0){
		counter = 400;
		y_type *= -1;
	}
	counter--;
}
void Animation::GITGELZ(){
	if(animationSpeed != vector3df(0,0,1))
		animationSpeed = vector3df(0,0,1);
	if(counter == 0){
		counter = 400;
		z_type *= -1;
	}
	counter--;
}
void Animation::GITGELXYZ(){
	if(animationSpeed != vector3df(1,1,1))
		animationSpeed = vector3df(1,1,1);
	if(counter == 0){
		counter = 400;
		x_type *= -1;
		y_type *= -1;
		z_type *= -1;
	}
	counter--;
}

void Animation::SQUARE(){
	if(counter < 100){
		if(animationSpeed != vector3df(5,0,0))
			animationSpeed = vector3df(5,0,0);
	}else if(counter < 200){
		if(animationSpeed != vector3df(0,5,0))
			animationSpeed = vector3df(0,5,0);
	}else if(counter < 300){
		if(animationSpeed != vector3df(-5,0,0))
			animationSpeed = vector3df(-5,0,0);
	}else{
		if(animationSpeed != vector3df(0,-5,0))
			animationSpeed = vector3df(0,-5,0);
	}
	if(counter == 0){
		counter = 400;
	}
	counter--;
}

void Animation::CIRCUIT(){
	if(counter == 0)
		counter = 400;
	if(counter == 100)
		animationSpeed = vector3df(0,-2.4,0);
	else if(counter == 200)
		animationSpeed = vector3df(2.4,0,0);
	else if(counter == 300)
		animationSpeed = vector3df(0,2.4,0);
	else if(counter == 400)
		animationSpeed = vector3df(-2.4,0,0);
	else if(counter < 100)
		animationSpeed += vector3df(-0.025,0.025,0);
	else if(counter < 200)
		animationSpeed += vector3df(-0.025,-0.025,0);
	else if(counter < 300)
		animationSpeed += vector3df(0.025,-0.025,0);
	else if(counter < 400)
		animationSpeed += vector3df(0.025,0.025,0);
	counter--;
}

void Animation::TOPSEKME(){
	if(counter == 0)
		counter = 400;
	if(counter == 25)
		animationSpeed = vector3df(4.8,0,0);
	else if(counter == 50)
		animationSpeed = vector3df(0,4.8,0);
	else if(counter == 75)
		animationSpeed = vector3df(4.8,0,0);
	else if(counter == 100)
		animationSpeed = vector3df(0,4.8,0);
	else if(counter == 125)
		animationSpeed = vector3df(4.8,0,0);
	else if(counter == 150)
		animationSpeed = vector3df(0,4.8,0);
	else if(counter == 175)
		animationSpeed = vector3df(4.8,0,0);
	else if(counter == 200)
		animationSpeed = vector3df(0,4.8,0);
	else if(counter == 225)
		animationSpeed = vector3df(-4.8,0,0);
	else if(counter == 250)
		animationSpeed = vector3df(0,4.8,0);
	else if(counter == 275)
		animationSpeed = vector3df(-4.8,0,0);
	else if(counter == 300)
		animationSpeed = vector3df(0,4.8,0);
	else if(counter == 325)
		animationSpeed = vector3df(-4.8,0,0);
	else if(counter == 350)
		animationSpeed = vector3df(0,4.8,0);
	else if(counter == 375)
		animationSpeed = vector3df(-4.8,0,0);
	else if(counter == 400)
		animationSpeed = vector3df(0,4.8,0);
	else if(counter < 25)
		animationSpeed += vector3df(-0.2,-0.2,0);
	else if(counter < 50)
		animationSpeed += vector3df(0.2,-0.2,0);
	else if(counter < 75)
		animationSpeed += vector3df(-0.2,-0.2,0);
	else if(counter < 100)
		animationSpeed += vector3df(0.2,-0.2,0);
	else if(counter < 125)
		animationSpeed += vector3df(-0.2,-0.2,0);
	else if(counter < 150)
		animationSpeed += vector3df(0.2,-0.2,0);
	else if(counter < 175)
		animationSpeed += vector3df(-0.2,-0.2,0);
	else if(counter < 200)
		animationSpeed += vector3df(0.2,-0.2,0);
	else if(counter < 225)
		animationSpeed += vector3df(0.2,-0.2,0);
	else if(counter < 250)
		animationSpeed += vector3df(-0.2,-0.2,0);
	else if(counter < 275)
		animationSpeed += vector3df(0.2,-0.2,0);
	else if(counter < 300)
		animationSpeed += vector3df(-0.2,-0.2,0);
	else if(counter < 325)
		animationSpeed += vector3df(0.2,-0.2,0);
	else if(counter < 350)
		animationSpeed += vector3df(-0.2,-0.2,0);
	else if(counter < 375)
		animationSpeed += vector3df(0.2,-0.2,0);
	else if(counter < 400)
		animationSpeed += vector3df(-0.2,-0.2,0);
	counter--;
}
