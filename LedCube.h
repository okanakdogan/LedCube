#ifndef LEDCUBE_H
#define LEDCUBE_H


/*-------------------
  Okan Akdogan
    Created 10.5.2015
---------------------*/

#include <iostream>
#include <vector>
#include "irrlicht.h"
#include "Objects.h"

#define VBOX_SPHERE_RAD 3.0f
const path path_on = "media/green.jpg";
const path path_off = "media/lightgrey.jpg";

using namespace std;
using namespace irr;

using namespace core;
using namespace scene;
using namespace video;
using namespace io;
using namespace gui;

    class LedCubeData{
    public:
        unsigned char m_data[8][8];

        char* chartobin ( unsigned char c );
        LedCubeData();
        void setLed(char l, char r,char c);
        void clearLed(char l, char r,char c);
        void changeAllto(char setValue);
        void print();

    };

    class CubeObject{
        private:
        vector<ISceneNode *>cubes;
        LedCubeData m_data;
        IVideoDriver* m_driver;

        vector<ISceneNode*> createOneRowCube(ISceneManager *smgr,core::vector3df position,float space);

        vector<ISceneNode*> createOneFaceCube(ISceneManager *smgr,core::vector3df position,float space);

        vector<ISceneNode*> createLedCube(ISceneManager *smgr,core::vector3df position,float space);

    public:
    CubeObject(ISceneManager *smgr,IVideoDriver* driver,core::vector3df position,float space);

        void setLedCubeTexture(IVideoDriver* driver,const io::path path);

        void switchLed(core::vector3di pos,bool status);
        void switchRow(char layer,char row);

        LedCubeData& getData();
        void loadData();
        void checkSetCollision(vector<ISceneNode*> &painted);

        void checkSetCollisionObject(vector<Object> &objects);

        void setVisible(bool isVisible);

    };
    #ifdef _WIN32

    struct threadInf{
        CubeObject * cube;
        vector<Object> * objs;
        threadInf(CubeObject*c,vector<Object>*o){
            cube=c;
            objs=o;
        }
    };
    #endif //win32
#endif
