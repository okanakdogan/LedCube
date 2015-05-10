
#include "myEventReceiver.h"

int main()
{
    // ask user for driver
    video::E_DRIVER_TYPE driverType=driverChoiceConsole();
    if (driverType==video::EDT_COUNT)
        return 1;

    
   	
    
    IrrlichtDevice* device = createDevice(driverType,
            core::dimension2d<u32>(640, 480));
    IGUIEnvironment* guienv = device->getGUIEnvironment();
    // create device

    SAppContext context;
    context.device = device;

    // Then create the event receiver, giving it that context structure.
    MyEventReceiver receiver(context);
    device->setEventReceiver(&receiver);

    // And tell the device to use our custom event receiver.
    


    if (device == 0)
        return 1; // could not create selected driver.

    irr::core::line3df line(10, 50, 10, 300, 100, 400);
    driver = device->getVideoDriver();
    SMaterial material;
	material.Lighting = false;
	material.BackfaceCulling = false;       
	material.ZBuffer = irr::video::ECFN_ALWAYS;
	driver->setMaterial(material);

    smgr = device->getSceneManager();

    scene::ISceneNode * node = smgr->addMeshSceneNode(
        smgr->addArrowMesh( "Arrow",
                video::SColor(255, 255, 0, 0),
                video::SColor(255, 0, 255, 0),
                16,16,
                2.f, 1.3f,
                0.1f, 0.6f
                )
        );
    node->setMaterialFlag(video::EMF_LIGHTING, false);

    scene::ICameraSceneNode * camera = smgr->addCameraSceneNode();
    camera->setPosition(core::vector3df(0, 0, -10));

    // As in example 04, we'll use framerate independent movement.
    u32 then = device->getTimer()->getTime();
    const f32 MOVEMENT_SPEED = 1000.f;

    

	while(device->run())
 	{



		driver->beginScene(true, true, video::SColor(255,113,113,133));
        // Work out a frame delta time.
     	const u32 now = device->getTimer()->getTime();
      const f32 frameDeltaTime = (f32)(now - then) / 1000.f; // Time in seconds
      then = now;

      bool movedWithJoystick = false;
      core::vector3df nodePosition = node->getPosition();

      /*vector3df cameraPosition = camera->getPosition();

    	if(receiver.isKeyDown(irr::KEY_KEY_D))
			camera->setPosition(vector3df(cameraPosition.X-1,cameraPosition.Y,cameraPosition.Z));
		if(receiver.isKeyDown(irr::KEY_KEY_A))
			camera->setPosition(vector3df(cameraPosition.X+1,cameraPosition.Y,cameraPosition.Z));
		if(receiver.isKeyDown(irr::KEY_KEY_W))
			camera->setPosition(vector3df(cameraPosition.X,cameraPosition.Y-1,cameraPosition.Z));
		if(receiver.isKeyDown(irr::KEY_KEY_S))
			camera->setPosition(vector3df(cameraPosition.X,cameraPosition.Y+1,cameraPosition.Z));
		if(receiver.isKeyDown(irr::KEY_KEY_Q))
			camera->setPosition(vector3df(cameraPosition.X,cameraPosition.Y,cameraPosition.Z-1));
		if(receiver.isKeyDown(irr::KEY_KEY_E))
			camera->setPosition(vector3df(cameraPosition.X,cameraPosition.Y,cameraPosition.Z+1));*/
		
		
		
		
       
       

        	core::vector3df temp = mousePosition ;
            // Create a ray through the mouse cursor.
            core::line3df ray = smgr->getSceneCollisionManager()->getRayFromScreenCoordinates(
                receiver.GetMouseState().Position, camera);

            // And intersect the ray with a plane around the node facing towards the camera.
            core::plane3df plane(nodePosition, core::vector3df(0, 0, -1));
            

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

            if(receiver.GetMouseState().LeftButtonDown == true){
	        	


	        	if(1){
		        	float x_ = temp.X -mousePosition.X;
		        	float y_ = temp.Y -mousePosition.Y;
		        	for(int i = 0  ; i < 50 ; i++){

			        	ISceneNode *cube = smgr->addCubeSceneNode(punto);
		        		cube->setPosition(vector3df(mousePosition.X+x_*0.02*i,mousePosition.Y+y_*0.02*i,mousePosition.Z));
		    			myNode.push_back(cube);
		        	}
	        	}

	        	
	        	/*core::stringw str = "";
	            str += mousePosition.X;
	            str += "-";
	            str += mousePosition.Y;
	            str += "-";
	            str += mousePosition.Z;

				device->setWindowCaption(str.c_str());*/
	        }

			if(receiver.GetMouseState().RightButtonDown == true)
        	{
        		

        		cout << mousePosition.X << "\t"<< mousePosition.Y  << "\t"<< mousePosition.Z << "\n";
        		if(myNode.size() != 0){
    				
	        		temp.X = mousePosition.X - myNode[0]->getPosition().X;
	    			temp.Y = mousePosition.Y - myNode[0]->getPosition().Y;
	        		for(int i  = 0 ; i < myNode.size() ; i++)
					{
						vector3df cubePos = myNode[i]->getPosition();
						vector3df newPos = vector3df(cubePos.X+temp.X ,cubePos.Y+temp.Y,cubePos.Z);
						myNode[i]->setPosition(newPos);

	        		}
    			}
        	}


        }


       

        node->setPosition(nodePosition);

        // Turn lighting on and off depending on whether the left mouse button is down.
        node->setMaterialFlag(video::EMF_LIGHTING, receiver.GetMouseState().LeftButtonDown);

        

        smgr->drawAll(); // draw the 3d scene
       
        guienv->drawAll();
        driver->endScene();
    
    	
    }

    device->drop();

    return 0;
}





