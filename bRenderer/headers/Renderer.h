#ifndef B_RENDERER_H
#define B_RENDERER_H

/* bRenderer includes */
#include "OSdetect.h"
#include "Renderer_GL.h"
#include "Logger.h"
#include "View.h"
#include "Input.h"
#include "ObjectManager.h"
#include "RenderQueue.h"

/* vmmlib includes */
#include "vmmlib/util.hpp"
#include "vmmlib/frustum_culler.hpp"

#ifdef __OBJC__
@class RendererCaller;
#endif

class IRenderProject;

/** @brief The main class that is able to initialize and maintain everything that is necessary to render an image to the screen
*	@author Benjamin B�rgisser
*/
class Renderer
{
public:
    
#ifndef __OBJC__
    typedef int RendererCaller;
#endif

	/* Functions */

	/**	@brief Constructor
	*/
	Renderer();

	/**	@brief Virtual destructor
	*/
	virtual ~Renderer(){}

	/**	@brief Returns a pointer to the view of the renderer
	*
	*	This function returns a pointer to the view of the renderer. The view is where the OpenGL context and
	*	on desktop systems a window is created and managed. On iOS a UIView is created instead.
	*
	*/
	ViewPtr getView();

	/**	@brief Returns a pointer to the input handler of the renderer
	*/
	InputPtr getInput();

	/**	@brief Returns a pointer to the object management of the renderer
	*/
	ObjectManagerPtr getObjects();

	/**	@brief Returns a pointer to the render queue of the renderer
	*/
	RenderQueuePtr getRenderQueue();

	/**	@brief Returns true if the renderer has already been initialized
	*/
	bool isInitialized();

	/**	@brief Returns true if the renderer is running
	*/
	bool isRunning();

	/**	@brief Sets the project instance to be used for function calls
	*
	*	The init, loop and terminate function of the set instance get called automatically.
	*
	*	@param[in] p The project instance
	*/
	void setRenderProject(IRenderProject *p);

	/**	@brief Sets a static function that gets called when initializing the renderer
	*
	*	In an application a function can be defined that gets called immediately after the renderer is initialized.
	*
	*	@param[in] f The function to be called
	*/
	void setInitFunction(void(*f)());

	/**	@brief Sets a static function that gets called repeatedly when looping
	*
	*	The set function gets called repeatedly while the renderer is running.
	*	Usually the scene gets drawn and setup in the defined function.
	*
	*	@param[in] f The function to be called
	*/
	void setLoopFunction(void(*f)(const double deltaTime, const double elapsedTime));

	/**	@brief Sets a static function that gets called when terminating the renderer
	*
	*	The set function gets called when the renderer is terminated.
	*	Here allocated memory can be freed, instances deleted and statuses changed.
	*
	*	@param[in] f The function to be called
	*/
	void setTerminateFunction(void(*f)());

	/**	@brief Do all necessary initializations for the renderer to be ready to run (returns true in the case of success)
	*	@param[in] windowTitle The title for the window on desktop systems (optional)
	*/
	bool initRenderer(std::string windowTitle = bRenderer::DEFAULT_WINDOW_TITLE());

	/**	@brief Do all necessary initializations for the renderer to be ready to run (returns true in the case of success)
	*	@param[in] fullscreen Decides whether or not the application runs in full screen mode
	*	@param[in] windowTitle The title for the window on desktop systems (optional)
	*/
	bool initRenderer(bool fullscreen, std::string windowTitle = bRenderer::DEFAULT_WINDOW_TITLE());

	/**	@brief Do all necessary initializations for the renderer to be ready to run (returns true in the case of success)
	*	@param[in] width The width of the window in pixels
	*	@param[in] height The height of the window in pixels
	*	@param[in] fullscreen Decides whether or not the application runs in full screen mode (optional)
	*	@param[in] windowTitle The title for the window on desktop systems (optional)
	*/
	bool initRenderer(GLint width, GLint height, bool fullscreen = false, std::string windowTitle = bRenderer::DEFAULT_WINDOW_TITLE());

	/**	@brief Start the render loop
	*/
	void runRenderer();

	/**	@brief Stop the render loop
	*/
	void stopRenderer();

	/**	@brief Terminate the renderer
	*/
	void terminateRenderer();

	/**	@brief Draw specified model into the current framebuffer
	*	@param[in] modelName Name of the model
	*	@param[in] cameraName Name of the camera
	*	@param[in] modelMatrix 
	*	@param[in] lightNames Names of the lights in a vector
	*	@param[in] doFrustumCulling Set true if the model should be tested against the view frustum (optional)
	*	@param[in] cullIndividualGeometry Set true if all the geometry should be tested against the view frustum (optional)
	*/
	void drawModel(const std::string &modelName, const std::string &cameraName, const vmml::Matrix4f &modelMatrix, const std::vector<std::string> &lightNames, bool doFrustumCulling = true, bool cullIndividualGeometry = false);

	/**	@brief Draw specified model into the current framebuffer
	*	@param[in] model The model to be drawn
	*	@param[in] modelMatrix
	*	@param[in] viewMatrix
	*	@param[in] projectionMatrix
	*	@param[in] lightNames Names of the lights in a vector
	*	@param[in] doFrustumCulling Set true if the model should be tested against the view frustum (optional)
	*	@param[in] cullIndividualGeometry Set true if all the geometry should be tested against the view frustum (optional)
	*/
	void drawModel(ModelPtr model, const vmml::Matrix4f &modelMatrix, const vmml::Matrix4f &viewMatrix, const vmml::Matrix4f &projectionMatrix, const std::vector<std::string> &lightNames, bool doFrustumCulling = true, bool cullIndividualGeometry = false);

	/**	@brief Queue specified model into the render queue of the renderer
	*	@param[in] modelName Name of the model
	*	@param[in] instanceName The name of the model instance to be queued (instance is created automatically if not present)
	*	@param[in] cameraName Name of the camera
	*	@param[in] modelMatrix
	*	@param[in] lightNames Names of the lights in a vector
	*	@param[in] doFrustumCulling Set true if the model should be tested against the view frustum (optional)
	*	@param[in] cullIndividualGeometry Set true if all the geometry should be tested against the view frustum (optional)
	*	@param[in] isTransparent Set true if the model is (partially) transparent and sorting according to distance should occur (optional)
	*	@param[in] blendSfactor Specifies how the red, green, blue, and alpha source blending factors are computed (optional)
	*	@param[in] blendDfactor Specifies how the red, green, blue, and alpha destination blending factors are computed (optional)
	*	@param[in] customDistance If the function should not determine the distance to the camera a custom distance can be set (optional)
	*/
	void queueModelInstance(const std::string &modelName, const std::string &instanceName, const std::string &cameraName, const vmml::Matrix4f &modelMatrix, const std::vector<std::string> &lightNames, bool doFrustumCulling = true, bool cullIndividualGeometry = false, bool isTransparent = false, GLenum blendSfactor = GL_SRC_ALPHA, GLenum blendDfactor = GL_ONE_MINUS_SRC_ALPHA, GLfloat customDistance = 10000.0f);

	/**	@brief Queue specified model into the render queue of the renderer
	*	@param[in] model The model to be drawn
	*	@param[in] instanceName The name of the model instance to be queued (instance is created automatically if not present)
	*	@param[in] modelMatrix
	*	@param[in] viewMatrix
	*	@param[in] projectionMatrix
	*	@param[in] lightNames Names of the lights in a vector
	*	@param[in] doFrustumCulling Set true if the model should be tested against the view frustum (optional)
	*	@param[in] cullIndividualGeometry Set true if all the geometry should be tested against the view frustum (optional)
	*	@param[in] isTransparent Set true if the model is (partially) transparent and sorting according to distance should occur (optional)
	*	@param[in] blendSfactor Specifies how the red, green, blue, and alpha source blending factors are computed (optional)
	*	@param[in] blendDfactor Specifies how the red, green, blue, and alpha destination blending factors are computed (optional)
	*	@param[in] customDistance If the function should not determine the distance to the camera a custom distance can be set (optional)
	*/
	void queueModelInstance(ModelPtr model, const std::string &instanceName, const vmml::Matrix4f &modelMatrix, const vmml::Matrix4f &viewMatrix, const vmml::Matrix4f &projectionMatrix, const std::vector<std::string> &lightNames, bool doFrustumCulling = true, bool cullIndividualGeometry = false, bool isTransparent = false, GLenum blendSfactor = GL_SRC_ALPHA, GLenum blendDfactor = GL_ONE_MINUS_SRC_ALPHA, GLfloat customDistance = 10000.0f);

	/**	@brief Draw specified text sprite into the current framebuffer
	*	@param[in] textSpriteName Name of the text sprite
	*	@param[in] cameraName Name of the camera
	*	@param[in] modelMatrix
	*	@param[in] lightNames Names of the lights in a vector
	*	@param[in] doFrustumCulling Set true if the text sprite should be tested against the view frustum (optional)
	*/
	void drawText(const std::string &textSpriteName, const std::string &cameraName, const vmml::Matrix4f &modelMatrix, const std::vector<std::string> &lightNames, bool doFrustumCulling = false);

	/**	@brief Queue specified text sprite into the render queue of the renderer
	*	@param[in] textSpriteName Name of the text sprite
	*	@param[in] instanceName The name of the text sprite instance to be queued (instance is created automatically if not present)
	*	@param[in] cameraName Name of the camera
	*	@param[in] modelMatrix
	*	@param[in] lightNames Names of the lights in a vector
	*	@param[in] doFrustumCulling Set true if the text sprite should be tested against the view frustum (optional)
	*	@param[in] blendSfactor Specifies how the red, green, blue, and alpha source blending factors are computed (optional)
	*	@param[in] blendDfactor Specifies how the red, green, blue, and alpha destination blending factors are computed (optional)
	*	@param[in] customDistance If the function should not determine the distance to the camera a custom distance can be set (optional)
	*/
	void queueTextInstance(const std::string &textSpriteName, const std::string &instanceName, const std::string &cameraName, const vmml::Matrix4f &modelMatrix, const std::vector<std::string> &lightNames, bool doFrustumCulling = true, GLenum blendSfactor = GL_SRC_ALPHA, GLenum blendDfactor = GL_ONE_MINUS_SRC_ALPHA, GLfloat customDistance = 10000.0f);

	/**	@brief Draws the render queue into the current framebuffer
	*	@param[in] mode
	*/
	void drawQueue(GLenum mode = GL_TRIANGLES);

	/**	@brief Clears the render queue of the renderer
	*/
	void clearQueue();

	/**	@brief Tests an axis-aligned bounding box against the view frustum
	*	@param[in] aabbObjectSpace The axis-aligned bounding box in object space
	*	@param[in] modelViewProjectionMatrix The model view projection matrix (projection * view * model)
	*/
	vmml::Visibility viewFrustumCulling(const vmml::AABBf &aabbObjectSpace, const vmml::Matrix4f &modelViewProjectionMatrix);

private:
	/* Functions */

    /**	@brief Update the scene
     */
    void update();
    
	/**	@brief Reset all variables
	*/
	void reset();

	/* Variables */

	bool _initialized = false;
	bool _running = false;

	double _elapsedTime = 0.0, _stopTime = 0.0, _initialTime = 0.0;

	ViewPtr				_view = nullptr;
	InputPtr			_input = nullptr;
	ObjectManagerPtr	_objectManager = nullptr;
	RenderQueuePtr		_renderQueue = nullptr;

	IRenderProject *_renderProject = nullptr;
    
	RendererCaller *_rendererCaller = nullptr;
	
	void(*_initFunction)() = nullptr;
	void(*_loopFunction)(const double deltaTime, const double elapsedTime) = nullptr;
	void(*_terminateFunction)() = nullptr;

};

typedef std::shared_ptr< Renderer >  RendererPtr;

#endif /* defined(B_RENDERER_H) */
