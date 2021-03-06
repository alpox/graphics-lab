#include "RenderProject.h"

/* Initialize the Project */
void RenderProject::init()
{
	bRenderer::loadConfigFile("config.json");	// load custom configurations replacing the default values in Configuration.cpp

	// let the renderer create an OpenGL context and the main window
	if(Input::isTouchDevice())
		bRenderer().initRenderer(true);										// full screen on iOS
	else
		bRenderer().initRenderer(1920, 1080, false, "The Cave - Demo");		// windowed mode on desktop
		//bRenderer().initRenderer(View::getScreenWidth(), View::getScreenHeight(), true);		// full screen using full width and height of the screen

	// start main loop 
	bRenderer().runRenderer();
}

/* This function is executed when initializing the renderer */
void RenderProject::initFunction()
{
	// get OpenGL and shading language version
	bRenderer::log("OpenGL Version: ", glGetString(GL_VERSION));
	bRenderer::log("Shading Language Version: ", glGetString(GL_SHADING_LANGUAGE_VERSION));

	// initialize variables
	_offset = 0.0f;
	_randomOffset = 0.0f;
	_cameraSpeed = 40.0f;
	_running = false; _lastStateSpaceKey = bRenderer::INPUT_UNDEFINED; _lastBlueState = bRenderer::INPUT_UNDEFINED;
	_viewMatrixHUD = Camera::lookAt(vmml::Vector3f(0.0f, 20.0f, 0.25f), vmml::Vector3f::ZERO, vmml::Vector3f::UP);

	// set shader versions (optional)
	bRenderer().getObjects()->setShaderVersionDesktop("#version 120");
	bRenderer().getObjects()->setShaderVersionES("#version 100");

	// load materials and shaders before loading the model
	ShaderPtr customShader = bRenderer().getObjects()->generateShader("customShader", { 5, true, true, true, true, true, true, true, true, true, false, false, false });	// automatically generates a shader with a maximum of 2 lights
	//ShaderPtr flameShader = bRenderer().getObjects()->loadShaderFile("flame", 0, false, true, true, false, false);				// load shader from file without lighting, the number of lights won't ever change during rendering (no variable number of lights)
	ShaderPtr flameShader = bRenderer().getObjects()->loadShaderFile_o("flame", 0, AMBIENT_LIGHTING);				// load shader from file without lighting, the number of lights won't ever change during rendering (no variable number of lights)
	MaterialPtr flameMaterial = bRenderer().getObjects()->loadObjMaterial("flame.mtl", "flame", flameShader);				// load material from file using the shader created above

	// create additional properties for a model
	PropertiesPtr flameProperties = bRenderer().getObjects()->createProperties("flameProperties");
	PropertiesPtr streamProperties = bRenderer().getObjects()->createProperties("streamProperties");

	// load models	//bRenderer().getObjects()->loadObjModel_o("boulder01.obj", 4, FLIP_T | FLIP_Z | VARIABLE_NUMBER_OF_LIGHTS);
	//bRenderer().getObjects()->loadObjModel("cave.obj", true, true, false, 4, true, false);								// automatically generates a shader with a maximum of 4 lights (number of lights may vary between 0 and 4 during rendering without performance loss)
							// automatically generates a shader with a maximum of 4 lights (number of lights may vary between 0 and 4 during rendering without performance loss)
	//bRenderer().getObjects()->loadObjModel_o("stalagmite.obj", 4, FLIP_T | FLIP_Z | VARIABLE_NUMBER_OF_LIGHTS);
	
	//bRenderer().getObjects()->loadObjModel("cave_stream.obj", true, true, true, 4, false, false, streamProperties);		// automatically loads shader files according to the name of the material
	//bRenderer().getObjects()->loadObjModel_o("cave_stream.obj", 4, FLIP_T | FLIP_Z | SHADER_FROM_FILE, streamProperties);		// automatically loads shader files according to the name of the material
	//bRenderer().getObjects()->loadObjModel("crystal.obj", false, true, customShader);									// the custom shader created above is used
	//bRenderer().getObjects()->loadObjModel_o("crystal.obj", customShader, FLIP_Z);									// the custom shader created above is used
	//bRenderer().getObjects()->loadObjModel("torch.obj", false, true, false, 1, false, true);							// create custom shader with a maximum of 1 light
	//bRenderer().getObjects()->loadObjModel_o("torch.obj", 1, FLIP_Z | AMBIENT_LIGHTING);							// create custom shader with a maximum of 1 light
	// create sprites
	bRenderer().getObjects()->createSprite_o("flame", flameMaterial, NO_OPTION, flameProperties);				// create a sprite using the material created above, to pass additional properties a Properties object is used
	bRenderer().getObjects()->createSprite("sparks", "sparks.png");										// create a sprite displaying sparks as a texture
	bRenderer().getObjects()->createSprite("bTitle", "basicTitle_light.png");							// create a sprite displaying the title as a texture

	// create text sprites
	FontPtr font = bRenderer().getObjects()->loadFont("KozGoPro-ExtraLight.otf", 50);
	if (Input::isTouchDevice())
		bRenderer().getObjects()->createTextSprite("instructions", vmml::Vector3f(1.f, 1.f, 1.f), "Double tap to start", font);
	else
		bRenderer().getObjects()->createTextSprite("instructions", vmml::Vector3f(1.f, 1.f, 1.f), "Press space to start", font);

	// create camera
	bRenderer().getObjects()->createCamera("camera", vmml::Vector3f(20.f, -10.f, 0.f), vmml::Vector3f(0.f, 0.f, 0.f));

	// create lights
	//bRenderer().getObjects()->createLight("firstLight", vmml::Vector3f(0.0f, 50.0f, -120.0f), vmml::Vector3f(0.5f, 0.5f, 0.5f), vmml::Vector3f(0.01f, 0.01f, 0.01f), 0.2f, 0.0f, 50.0f);
	//bRenderer().getObjects()->createLight("firstLight", vmml::Vector3f(78.0f, -3.0f, 0.0f), vmml::Vector3f(0.5f, 0.5f, 1.0f), vmml::Vector3f(1.0f, 1.0f, 1.0f), 100.0f, 0.4f, 100.0f);
	//bRenderer().getObjects()->createLight("secondLight", vmml::Vector3f(-60.0f, 50.0f, 270.0f), vmml::Vector3f(0.5f, 0.5f, 0.5f), vmml::Vector3f(0.01f, 0.01f, 0.01f), 0.2f, 0.0f, 50.0f);
	//bRenderer().getObjects()->createLight("secondLight", vmml::Vector3f(148.0f, -3.0f, 15.0f), vmml::Vector3f(0.3f, 1.0f, 0.3f), vmml::Vector3f(1.0f, 1.0f, 1.0f), 100.0f, 0.8f, 100.0f);
	//bRenderer().getObjects()->createLight("thirdLight", vmml::Vector3f(0.0f, 50.0f, 0.0f), vmml::Vector3f(0.5f, 0.5f, 0.5f), vmml::Vector3f(0.01f, 0.01f, 0.01f), 0.2f, 0.0f, 50.0f);
	//bRenderer().getObjects()->createLight("thirdLight", vmml::Vector3f(218.0f, -3.0f, 0.0f), vmml::Vector3f(0.8f, 0.2f, 0.2f), vmml::Vector3f(1.0f, 1.0f, 1.0f), 100.0f, 0.8f, 100.0f);
    bRenderer().getObjects()->createLight("torchLight", bRenderer().getObjects()->getCamera("camera")->getPosition(), vmml::Vector3f(0.8f, 0.8f, 0.8f), vmml::Vector3f(0.15f, 0.15f, 0.15f), 1.3f, 0.0f, 250.f);


	// postprocessing
	bRenderer().getObjects()->createFramebuffer("fbo");					// create framebuffer object
	bRenderer().getObjects()->createTexture("fbo_texture1", 0.f, 0.f);	// create texture to bind to the fbo
	bRenderer().getObjects()->createTexture("fbo_texture2", 0.f, 0.f);	// create texture to bind to the fbo
	//ShaderPtr blurShader = bRenderer().getObjects()->loadShaderFile("blurShader", 0, false, false, false, false, false);			// load shader that blurs the texture
	ShaderPtr blurShader = bRenderer().getObjects()->loadShaderFile_o("blurShader", 0);			// load shader that blurs the texture
	MaterialPtr blurMaterial = bRenderer().getObjects()->createMaterial("blurMaterial", blurShader);								// create an empty material to assign either texture1 or texture2 to
	bRenderer().getObjects()->createSprite("blurSprite", blurMaterial);																// create a sprite using the material created above
    
    //---------------------------------------------------
    //           ***** Add all entities *****
    //---------------------------------------------------
    
    

    //ShaderPtr sceneShader = bRenderer().getObjects()->loadShaderFile("sceneShader", 4, true, true, true, true, false);
    //world.createRenderModel("test22", modelTransform, modelRender, sceneShader, FLIP_T | FLIP_Z | VARIABLE_NUMBER_OF_LIGHTS);

	//ShaderPtr caveShader = bRenderer().getObjects()->loadShaderFile("caveShader", 4, true, true, true, true, false);
	//world.createRenderModel("test22", modelTransform, modelRender, caveShader, FLIP_T | FLIP_Z | VARIABLE_NUMBER_OF_LIGHTS);

    //ShaderPtr sceneShader = bRenderer().getObjects()->loadShaderFile("sceneShader", 4, true, true, true, true, false);
    //ShaderPtr scene = bRenderer().getObjects()->loadShaderFile("scene", 0);
    //world.createRenderModel("test3", modelTransform, modelRender, scene, FLIP_T | FLIP_Z);
   
    vmml::Matrix4f modelMatrix = vmml::create_scaling(vmml::Vector3f(3.f));
    
    TransformPtr modelTransform = TransformPtr(new Transform(modelMatrix));
    RenderPtr modelRender = RenderPtr(new Render(vmml::Vector3f({ 0.0f, 0.0f, 0.0f }), std::vector<std::string>({ "torchLight" }), false, false, false));
    ShaderPtr scene = bRenderer().getObjects()->loadShaderFile("scene1", 0, false, false, false, false, false);

    //EntityPtr entity = world.createRenderModel("caveAndBase", modelTransform, modelRender, scene, FLIP_T | FLIP_Z);
	EntityPtr entity = world.createRenderModel("caveSystem6", modelTransform, modelRender, scene, FLIP_T | FLIP_Z);

	ShaderPtr fall = bRenderer().getObjects()->loadShaderFile("fallShader", 0, false, false, false, false, false);
	entity = world.createRenderModel("Fall", modelTransform, modelRender, fall, FLIP_T | FLIP_Z);

	StreamPtr stream = StreamPtr(new Stream());
	entity->addComponent(stream);
    
    
    ColliderPtr collider = ColliderPtr(new Collider(EFFECT_CEL));
    modelMatrix = vmml::create_translation(vmml::Vector3f(0.f, 5.f, 0.f)) * vmml::create_scaling(0.3f);
    entity = world.createRenderModel("potion", TransformPtr(new Transform(modelMatrix)), modelRender, scene);
    entity->addComponent(collider);
    
    collider = ColliderPtr(new Collider(EFFECT_BLUEVISION));
    modelMatrix = vmml::create_translation(vmml::Vector3f(10.f, 5.f, 50.f)) * vmml::create_scaling(0.3f);
    entity = world.createRenderModel("potion_blue", TransformPtr(new Transform(modelMatrix)), modelRender, scene);
    entity->addComponent(collider);
    
    
    /*** Cave stream ***/
    /*modelRender = RenderPtr(new Render(bRenderer(), "cave_stream", "cave_stream_instance", "camera", std::vector<std::string>({ "torchLight", "firstLight", "secondLight", "thirdLight" }), true, false, true, GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA, 1.0f));
    
    Entity& entity = world.createRenderModel(modelTransform, modelRender);
    entity.addComponent(StreamPtr(new Stream(_offset)));*/
    
    /*** boulder ***/
    /*modelMatrix = vmml::create_translation(vmml::Vector3f(-70.0f, 50.0f, 280.0f)) * vmml::create_scaling(vmml::Vector3f(1.0f));
    modelTransform = TransformPtr(new Transform(modelMatrix));
    modelRender = RenderPtr(new Render(bRenderer(), "boulder01", "boulder01_instance", "camera", std::vector<std::string>({ "torchLight", "firstLight", "secondLight", "thirdLight" }), true, true));
    
    world.createRenderModel(modelTransform, modelRender);*/

	/*** stalagmite ***/
	/*modelMatrix = vmml::create_translation(vmml::Vector3f(0.0f, 5.0f, 0.0f)) * vmml::create_scaling(vmml::Vector3f(9.0f));
	modelTransform = TransformPtr(new Transform(modelMatrix));
	modelRender = RenderPtr(new Render(bRenderer(), "stalagmite", "stalagmite_instance", "camera", std::vector<std::string>({ "torchLight", "firstLight", "secondLight", "thirdLight" }), true, true));

	world.createRenderModel(modelTransform, modelRender);*/
    
    ///*** Y_Tube ***/
    //modelMatrix = vmml::create_translation(vmml::Vector3f(-75.0f, 35.0, 260.0)) * vmml::create_scaling(vmml::Vector3f(3.0f)) * vmml::create_rotation(3.14f, vmml::Vector3f::UNIT_Y);
    //modelTransform = TransformPtr(new Transform(modelMatrix));
    //modelRender = RenderPtr(new Render(bRenderer(), "L_Curve", "L_Curve_instance", "camera", std::vector<std::string>({ "torchLight", "firstLight", "secondLight", "thirdLight" }), true, true));
    
    //world.createRenderModel(modelTransform, modelRender);
    
    /*** Crystal (blue) ***/
    /*modelMatrix = vmml::create_translation(vmml::Vector3f(78.0f, -17.0f, 5.5f)) * vmml::create_scaling(vmml::Vector3f(0.1f));
    modelTransform = TransformPtr(new Transform(modelMatrix));
    modelRender = RenderPtr(new Render(bRenderer(), "crystal", "crystal_blue", "camera", vmml::Vector3f(0.2f, 0.2f, 1.0f), std::vector<std::string>({ "torchLight", "firstLight" }), true, false, true));
    
    world.createRenderModel(modelTransform, modelRender);*/
    
    /*** Crystal (green) ***/
    /*modelMatrix = vmml::create_translation(vmml::Vector3f(148.0f, -17.0f, 15.0f)) * vmml::create_scaling(vmml::Vector3f(0.1f));
    modelTransform = TransformPtr(new Transform(modelMatrix));
    modelRender = RenderPtr(new Render(bRenderer(), "crystal", "crystal_green", "camera", vmml::Vector3f(0.2f, 0.7f, 0.2f), std::vector<std::string>({ "torchLight", "secondLight" }), true, false, true));
    
    world.createRenderModel(modelTransform, modelRender);*/
    
    /*** Crystal (red) ***/
    /*modelMatrix = vmml::create_translation(vmml::Vector3f(218.0f, -17.0f, 4.0f)) * vmml::create_scaling(vmml::Vector3f(0.1f));
    modelTransform = TransformPtr(new Transform(modelMatrix));
    modelRender = RenderPtr(new Render(bRenderer(), "crystal", "crystal_red", "camera", vmml::Vector3f(0.8f, 0.2f, 0.2f), std::vector<std::string>({ "torchLight", "thirdLight" }), true, false, true));

    
    world.createRenderModel(modelTransform, modelRender);    */
    
    world.addSystem<StreamSystem>();
    world.addSystem<CollisionSystem>();

	// Update render queue
	updateRenderQueue("camera", 0.0f);
}

/* Draw your scene here */
void RenderProject::loopFunction(const double &deltaTime, const double &elapsedTime)
{
//	bRenderer::log("FPS: " + std::to_string(1 / deltaTime));	// write number of frames per second to the console every frame

	//// Draw Scene and do post processing ////

	/// Begin post processing ///
	GLint defaultFBO;
	if (!_running){
		bRenderer().getView()->setViewportSize(bRenderer().getView()->getWidth() / 5, bRenderer().getView()->getHeight() / 5);		// reduce viewport size
		defaultFBO = Framebuffer::getCurrentFramebuffer();	// get current fbo to bind it again after drawing the scene
		bRenderer().getObjects()->getFramebuffer("fbo")->bindTexture(bRenderer().getObjects()->getTexture("fbo_texture1"), false);	// bind the fbo
	}

	/// Draw scene ///	
	
	bRenderer().getModelRenderer()->drawQueue(/*GL_LINES*/);
	bRenderer().getModelRenderer()->clearQueue();
	
	if (!_running){
		/// End post processing ///		
        /*** Blur ***/
		// translate
		vmml::Matrix4f modelMatrix = vmml::create_translation(vmml::Vector3f(0.0f, 0.0f, -0.5));
		// blur vertically and horizontally
		bool b = true;		int numberOfBlurSteps = 2;
		for (int i = 0; i < numberOfBlurSteps; i++) {
			if (i == numberOfBlurSteps - 1){
				bRenderer().getObjects()->getFramebuffer("fbo")->unbind(defaultFBO); //unbind (original fbo will be bound)
				bRenderer().getView()->setViewportSize(bRenderer().getView()->getWidth(), bRenderer().getView()->getHeight());								// reset vieport size
			}
			else
				bRenderer().getObjects()->getFramebuffer("fbo")->bindTexture(bRenderer().getObjects()->getTexture(b ? "fbo_texture2" : "fbo_texture1"), false);
			bRenderer().getObjects()->getMaterial("blurMaterial")->setTexture("fbo_texture", bRenderer().getObjects()->getTexture(b ? "fbo_texture1" : "fbo_texture2"));
			bRenderer().getObjects()->getMaterial("blurMaterial")->setScalar("isVertical", static_cast<GLfloat>(b));
			// draw
			bRenderer().getModelRenderer()->drawModel(bRenderer().getObjects()->getModel("blurSprite"), modelMatrix, _viewMatrixHUD, vmml::Matrix4f::IDENTITY, std::vector<std::string>({}), false);
			b = !b;
		}
	
        /*** Title ***/
        // translate and scale 
        GLfloat titleScale = 0.5f;
        vmml::Matrix4f scaling = vmml::create_scaling(vmml::Vector3f(titleScale / bRenderer().getView()->getAspectRatio(), titleScale, titleScale));
		modelMatrix = vmml::create_translation(vmml::Vector3f(-0.4f, 0.0f, -0.65f)) * scaling;
        // draw
		bRenderer().getModelRenderer()->drawModel(bRenderer().getObjects()->getModel("bTitle"), modelMatrix, _viewMatrixHUD, vmml::Matrix4f::IDENTITY, std::vector<std::string>({}), false, false);

		/*** Instructions ***/
		titleScale = 0.1f;
		scaling = vmml::create_scaling(vmml::Vector3f(titleScale / bRenderer().getView()->getAspectRatio(), titleScale, titleScale));
		modelMatrix = vmml::create_translation(vmml::Vector3f(-0.45f / bRenderer().getView()->getAspectRatio(), -0.6f, -0.65f)) * scaling;
		// draw
		bRenderer().getModelRenderer()->drawModel(bRenderer().getObjects()->getTextSprite("instructions"), modelMatrix, _viewMatrixHUD, vmml::Matrix4f::IDENTITY, std::vector<std::string>({}), false);
    }

	//// Camera Movement ////
	updateCamera("camera", deltaTime);

    
    /*
    _timeElapsed += deltaTime;
    
    // let the light flicker
    float flickeringLightPosX = 20*sin(_timeElapsed);
    float flickeringLightPosY = 20*cos(_timeElapsed);
    
    bRenderer().getObjects()->getLight("torchLight")->setPosition(vmml::Vector3f(flickeringLightPosX, 10.f, flickeringLightPosY));
     */
    
    bRenderer().getObjects()->getLight("torchLight")->setPosition(-bRenderer().getObjects()->getCamera("camera")->getPosition());


    /*
	//// Torch Light ////
	if (_running){
		if (deltaTime > 0.0f){
			_offset += 5 * deltaTime;
			_randomOffset += (randomNumber(10.0f, 20.0f)) * deltaTime;
		}
		// let the light follow the camera
		GLfloat flickeringLightPosX = -bRenderer().getObjects()->getCamera("camera")->getPosition().x();
		GLfloat flickeringLightPosY = -bRenderer().getObjects()->getCamera("camera")->getPosition().y();
		GLfloat flickeringLightPosZ = -bRenderer().getObjects()->getCamera("camera")->getPosition().z();
		// let the light flicker
		flickeringLightPosX += 2*sin(flickeringLightPosY + 0.5f*_randomOffset);
		flickeringLightPosY += 2*sin(flickeringLightPosX + 0.5f*_randomOffset);
		bRenderer().getObjects()->getLight("torchLight")->setPosition(bRenderer().getObjects()->getCamera("camera")->getPosition());
	}
	else{
		// set the light to be at the camera position
<<<<<<< HEAD
		//bRenderer().getObjects()->getLight("torchLight")->setPosition(-bRenderer().getObjects()->getCamera("camera")->getPosition() - bRenderer().getObjects()->getCamera("camera")->getForward()*10.0f);
		bRenderer().getObjects()->getLight("torchLight")->setPosition(-bRenderer().getObjects()->getCamera("camera")->getPosition());
=======
		bRenderer().getObjects()->getLight("torchLight")->setPosition(bRenderer().getObjects()->getCamera("camera")->getPosition());
>>>>>>> 112ced26759920f4357bd228c73d520f31fd0fcb
	}
     */
	/// Update render queue ///
	updateRenderQueue("camera", deltaTime);

	// Quit renderer when escape is pressed
	if (bRenderer().getInput()->getKeyState(bRenderer::KEY_ESCAPE) == bRenderer::INPUT_PRESS)
		bRenderer().terminateRenderer();
}

/* This function is executed when terminating the renderer */
void RenderProject::terminateFunction()
{
	bRenderer::log("I totally terminated this Renderer :-)");
}

/* Update render queue */
void RenderProject::updateRenderQueue(const std::string &camera, const double &deltaTime)
{
    
	/*** Cave ***/
    world.render(deltaTime);

	GLint currentBlueState = bRenderer().getInput()->getKeyState(bRenderer::KEY_B);
	if (currentBlueState != _lastBlueState)
	{
		_lastBlueState = currentBlueState;
		if (currentBlueState == bRenderer::INPUT_PRESS) {
			if (bRenderer().getObjects()->getShader("scene1") != nullptr) {
				if (stateChange == 0) { stateChange = 1; }
				else { stateChange = 0; }
				bRenderer().getObjects()->getShader("scene1")->setUniform("blueShift", stateChange);
				std::cout << "I was here and need a longer message" << _lastBlueState;
			}
		}
	}
}

/* Camera movement */
void RenderProject::updateCamera(const std::string &camera, const double &deltaTime)
{
	//// Adjust aspect ratio ////
	bRenderer().getObjects()->getCamera(camera)->setAspectRatio(bRenderer().getView()->getAspectRatio());

	double deltaCameraY = 0.0;
	double deltaCameraX = 0.0;
	double cameraForward = 0.0;
	double cameraSideward = 0.0;

	/* iOS: control movement using touch screen */
	if (Input::isTouchDevice()){

		// pause using double tap
		if (bRenderer().getInput()->doubleTapRecognized()){
			_running = !_running;
		}

		if (_running){
			// control using touch
			TouchMap touchMap = bRenderer().getInput()->getTouches();
			int i = 0;
			for (auto t = touchMap.begin(); t != touchMap.end(); ++t)
			{
				Touch touch = t->second;
				// If touch is in left half of the view: move around
				if (touch.startPositionX < bRenderer().getView()->getWidth() / 2){
					cameraForward = -(touch.currentPositionY - touch.startPositionY) / 100;
					cameraSideward = (touch.currentPositionX - touch.startPositionX) / 100;

				}
				// if touch is in right half of the view: look around
				else
				{
					deltaCameraY = (touch.currentPositionX - touch.startPositionX) / 2000;
					deltaCameraX = (touch.currentPositionY - touch.startPositionY) / 2000;
				}
				if (++i > 2)
					break;
			}
		}

	}
	/* Windows: control movement using mouse and keyboard */
	else{
		// use space to pause and unpause
		GLint currentStateSpaceKey = bRenderer().getInput()->getKeyState(bRenderer::KEY_SPACE);
		if (currentStateSpaceKey != _lastStateSpaceKey)
		{
			_lastStateSpaceKey = currentStateSpaceKey;
			if (currentStateSpaceKey == bRenderer::INPUT_PRESS){
				_running = !_running;
				bRenderer().getInput()->setCursorEnabled(!_running);
			}
		}

		// mouse look
		double xpos, ypos; bool hasCursor = false;
		bRenderer().getInput()->getCursorPosition(&xpos, &ypos, &hasCursor);

		deltaCameraY = (xpos - _mouseX) / 1000;
		_mouseX = xpos;
		deltaCameraX = (ypos - _mouseY) / 1000;
		_mouseY = ypos;

		if (_running){
			// movement using wasd keys
			if (bRenderer().getInput()->getKeyState(bRenderer::KEY_W) == bRenderer::INPUT_PRESS)
				if (bRenderer().getInput()->getKeyState(bRenderer::KEY_LEFT_SHIFT) == bRenderer::INPUT_PRESS) 			cameraForward = 2.0;
				else			cameraForward = 1.0;
			else if (bRenderer().getInput()->getKeyState(bRenderer::KEY_S) == bRenderer::INPUT_PRESS)
				if (bRenderer().getInput()->getKeyState(bRenderer::KEY_LEFT_SHIFT) == bRenderer::INPUT_PRESS) 			cameraForward = -2.0;
				else			cameraForward = -1.0;
			else
				cameraForward = 0.0;

			if (bRenderer().getInput()->getKeyState(bRenderer::KEY_A) == bRenderer::INPUT_PRESS)
				cameraSideward = -1.0;
			else if (bRenderer().getInput()->getKeyState(bRenderer::KEY_D) == bRenderer::INPUT_PRESS)
				cameraSideward = 1.0;
			if (bRenderer().getInput()->getKeyState(bRenderer::KEY_UP) == bRenderer::INPUT_PRESS)
				bRenderer().getObjects()->getCamera(camera)->moveCameraUpward(_cameraSpeed*deltaTime);
			else if (bRenderer().getInput()->getKeyState(bRenderer::KEY_DOWN) == bRenderer::INPUT_PRESS)
				bRenderer().getObjects()->getCamera(camera)->moveCameraUpward(-_cameraSpeed*deltaTime);
			if (bRenderer().getInput()->getKeyState(bRenderer::KEY_LEFT) == bRenderer::INPUT_PRESS)
				bRenderer().getObjects()->getCamera(camera)->rotateCamera(0.0f, 0.0f, 0.03f*_cameraSpeed*deltaTime);
			else if (bRenderer().getInput()->getKeyState(bRenderer::KEY_RIGHT) == bRenderer::INPUT_PRESS)
				bRenderer().getObjects()->getCamera(camera)->rotateCamera(0.0f, 0.0f, -0.03f*_cameraSpeed*deltaTime);
		}
	}

	//// Update camera ////
	if (_running){
		bRenderer().getObjects()->getCamera(camera)->moveCameraForward(cameraForward*_cameraSpeed*deltaTime);
		bRenderer().getObjects()->getCamera(camera)->rotateCamera(deltaCameraX, deltaCameraY, 0.0f);
		bRenderer().getObjects()->getCamera(camera)->moveCameraSideward(cameraSideward*_cameraSpeed*deltaTime);
	}	
}

/* For iOS only: Handle device rotation */
void RenderProject::deviceRotated()
{
	if (bRenderer().isInitialized()){
		// set view to full screen after device rotation
		bRenderer().getView()->setFullscreen(true);
		bRenderer::log("Device rotated");
	}
}

/* For iOS only: Handle app going into background */
void RenderProject::appWillResignActive()
{
	if (bRenderer().isInitialized()){
		// stop the renderer when the app isn't active
		bRenderer().stopRenderer();
	}
}

/* For iOS only: Handle app coming back from background */
void RenderProject::appDidBecomeActive()
{
	if (bRenderer().isInitialized()){
		// run the renderer as soon as the app is active
		bRenderer().runRenderer();
	}
}

/* For iOS only: Handle app being terminated */
void RenderProject::appWillTerminate()
{
	if (bRenderer().isInitialized()){
		// terminate renderer before the app is closed
		bRenderer().terminateRenderer();
	}
}

/* Helper functions */
GLfloat RenderProject::randomNumber(GLfloat min, GLfloat max){
	return min + static_cast <GLfloat> (rand()) / (static_cast <GLfloat> (RAND_MAX / (max - min)));
}
