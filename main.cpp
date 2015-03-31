/*
The program will show how to use the
basics of the VideoDriver, the GUIEnvironment and the
SceneManager.

To use the engine, we will have to include the header file
irrlicht.h, which can be found in the Irrlicht Engine SDK
directory \include.
*/
#include <irrlicht.h>

/*
In the Irrlicht Engine, everything can be found in the namespace
'irr'. So if you want to use a class of the engine, you have to
write an irr:: before the name of the class. For example to use
the IrrlichtDevice write: irr::IrrlichtDevice. To get rid of the
irr:: in front of the name of every class, we tell the compiler
that we use that namespace from now on, and we will not have to
write that 'irr::'.
*/
using namespace irr;

/*
There are 5 sub namespaces in the Irrlicht Engine. Take a look
at them, you can read a detailed description of them in the
documentation by clicking on the top menu item 'Namespace List'
or using this link: http://irrlicht.sourceforge.net/docu/namespaces.html.
Like the irr Namespace, we do not want these 5 sub namespaces now,
to keep this example simple. Hence we tell the compiler again
that we do not want always to write their names:
*/
using namespace core;
using namespace scene;
using namespace video;
using namespace io;
using namespace gui;

enum
{
    GUI_ID_RIGHT_TAB = 101,
    GUI_ID_BOTTOM_TAB,
    GUI_ID_FILE_OPEN_BUTTON,
    GUI_ID_TRANSPARENCY_SCROLL_BAR
};
/*
This is the main method. We can use void main() on every platform.
On Windows platforms, we could also use the WinMain method
if we would want to get rid of the console window, which pops up when
starting a program with main(), but to keep this example simple,
we use main().
*/
int main(int argc, char** argv)
{
    /*
    The most important function of the engine is the 'createDevice'
    function. The Irrlicht Device can be created with it, which is the
    root object for doing everything with the engine.
    createDevice() has 7 paramters:
    deviceType: Type of the device. This can currently be the Null-device,
       the Software device, DirectX8, DirectX9, or OpenGL. In this example we use
       EDT_SOFTWARE, but to try out, you might want to change it to
       EDT_NULL, EDT_DIRECTX8 , EDT_DIRECTX9, or EDT_OPENGL.
    windowSize: Size of the Window or FullscreenMode to be created. In this
       example we use 640x480.
    bits: Amount of bits per pixel when in fullscreen mode. This should
       be 16 or 32. This parameter is ignored when running in windowed mode.
    fullscreen: Specifies if we want the device to run in fullscreen mode
       or not.
    stencilbuffer: Specifies if we want to use the stencil buffer for drawing shadows.
    vsync: Specifies if we want to have vsync enabled, this is only useful in fullscreen
      mode.
    eventReceiver: An object to receive events. We do not want to use this
       parameter here, and set it to 0.
    */
    //core::dimension2d< u32 > windowDim(640, 480);
    core::dimension2d< u32 > windowDim(1024, 640);
    IrrlichtDevice *device =
        createDevice(EDT_SOFTWARE, windowDim, 16,
            false, false, false, 0);

    /*
    Set the caption of the window to some nice text. Note that there is
    a 'L' in front of the string. The Irrlicht Engine uses wide character
    strings when displaying text.
    */
    device->setWindowCaption(L"Hello World! - Irrlicht Engine Demo");

    /*
    Get a pointer to the video driver, the SceneManager and the
    graphical user interface environment, so that
    we do not always have to write device->getVideoDriver(),
    device->getSceneManager() and device->getGUIEnvironment().
    */
    IVideoDriver* driver = device->getVideoDriver();
    ISceneManager* smgr = device->getSceneManager();
    IGUIEnvironment* guienv = device->getGUIEnvironment();

    /*
    We add a hello world label to the window, using the GUI environment.
    */
    guienv->addStaticText(L"Hello World! This is the Irrlicht Software renderer!",
        rect<int>(10,10,200,22), true);

    /*
    To display something interesting, we load a Quake 2 model
    and display it. We only have to get the Mesh from the Scene
    Manager (getMesh()) and add a SceneNode to display the mesh.
    (addAnimatedMeshSceneNode()). Instead of writing the filename
    sydney.md2, it would also be possible to load a Maya object file
    (.obj), a complete Quake3 map (.bsp) or a Milshape file (.ms3d).
    By the way, that cool Quake 2 model called sydney was modelled
    by Brian Collins.
    */
    //IAnimatedMesh* mesh = smgr->getMesh("../../led_base/led_base.obj");
    //IAnimatedMeshSceneNode* node = smgr->addAnimatedMeshSceneNode( mesh );
    //ISceneNode* node = smgr->addCubeSceneNode(10.0f,0,105);
    ISceneNode* node = smgr->addAnimatedMeshSceneNode( smgr->getMesh("../../media/led_base/led_base.obj") );
    ISceneNode* node2 = smgr->addAnimatedMeshSceneNode( smgr->getMesh("../../media/led1/led1.obj") );
    //ISceneNode* node = smgr->addAnimatedMeshSceneNode( smgr->getMesh("../../media/sydney.md2") );
    /*
    To let the mesh look a little bit nicer, we change its material a
    little bit: We disable lighting because we do not have a dynamic light
    in here, and the mesh would be totally black. Then we set the frame
    loop, so that the animation is looped between the frames 0 and 310.
    And at last, we apply a texture to the mesh. Without it the mesh
    would be drawn using only a color.
    */
    if (node)
    {
        node->setMaterialFlag(EMF_LIGHTING, false);
        node->setPosition(core::vector3d<f32>(0,0,0));
        node->setScale(core::vector3d<f32>(1,1,1));
        //node->setFrameLoop(0, 310);
        //node->setMaterialTexture( 0, driver->getTexture("../../media/rockwall_height.bmp") );
    }
    if (node2)
    {
        node2->setMaterialFlag(EMF_LIGHTING, false);
        node2->setPosition(core::vector3d<f32>(-5,-50,10));
        node2->setScale(core::vector3d<f32>(1,1,1));
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
    smgr->addCameraSceneNode(0, vector3df(10,30,40), vector3df(-13,-10,0));
    //scene::ICameraSceneNode* camera = smgr->addCameraSceneNodeFPS();

    /*
    Ok, now we have set up the scene, lets draw everything:
    We run the device in a while() loop, until the device does not
    want to run any more. This would be when the user closed the window
    or pressed ALT+F4 in windows.
    */
    while(device->run())
    {
        /*
        Anything can be drawn between a beginScene() and an endScene()
        call. The beginScene clears the screen with a color and also the
        depth buffer if wanted. Then we let the Scene Manager and the
        GUI Environment draw their content. With the endScene() call
        everything is presented on the screen.
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

