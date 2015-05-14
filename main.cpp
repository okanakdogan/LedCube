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

#define DEVICE_ON 1
#define THREECUBE 1

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
        t->cube->checkSetCollisionObjectOpt(*(t->objs));
    }
    return 0;
}
#endif // _WIN32

int connect_ledcube();
int connect_manuel_ledcube(int);
int connect_auto_ledcube();
int sendArrayLedCube(int portNo, unsigned char ledArray[8][8]);
int sendArrayLedCubes(int portNo, unsigned char ledArray[8][24]);
void show_rain(unsigned char ledArray[8][8]);



int main(){
	video::E_DRIVER_TYPE driverType=driverChoiceConsole();
	if (driverType==video::EDT_COUNT)
		return 1;

	IrrlichtDevice *device = createDevice(driverType, FULLSCREEN, 16,false, false, false, NULL);

	IVideoDriver* driver = device->getVideoDriver();
	ISceneManager* smgr = device->getSceneManager();
	smgr->addCameraSceneNode(0, vector3df(100,80,-300), vector3df(100,20,20));
	Interface * intface = new Interface(device,driver);

	vector<Object> objectList;
    CubeObject *cubes[3];

    int space=16;
    core::vector3df cubePosition(150,0,-50);

	CubeObject mainCube(device->getSceneManager(),device->getVideoDriver(),cubePosition,space);
    cubePosition+=core::vector3df(space*8,0,0);
	CubeObject secondCube(device->getSceneManager(),device->getVideoDriver(),cubePosition,space);
    cubePosition+=core::vector3df(space*8,0,0);
	CubeObject thirdCube(device->getSceneManager(),device->getVideoDriver(),cubePosition,space);

    (cubes[0]=&mainCube)->loadData();
    (cubes[1]=&secondCube)->loadData();
    (cubes[2]=&thirdCube)->loadData();

    bool threeCube=false;

	MyEventReceiver receiver(device,intface,objectList,&mainCube);
	device->setEventReceiver(&receiver);
    #ifdef _WIN32

    //threadInf tinf(&cube,&(receiver.objects));
    //HANDLE colThread=CreateThread(NULL,0,MyThreadFunction,&tinf,0,NULL);

    #endif // _WIN32

#ifdef DEVICE_ON
    #if THREECUBE==0
        unsigned char ledArray[8][8];
    #else
        unsigned char ledArray[8][24];
    #endif // THREECUBE

    unsigned char i, j, k =0, layer;
	int portNo;
	int status;
	srand(time(NULL));



	// Connect to led cube
	portNo = connect_ledcube();

    #if THREECUBE==0
        // Clear array
        for (i = 0; i < 8; ++i) {
            for (j = 0; j < 8; ++j) {
                ledArray[i][j]= 0;
            }
        }
    #else
        // Clear array
        for (i = 0; i < 8; ++i) {
            for (j = 0; j < 24; ++j) {
                ledArray[i][j]= 0;
            }
        }
    #endif // THREECUBE


	//ledArray[dikey][z ekseni] = [yatay]
	//ledArray[0][1] = 255;

    SendByte(portNo,0xff);
    SendByte(portNo,0xff);

    Sleep(5);



#endif // DEVICE_ON


	int counter = 0;
	while(device->run())
	{
/*
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
*/
        #ifdef DEVICE_ON

        #if THREECUBE==0
            for (i = 0; i < 8; ++i) {
                for (j = 0; j < 8; ++j) {
                    ledArray[i][j]= mainCube.getData().m_data[j][7-i];
                   // cout<<LedCubeData().chartobin(mainCube.getData().m_data[i][j])<<endl;
                    cout<<LedCubeData().chartobin(ledArray[i][j])<<endl;
                }
            }
            status = sendArrayLedCube(portNo, ledArray);
        #else/*
            int cubeInd=0;
            for (i = 0; i < 8; ++i) {
                for (j = 0; j < 24; ++j) {
                    ledArray[i][j]= cubes[cubeInd]->getData().m_data[j%8][7-i];
                   // cout<<LedCubeData().chartobin(mainCube.getData().m_data[i][j])<<endl;
                    //cout<<LedCubeData().chartobin(ledArray[i][j])<<endl;
                    if(j==7 || j==15)
                        cubeInd++;
                }
            }
            cubeInd=0;*/
            for (i = 0; i < 8; ++i) {
                for (j = 0; j < 8; ++j) {
                    ledArray[i][j]= mainCube.getData().m_data[j][7-i];
                   // cout<<LedCubeData().chartobin(mainCube.getData().m_data[i][j])<<endl;
                    //cout<<LedCubeData().chartobin(ledArray[i][j])<<endl;
                }
            }
            for (i = 0; i < 8; ++i) {
                for (j = 8; j < 16; ++j) {
                    ledArray[i][j]= secondCube.getData().m_data[j%8][7-i];
                   // cout<<LedCubeData().chartobin(mainCube.getData().m_data[i][j])<<endl;
                    //cout<<LedCubeData().chartobin(ledArray[i][j])<<endl;
                }
            }
            for (i = 0; i < 8; ++i) {
                for (j = 16; j < 24; ++j) {
                    ledArray[i][j]= thirdCube.getData().m_data[j%8][7-i];
                   // cout<<LedCubeData().chartobin(mainCube.getData().m_data[i][j])<<endl;
                    //cout<<LedCubeData().chartobin(ledArray[i][j])<<endl;
                }
            }
            status = sendArrayLedCubes(portNo, ledArray);
        #endif // THREECUBE

/*
		#ifdef _WIN32
			//Sleep(10);
		#elif __linux
			usleep(10000);
		#endif
*/
		// Array gönderme hatası
		if(status == -1){
			//printf("Array not sent!\n");

			CloseComport(portNo);

			return -1;
		}else
			//printf("Array gönderildi %d\n", ledArray[0][0]);
        #endif // DEVICE_ON

/*
        #ifdef _WIN32
			Sleep(80);
		#elif __linux
			usleep(80000);
		#endif
*/

		driver->beginScene(true, true, SCOL_GRAY);
        for(short i=0;i<3;i++){
            cubes[i]->getData().changeAllto(0x00);
            cubes[i]->loadData();
            cubes[i]->checkSetCollisionObject(receiver.objects);
            for(short j=0; j<receiver.drawingObj.size(); j++){
                cubes[i]->checkSetCollision(receiver.drawingObj[j].node);
            }
        }


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

int connect_ledcube(){

	int select;
	int flag = 1;
	int portNo;

	printf("Select connection type:\n");
	printf("1: Automatic connection\n");
	printf("2: Manuel connection\n\n");

	while(flag){
		printf("select: ");
		scanf("%d", &select);

		// Auto connection part
		if(select == 1){
			flag = 0;
			while( (portNo = connect_auto_ledcube()) < 0){
				printf("Cannot open any port!\n");

				#if defined (_WIN32) || defined( _WIN64)
					Sleep(1000);
				#elif __linux
					usleep(1000000);
				#endif
			}

			return portNo;
		}

		// Manuel connection part
		else if(select == 2){
			flag = 0;
			printf("Enter port number: \n");
			scanf("%d", &portNo);

			portNo = connect_manuel_ledcube(portNo);

			if(portNo != -1)
				return portNo;
		}

		else{
			printf("Wrong selection! Try again...\n\n");
		}
	}

	// Any connection failed
	return -1;
}

int connect_auto_ledcube(){
	char buffer[BUFFER_SIZE];
	int portNum;
	int openedFlag = 0;

	for(portNum = 0; portNum < 6 ; ++portNum){
		if (OpenComport(portNum, BAUDRATE) == 0){

			#if defined (_WIN32) || defined( _WIN64)
				printf("COM%d opened succesfully\n", portNum+1);
			#elif __linux
				printf("ttyACM%d opened succesfully\n", portNum);
			#endif

			return portNum;
		}
	}

	return -1;
}

int connect_manuel_ledcube(int portNum){

	if (OpenComport(portNum, BAUDRATE) == 0){
		printf("Port ttyACM%d opened succesfully\n", portNum);
		return portNum;
	}else{
		perror("Connection Failed: ");

		#if defined (_WIN32) || defined( _WIN64)
		system("PAUSE");
		#endif

		return -1;
	}

}

int sendArrayLedCube(int portNo, unsigned char ledArray[8][8]){

	static unsigned char current[8][8] = { 0 };
	char wr[512];
	unsigned char length = 0;
	int i,j,k;
	int result;

	for ( i = 0; i < 8; i++){
		for ( j = 0; j < 8; j++){
            //cout<<(int)ledArray[i][j]<<" ";
			if (current[i][j] != ledArray[i][j]){
				wr[length++] = i * 32 + j;
				wr[length++] = ledArray[i][j];
				current[i][j] = ledArray[i][j];
				//cout<<"w1"<< i * 32 + j<<"w2"<<(int)ledArray[i][j]<<endl;
			}
		}
		//cout<<endl;
	}

	result = SendBuf(portNo, wr, length);

	return result;
}
int sendArrayLedCubes(int portNo, unsigned char ledArray[8][24]){

	static unsigned char current[8][24] = { 0 };
	char wr[512];
	unsigned char length = 0;
	int i,j,k;
	int result;

	for ( i = 0; i < 8; i++){
		for ( j = 0; j < 24; j++){
            //cout<<(int)ledArray[i][j]<<" ";
			if (current[i][j] != ledArray[i][j]){
				wr[length++] = i * 32 + j;
				wr[length++] = ledArray[i][j];
				current[i][j] = ledArray[i][j];
				//cout<<"w1"<< i * 32 + j<<"w2"<<(int)ledArray[i][j]<<endl;
			}
		}
		//cout<<endl;
	}

	result = SendBuf(portNo, wr, length);

	return result;
}
