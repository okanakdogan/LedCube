/*-------------------
  Okan Akdogan
    Created 10.5.2015
---------------------*/

#include "LedCube.h"

LedCubeData::LedCubeData(){
      for(char i=0;i<8;i++)
            for(char j=0;j<8;++j)
                m_data[i][j]=0;
    }


char* LedCubeData::chartobin ( unsigned char c )
    {
        static char bin[CHAR_BIT + 1] = { 0 };
        int i;

        for ( i = CHAR_BIT - 1; i >= 0; i-- ){
            bin[i] = (c % 2) + '0';
            c /= 2;
        }
        return bin;
    }

    void LedCubeData::setLed(char l, char r,char c){
        m_data[l][r]=m_data[l][r] | (char)1<<CHAR_BIT-1-c;
    }
    void LedCubeData::clearLed(char l, char r,char c){
        m_data[l][r]=m_data[l][r] & ~( (char)1<<CHAR_BIT-1-c);
    }

    void LedCubeData::changeAllto(char setValue){
        for(char i=0;i<8;i++)
            for(char j=0;j<8;++j)
                m_data[i][j]=setValue;
    }
    void LedCubeData::print(){
        for(char i=0;i<8;i++)
            for(char j=0;j<8;++j)
                std::cout<<chartobin(m_data[i][j])<<std::endl;
    }


//Cube Object

vector<ISceneNode*> CubeObject::createOneRowCube(ISceneManager *smgr,core::vector3df position,float space){
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

vector<ISceneNode*> CubeObject::createOneFaceCube(ISceneManager *smgr,core::vector3df position,float space){
    vector<ISceneNode*> face;
    for(int i=0;i<8;i++){

       vector<ISceneNode*> result=createOneRowCube(smgr,position,space);
       face.insert(face.end(),result.begin(),result.end());
       position.Y-=space;
    }
    return face;
}

vector<ISceneNode*> CubeObject::createLedCube(ISceneManager *smgr,core::vector3df position,float space){

    vector<ISceneNode*> ledCube;
    for(int i=0;i<8;i++){

       vector<ISceneNode*> result=createOneFaceCube(smgr,position,space);
       ledCube.insert(ledCube.end(),result.begin(),result.end());
       position.Z+=space;
    }
    return ledCube;

}

CubeObject::CubeObject(ISceneManager *smgr,IVideoDriver* driver,core::vector3df position,float space){
        cubes=createLedCube(smgr,position,space);
        m_driver=driver;
        //m_data.changeAllto(0xFF);
    }

void CubeObject::setLedCubeTexture(IVideoDriver* driver,const io::path path){
        for(int i=0;i<512;i++)
            cubes.at(i)->setMaterialTexture( 0, driver->getTexture(path) );
    }

void CubeObject::switchLed(core::vector3di pos,bool status){

        ISceneNode* cube=cubes.at(pos.X*64+pos.Y*8+pos.Z);
        cube->setMaterialTexture( 0, m_driver->getTexture(status==true?path_on:path_off));
         cube->setMaterialType(status==true?EMT_SOLID:EMT_TRANSPARENT_ADD_COLOR);
    }
void CubeObject::switchRow(char layer,char row){
        char * arr=m_data.chartobin(m_data.m_data[layer][row]);
        ISceneNode* cube;
        for(char i=0;i<8;++i){
            cube=cubes.at(layer*64+row*8+i);
            cube->setMaterialTexture( 0, m_driver->getTexture(arr[i]=='1'?path_on:path_off));
            cube->setMaterialType(arr[i]=='1'?EMT_SOLID:EMT_TRANSPARENT_ADD_COLOR);
        }

    }

LedCubeData& CubeObject::getData(){
        return m_data;
}

void CubeObject::loadData(){

        for(char i=0;i<8;i++)
            for(char j=0;j<8;j++){
                //bir satýrý demo kupe aktar
                switchRow(i,j);
        }
    }
void CubeObject::checkSetCollision(vector<ISceneNode*> &painted){
        ISceneNode* sph,*part;
        int psize=painted.size();
        //std::cout<<"psize="<<psize<<endl;
        for(short i=0;i<512;++i){
            sph=cubes.at(i);
            int loc=0;
            bool collision=false;

            while(loc<psize){
                /*collision check*/
                part=painted.at(loc);
                //if((part->getPosition()-sph->getPosition()).getLength()<VBOX_SPHERE_RAD+0.1f/*part->getRadius()*/)
                if(part->getBoundingBox().isPointInside(sph->getPosition()))
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
            if(collision==false)
                switchLed(core::vector3di(i/64,(i%64)/8,((i%64)%8)),false);
        }
    }

void CubeObject::checkSetCollisionObject(vector<Object> &objects){

    	if(objects.size() == 0)
		{
			getData().changeAllto(0x00);
			loadData();
    		return ;
		}

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
                if(part->isPointInside(sph->getPosition()) && sph->getPosition().X>=part->node->getBoundingBox().MaxEdge.X)
               //if(sph->getTransformedBoundingBox().isFullInside(part->getTransformedBoundingBox()))
        		//if(part->getTransformedBoundingBox().isPointTotalInside(sph->getTransformedBoundingBox().getCenter()))
               //if((part->getPosition()-sph->getPosition()).getLength()<VBOX_SPHERE_RAD+0.1f/*part->getRadius()*/)
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

void CubeObject::setVisible(bool isVisible){
		for(int i=0; i<cubes.size(); ++i){
			if(cubes[i]->getMaterial(0).MaterialType == EMT_TRANSPARENT_ADD_COLOR){
				cubes[i]->setVisible(isVisible);
			}
		}
}
