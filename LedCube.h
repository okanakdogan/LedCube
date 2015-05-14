#ifndef LEDCUBE_H
#define LEDCUBE_H

#include <iostream>
#include <vector>
#include "irrlicht.h"
#include "Objects.h"
#include "Node.h"

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

    char* chartobin ( unsigned char c )
    {
        static char bin[CHAR_BIT + 1] = { 0 };
        int i;

        for ( i = CHAR_BIT - 1; i >= 0; i-- )
        {
            bin[i] = (c % 2) + '0';
            c /= 2;
        }

        return bin;
    }


    LedCubeData(){
      for(unsigned char i=0;i<8;i++)
            for(unsigned char j=0;j<8;++j)
                m_data[i][j]=0;
    }
    void setLed(unsigned char l,unsigned char r,unsigned char c){
        m_data[l][r]=m_data[l][r] | (char)1<<(CHAR_BIT-1-c);
    }
    void clearLed(unsigned char l,unsigned char r,unsigned char c){
        m_data[l][r]=m_data[l][r] & ~( (char)1<<(CHAR_BIT-1-c));
    }
    void changeAllto(unsigned char setValue){
        for(unsigned char i=0;i<8;i++)
            for(unsigned char j=0;j<8;++j)
                m_data[i][j]=setValue;
    }
    void print(){
        for(unsigned char i=0;i<8;i++)
            for(unsigned char j=0;j<8;++j)
                std::cout<<chartobin(m_data[i][j])<<std::endl;
    }

};
/*
class LedCubeDataUpdated{
public:
    unsigned char m_data[8][8];

    char* chartobin ( unsigned char c )
    {
        static char bin[CHAR_BIT + 1] = { 0 };
        int i;

        for ( i = CHAR_BIT - 1; i >= 0; i-- )
        {
            bin[i] = (c % 2) + '0';
            c /= 2;
        }

        return bin;
    }


    LedCubeData(){
      for(char i=0;i<8;i++)
            for(char j=0;j<8;++j)
                m_data[i][j]=0;
    }
    void setLed(char r, char c,char d){
        m_data[r][c]=m_data[r][c] | (char)1<<c;
    }
    void clearLed(char r, char c,char d){
        m_data[r][c]=m_data[r][c] & ~( (char)1<<c);
    }
    void changeAllto(char setValue){
        for(char i=0;i<8;i++)
            for(char j=0;j<8;++j)
                m_data[i][j]=setValue;
    }
    void print(){
        for(char i=0;i<8;i++)
            for(char j=0;j<8;++j)
                std::cout<<chartobin(m_data[i][j])<<std::endl;
    }

};
*/
class CubeObject{
    private:
    vector<ISceneNode *>cubes;
    LedCubeData m_data;
    IVideoDriver* m_driver;
    aabbox3df ledAbox;

    vector<ISceneNode*> createOneRowCube(ISceneManager *smgr,core::vector3df position,float space){
    vector<ISceneNode*> row;
    ISceneNode* node;
    for(int i=0;i<8;i++){

        row.push_back(node=smgr->addSphereSceneNode(VBOX_SPHERE_RAD,16,0,-1,position));
        node->setMaterialFlag(EMF_LIGHTING,false);
        node->setMaterialType(EMT_TRANSPARENT_ADD_COLOR);
        position.X+=space;
    }
    return row;
}

vector<ISceneNode*> createOneFaceCube(ISceneManager *smgr,core::vector3df position,float space){
    vector<ISceneNode*> face;
    for(int i=0;i<8;i++){

       vector<ISceneNode*> result=createOneRowCube(smgr,position,space);
       face.insert(face.end(),result.begin(),result.end());
       position.Y-=space;
    }
    return face;
}

vector<ISceneNode*> createLedCube(ISceneManager *smgr,core::vector3df position,float space){

    vector<ISceneNode*> ledCube;
    for(int i=0;i<8;i++){

       vector<ISceneNode*> result=createOneFaceCube(smgr,position,space);
       ledCube.insert(ledCube.end(),result.begin(),result.end());
       position.Z+=space;
    }
    return ledCube;

}
public:
CubeObject(ISceneManager *smgr,IVideoDriver* driver,core::vector3df position,float space){
        cubes=createLedCube(smgr,position,space);
        m_driver=driver;
        ledAbox=aabbox3df(position,position+core::vector3df(space*8,space*8,space*8));
    }

    void setLedCubeTexture(IVideoDriver* driver,const io::path path){
        for(int i=0;i<512;i++)
            cubes.at(i)->setMaterialTexture( 0, driver->getTexture(path) );
    }

    void switchLed(core::vector3di pos,bool status){

        ISceneNode* cube=cubes.at(pos.X*64+pos.Y*8+pos.Z);
        cube->setMaterialTexture( 0, m_driver->getTexture(status==true?path_on:path_off));
         cube->setMaterialType(status==true?EMT_SOLID:EMT_TRANSPARENT_ADD_COLOR);
    }
    void switchRow(unsigned char layer,unsigned char row){
        char * arr=m_data.chartobin(m_data.m_data[layer][row]);
        ISceneNode* cube;
        for(char i=0;i<8;++i){
            cube=cubes.at(layer*64+row*8+i);
            cube->setMaterialTexture( 0, m_driver->getTexture(arr[i]=='1'?path_on:path_off));
            cube->setMaterialType(arr[i]=='1'?EMT_SOLID:EMT_TRANSPARENT_ADD_COLOR);
        }

    }

    LedCubeData& getData(){
        return m_data;
    }
    void loadData(){

        for(char i=0;i<8;i++)
            for(char j=0;j<8;j++){
                //bir satýrý demo kupe aktar
                switchRow(i,j);
        }
    }
    void checkSetCollision(vector<Node> &painted){
		if(painted.size() == 0)
            return ;

        ISceneNode *part;
        int psize=painted.size();
        //std::cout<<"psize="<<psize<<endl;
        for(int i=0; i<512; ++i){
            int loc=0;


            while(loc<psize){
                bool collision=false;
                /*collision check*/
				part=painted[loc].node;
				part->updateAbsolutePosition();
				aabbox3df box = aabbox3df(part->getTransformedBoundingBox().MinEdge-1, part->getTransformedBoundingBox().MaxEdge+1);
				if(box.isPointInside(cubes[i]->getPosition()))
                {
                    switchLed(core::vector3di(i/64,(i%64)/8,((i%64)%8)),true);
                    /*change data*/
                    m_data.setLed(i/64,(i%64)/8,((i%64)%8));
                    collision=true;
                    //std::cout<<"collision"<<endl;
                    break;
                }

                loc++;

            }
            /*if(collision==false){
                switchLed(core::vector3di(i/64,(i%64)/8,((i%64)%8)),false);
				m_data.clearLed(i/64,(i%64)/8,((i%64)%8));
			}*/

        }

    }

    void checkSetCollisionObject(vector<Object> &objects){

    	if(objects.size() == 0)
    		return ;

        ISceneNode* sph ;
        Object  *part ;
        int psize=objects.size();
        //std::cout<<"psize="<<psize<<endl;
        for(int i=0;i<512;++i){
            sph=cubes.at(i);
            int loc=0;
            bool collision=false;

            while(loc<psize){

                /*collision check*/
                part=&objects.at(loc);
                if(part->isPointInside(sph->getPosition()) /*&& sph->getPosition().X>=part->node->getBoundingBox().MaxEdge.X*/)
                {
            		//cerr << "loc :" << loc << "  i : " << i << endl ;
                    switchLed(core::vector3di(i/64,(i%64)/8,((i%64)%8)),true);
                    /*change data*/
                    m_data.setLed(i/64,(i%64)/8,((i%64)%8));
                    collision=true;
                    //std::cout<<"collision"<<endl;
                    break;
                }

                loc++;

            }
            if(collision==false){
                switchLed(core::vector3di(i/64,(i%64)/8,((i%64)%8)),false);
                m_data.clearLed(i/64,(i%64)/8,((i%64)%8));
                }
        }


    }
     void checkSetCollisionObjectOpt(vector<Object> &objects){
         if(objects.size() == 0)
                return ;

        ISceneNode* sph ;
        Object  *part ;
        int loc=0;
        int psize=objects.size();

        while(loc<psize){
            part=&objects.at(loc++);

            /*abox collision check*/
            if(!(ledAbox.intersectsWithBox(part->node->getTransformedBoundingBox()))){
               continue;
            }

            /*collision check*/
             for(int i=0;i<512;++i){
                sph=cubes.at(i);
                if(part->isPointInside(sph->getPosition()) /*&& sph->getPosition().X>=part->node->getBoundingBox().MaxEdge.X*/)
                {
                    /*change data*/
                    m_data.setLed(i/64,(i%64)/8,((i%64)%8));
                }
             }
             loadData();
        }

     }

	void setVisible(bool isVisible){
		for(unsigned char  i=0; i<cubes.size(); ++i){
			if(cubes[i]->getMaterial(0).MaterialType == EMT_TRANSPARENT_ADD_COLOR){
				cubes[i]->setVisible(isVisible);
			}
		}
	}

};
/*
void threeCubeDataArray(char *tcArray,char * mCube,char * sCube,char * tCube){

    for(int i=0;i<8;i++)
        for(int j=0;j<8;j++)
            tcArray[][]


}
*/
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
