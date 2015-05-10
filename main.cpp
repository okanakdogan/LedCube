#include<iostream>
#include<vector>
#include "irrlicht.h"

#include "MyEventReceiver.h"
#include "Interface.h"
#include "LedCube.h"
#include "driverChoice.h"

#include "rs232.h"
#ifdef _WIN32
#include <windows.h>
#elif __linux
#include <unistd.h> // usleep
#endif

#include <stdio.h>
#include <stdlib.h>
#include <time.h>


#define BAUDRATE 115200
#define DELAY_TIME_US 100000
#define RECONNECT_TIME 100000
#define BUFFER_SIZE 4096
//#define DEVICE_ON 1

using namespace irr;
using namespace core;
using namespace scene;
using namespace video;
using namespace io;
using namespace gui;

using namespace std;

#ifdef _WIN32
DWORD WINAPI MyThreadFunction( LPVOID lpParam )
{
    cout<<"Thread"<<endl;
    threadInf *t = (threadInf*)lpParam;

    while(1){
        t->cube->checkSetCollisionObject(*(t->objs));

        }
    return 0;
}
#endif // _WIN32

int connectLedCube();
int sendArrayLedCube(int portNo, unsigned char ledArray[8][8][1]);

int main(){
    /*
	video::E_DRIVER_TYPE driverType=driverChoiceConsole();
	if (driverType==video::EDT_COUNT)
		return 1;
*/
	IrrlichtDevice *device = createDevice(EDT_OPENGL, FULLSCREEN, 16,false, false, false, NULL);

	IVideoDriver* driver = device->getVideoDriver();
	ISceneManager* smgr = device->getSceneManager();
	smgr->addCameraSceneNode(0, vector3df(100,80,-300), vector3df(100,20,20));
	Interface * intface = new Interface(device,driver);

	vector<Object> objectList;

	CubeObject cube(device->getSceneManager(),device->getVideoDriver(),core::vector3df(150,100,-50),16);
    cube.loadData();

	MyEventReceiver receiver(device,intface,objectList,&cube);
	device->setEventReceiver(&receiver);
    #ifdef _WIN32


    //threadInf tinf(&cube,&(receiver.objects));
    //HANDLE colThread=CreateThread(NULL,0,MyThreadFunction,&tinf,0,NULL);


    #endif // _WIN32
#ifdef DEVICE_ON
    unsigned char ledArray[8][8][1];
    unsigned char i, j, k =0, layer;
	int portNo;
	int status;



	while( (portNo = connectLedCube()) < 0){
		printf("Cannot open any port!\n");

		#ifdef _WIN32
			Sleep(1000);
		#elif __linux
			usleep(1000000);
		#endif
	}
#endif // DEVICE_ON


	int counter = 0;
	while(device->run())
	{

		stringw str = L"LED CUBE PROJECT   ";
		str += smgr->getActiveCamera()->getAbsolutePosition().X;
		str += "__";
		str += smgr->getActiveCamera()->getAbsolutePosition().Y;
		str += "__";
		str += smgr->getActiveCamera()->getAbsolutePosition().Z;
		str += "__&&__";
		str += smgr->getActiveCamera()->getTarget().X;
		str += "__";
		str += smgr->getActiveCamera()->getTarget().Y;
		str += "__";
		str += smgr->getActiveCamera()->getTarget().Z;
		device->setWindowCaption(str.c_str());

        #ifdef DEVICE_ON
        for (i = 0; i < 8; ++i) {
            for (j = 0; j < 8; ++j) {
                ledArray[i][j][0]= cube.getData().m_data[7-i][j];
                cout<<LedCubeData().chartobin(ledArray[i][j][0])<<endl;
            }
        }

        status = sendArrayLedCube(portNo, ledArray);
        #endif // DEVICE_ON

/*
        #ifdef _WIN32
			Sleep(80);
		#elif __linux
			usleep(80000);
		#endif
*/

		driver->beginScene(true, true, SCOL_GRAY);
		cube.checkSetCollisionObject(receiver.objects);
		smgr->drawAll();
		//cout << "draw ol OKKK\n";
		intface->guienv->drawAll();

		//cout <<"size :" <<receiver.objects.size() << endl ;

		driver->endScene();
	}
	#ifdef _WIN32
	//TerminateThread(colThread,0);
    //CloseHandle(colThread);
    #endif // _WIN32
    #ifdef DEVICE_ON
        CloseComport(portNo);
    #endif // DEVICE_ON


    device->drop();
	return 0;
}

int connectLedCube(){
	char buffer[BUFFER_SIZE];
	int portNum;
	int openedFlag = 0;

	for(portNum = 0; portNum < 6 ; ++portNum){
		if (OpenComport(portNum, BAUDRATE) == 0){
			printf("Port ttyACM%d opened succesfully\n", portNum);
			return portNum;
		}
	}

	return -1;
}

int sendArrayLedCube(int portNo, unsigned char ledArray[8][8][1]){

	int i, j;


	//send terminate character
	if (SendByte(portNo, '+') == 1){
		perror("SendByte");
	}else{
		printf("start gönderildi\n");
	}

	//usleep(100000);


	for (i = 0; i < 8; ++i) {
		for (j = 0; j < 8; ++j) {

			if (SendByte(portNo, ledArray[i][j][0]) == 1){
				perror("SendByte");

				#ifdef _WIN32
				system("PAUSE");
				#endif

				return -1;
			}
		//usleep(100);
		}
	}




	// send terminate character
	if (SendByte(portNo, '-') == 1){
		perror("SendByte");
	}else{
		printf("terminate gönderildi\n");
	}

	//usleep(100000);


	return 1;
}
