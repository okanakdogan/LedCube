#include <iostream>
#include <irrlicht.h>
#include <vector>



using namespace irr;

using namespace core;
using namespace scene;
using namespace video;
using namespace io;
using namespace gui;

using namespace std;

char* chartobin ( unsigned char c );

enum
{
    GUI_ID_RIGHT_TAB = 101,
    GUI_ID_BOTTOM_TAB,
    GUI_ID_FILE_OPEN_BUTTON,
    GUI_ID_TRANSPARENCY_SCROLL_BAR
};

class LedCubeData{
public:
    char m_data[8][8];

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


    LedCubeData(){}
    void setLed(char l, char r,char c){
        m_data[l][r]=m_data[l][r] | (char)1<<c;
    }
    void clearLed(char l, char r,char c){
        m_data[l][r]=m_data[l][r] & ~( (char)1<<c);
    }
    void changeAllto(char setValue){


        for(char i=0;i<8;i++)
            for(char j=0;j<8;++j)
                m_data[i][j]=setValue;
    }
    void print(){
        for(char i=0;i<8;i++)
            for(char j=0;j<8;++j)
                cout<<chartobin(m_data[i][j])<<endl;
    }

};

class CubeObject{
    private:
    vector<ISceneNode *>cubes;
    LedCubeData m_data;
    IVideoDriver* m_driver;
    const io::path path_on="../../media/green.jpg";
    const io::path path_off="../../media/black.jpg";

    vector<ISceneNode*> createOneRowCube(ISceneManager *smgr,core::vector3df position,float space){
    vector<ISceneNode*> row;
    ISceneNode* node;
    for(int i=0;i<8;i++){
        position.X+=space;
        row.push_back(node=smgr->addCubeSceneNode(1.0f,0,0,position));
        node->setMaterialFlag(EMF_LIGHTING,true);
    }
    return row;
}

vector<ISceneNode*> createOneFaceCube(ISceneManager *smgr,core::vector3df position,float space){
    vector<ISceneNode*> face;
    for(int i=0;i<8;i++){
       position.Y-=space;
       vector<ISceneNode*> result=createOneRowCube(smgr,position,space);
       face.insert(face.end(),result.begin(),result.end());
    }
    return face;
}

vector<ISceneNode*> createLedCube(ISceneManager *smgr,core::vector3df position,float space){

    vector<ISceneNode*> ledCube;
    for(int i=0;i<8;i++){
       position.Z+=space;
       vector<ISceneNode*> result=createOneFaceCube(smgr,position,space);
       ledCube.insert(ledCube.end(),result.begin(),result.end());
    }
    return ledCube;

}
public:
CubeObject(ISceneManager *smgr,IVideoDriver* driver,core::vector3df position,float space){
        cubes=createLedCube(smgr,position,space);
        m_driver=driver;
        m_data.changeAllto(0x01);
    }

    void setLedCubeTexture(IVideoDriver* driver,const io::path path){
        for(int i=0;i<512;i++)
            cubes.at(i)->setMaterialTexture( 0, driver->getTexture(path) );
    }

    void switchLed(core::vector3di pos,IVideoDriver* driver,const io::path path){

        cubes.at(pos.X*64+pos.Y*8+pos.Z)->setMaterialTexture( 0, driver->getTexture(path));

    }
    void switchRow(char layer,char row){
        char * arr=m_data.chartobin(m_data.m_data[layer][row]);

        for(char i=0;i<8;++i){
            cubes.at(layer*64+row*8+i)->setMaterialTexture( 0, m_driver->getTexture(arr[i]=='1'?path_on:path_off));
        }

    }

    LedCubeData getData(){
        return m_data;
    }
    void loadData(){
        cout<<"load data"<<endl;
        for(char i=0;i<8;i++)
            for(char j=0;j<8;j++){
                //bir satırı demo kupe aktar
                switchRow(i,j);
        }
    }
};





class MyEventReceiver : public IEventReceiver
{
public:
    // We'll create a struct to record info on the mouse state
    struct SMouseState
    {
        core::position2di Position;
        bool LeftButtonDown;
        SMouseState() : LeftButtonDown(false) { }
    } MouseState;

    // This is the one method that we have to implement
    virtual bool OnEvent(const SEvent& event)
    {
        // Remember the mouse state
        if (event.EventType == irr::EET_MOUSE_INPUT_EVENT)
        {
            switch(event.MouseInput.Event)
            {
            case EMIE_LMOUSE_PRESSED_DOWN:
                MouseState.LeftButtonDown = true;
                std::cout<<"click"<<std::endl;
                break;

            case EMIE_LMOUSE_LEFT_UP:
                MouseState.LeftButtonDown = false;
                break;

            case EMIE_MOUSE_MOVED:
                MouseState.Position.X = event.MouseInput.X;
                MouseState.Position.Y = event.MouseInput.Y;
                break;

            default:
                // We won't use the wheel
                break;
            }
        }

        return false;
    }

    const SMouseState & GetMouseState(void) const
    {
        return MouseState;
    }


    MyEventReceiver()
    {
    }

    };

vector<ISceneNode*> createOneRowCube(ISceneManager *smgr,core::vector3df position,float space);
vector<ISceneNode*> createOneFaceCube(ISceneManager *smgr,core::vector3df position,float space);
vector<ISceneNode*> createLedCube(ISceneManager *smgr,core::vector3df position,float space);
void setLedCubeTexture(const vector<ISceneNode*> & r,IVideoDriver* driver,const io::path path);
void switchLed(const vector<ISceneNode*> & r,core::vector3di pos,IVideoDriver* driver,const io::path path);
int main(int argc, char** argv)
{

    //core::dimension2d< u32 > windowDim(640, 480);
    MyEventReceiver receiver;

    core::dimension2d< u32 > windowDim(1024, 640);
    IrrlichtDevice *device =
        createDevice(EDT_SOFTWARE, windowDim, 16,
            false, false, false, &receiver);


    device->setWindowCaption(L"Hello World! - Irrlicht Engine Demo");


    IVideoDriver* driver = device->getVideoDriver();
    ISceneManager* smgr = device->getSceneManager();
    IGUIEnvironment* guienv = device->getGUIEnvironment();

    /*
    We add a hello world label to the window, using the GUI environment.
    */
    guienv->addStaticText(L"Hello World! This is the Irrlicht Software renderer!",
        rect<int>(10,10,200,22), true);


    //IAnimatedMesh* mesh = smgr->getMesh("../../led_base/led_base.obj");
    //IAnimatedMeshSceneNode* node = smgr->addAnimatedMeshSceneNode( mesh );
    //ISceneNode* node = smgr->addCubeSceneNode(10.0f,0,105);
    ISceneNode* node = smgr->addAnimatedMeshSceneNode( smgr->getMesh("../../media/led_base/led_base.obj") );
    //ISceneNode* node2 = smgr->addAnimatedMeshSceneNode( smgr->getMesh("../../media/led1/led1.obj") );
    //ISceneNode* node = smgr->addAnimatedMeshSceneNode( smgr->getMesh("../../media/sydney.md2") );


    if (node)
    {
        node->setMaterialFlag(EMF_LIGHTING, false);
        node->setPosition(core::vector3d<f32>(0,0,0));
        node->setScale(core::vector3d<f32>(2,2,2));
        //node->setFrameLoop(0, 310);
        //node->setMaterialTexture( 0, driver->getTexture("../../media/rockwall_height.bmp") );
    }

    rect<s32> tabcontrol(0,0,400,400);


    gui::IGUITabControl * rightTabControl= guienv->addTabControl(rect<s32>(windowDim.Width*0.70,0,
                                                                           windowDim.Width,windowDim.Height),0,true,true,102);
    gui::IGUITab * rightTab=rightTabControl->addTab(L"Hello World!",GUI_ID_RIGHT_TAB);


    gui::IGUITabControl * bottomTabControl= guienv->addTabControl(rect<s32>(0,windowDim.Height*0.70,
                                                                           windowDim.Width*0.70,windowDim.Height),0,true,true,103);

    gui::IGUITab * bottomTab=bottomTabControl->addTab(L"Bottom World!",GUI_ID_BOTTOM_TAB);

    guienv->addButton(rect<s32>(0,0,100,50),bottomTab,-1,L"button");

    for (s32 i=0; i<irr::gui::EGDC_COUNT ; ++i)
    {
        video::SColor col = guienv->getSkin()->getColor((EGUI_DEFAULT_COLOR)i);
        col.setAlpha(255);
        guienv->getSkin()->setColor((EGUI_DEFAULT_COLOR)i, col);
    }
    /*guienv->addButton(rect<s32>(0,0,400,400),0,103,L"Hello World!");*/
    /*
    To look at the mesh, we place a camera into 3d space at the position
    (0, 30, -40). The camera looks from there to (0,5,0).
    */
    scene::ICameraSceneNode * camera=smgr->addCameraSceneNode(0, vector3df(20,50,-60), vector3df(30,10,0));
    //scene::ICameraSceneNode* camera = smgr->addCameraSceneNodeFPS();

    /*scene::ICameraSceneNode * camera = smgr->addCameraSceneNode();
    camera->setPosition(core::vector3df(0, 0, -10));*/
    /*
    Ok, now we have set up the scene, lets draw everything:
    We run the device in a while() loop, until the device does not
    want to run any more. This would be when the user closed the window
    or pressed ALT+F4 in windows.
    */
     u32 then = device->getTimer()->getTime();
    const f32 MOVEMENT_SPEED = 5.f;
    CubeObject cube(smgr,driver,core::vector3df(5,45,-10),4);
    cube.setLedCubeTexture(driver,"../../media/black.jpg");
    cube.switchLed(core::vector3di(1,0,7),driver,"../../media/green.jpg");


    cube.loadData();
    while(device->run())
    {
        /*
        Anything can be drawn between a beginScene() and an endScene()
        call. The beginScene clears the screen with a color and also the
        depth buffer if wanted. Then we let the Scene Manager and the
        GUI Environment draw their content. With the endScene() call
        everything is presented on the screen.
        */
        const u32 now = device->getTimer()->getTime();
        const f32 frameDeltaTime = (f32)(now - then) / 1000.f; // Time in seconds
        then = now;

        core::vector3df nodePosition = node->getPosition();


/*
        // Create a ray through the mouse cursor.
            core::line3df ray = smgr->getSceneCollisionManager()->getRayFromScreenCoordinates(
                receiver.GetMouseState().Position, camera);

            // And intersect the ray with a plane around the node facing towards the camera.
            core::plane3df plane(nodePosition, core::vector3df(0, 0, -1));
            core::vector3df mousePosition;
            if(plane.getIntersectionWithLine(ray.start, ray.getVector(), mousePosition))
            {
                // We now have a mouse position in 3d space; move towards it.
                core::vector3df toMousePosition(mousePosition - nodePosition);
                const f32 availableMovement = MOVEMENT_SPEED * frameDeltaTime;

                if(toMousePosition.getLength() <= availableMovement)
                    nodePosition = mousePosition; // Jump to the final position
                else
                    nodePosition += toMousePosition.normalize() * availableMovement; // Move towards it
            }
            node->setPosition(nodePosition);

        // Turn lighting on and off depending on whether the left mouse button is down.
        node->setMaterialFlag(video::EMF_LIGHTING, receiver.GetMouseState().LeftButtonDown);
*/


        driver->beginScene(true, true, SColor(0,96,96,96));
        smgr->drawAll();
        guienv->drawAll();


        driver->endScene();
    }

    /*
    After we are finished, we have to delete the Irrlicht Device
    created before with createDevice(). In the Irrlicht Engine,
    you will have to delete all objects you created with a method or
    function which starts with 'create'. The object is simply deleted
    by calling ->drop().
    See the documentation at
    http://irrlicht.sourceforge.net//docu/classirr_1_1IUnknown.html#a3
    for more information.
    */
    device->drop();

    return 0;
}



