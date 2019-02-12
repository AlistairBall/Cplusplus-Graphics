#include <GL/glew.h>
#include <SDL.h>
#include <iostream>
#include "MMath.h"
#include "Window.h"
#include "Shader.h"
#include "Scene1.h"
#include "SceneEnvironment.h"
#include "Trackball.h"
#include "Skybox.h"

#include "ObjLoader.h"
#include <map>

using namespace GAME;
using namespace MATH;

Matrix4 model;


Scene1::Scene1(Window& windowRef):Scene(windowRef) {

}


bool Scene1::OnCreate() {
	
	camera = nullptr;
	window = windowPtr->getSDLWindow();
	
	/// Load Assets: as needed 

	//for (Model* model : models) {
	if (addModel("sphere.obj") == false) {
		return false;
	}


	skull = new Model(Vec3(0.0f, 0.0f, 0.0f), Vec3(0.0f, 0.0f, 0.0f));
	skull->setPos(Vec3(0.0f, 0.0f, 0.0f));
	//skull->DisableDepth();
	skull->OnCreate();

	if (skull->LoadMesh("skull.obj") == false) {
		return false;
	}

	/// Create a shader with attributes
	SceneEnvironment::getInstance()->setLight(Vec3(0.0f, 3.0f, 7.0f));

	//add illumnation for shadows

	OnResize(windowPtr->getWidth(), windowPtr->getHeight());
	return true;

	std::map<float, Vec3> sorted;
	//for (unsigned int i = 0; i < windowPtr->setWindowSize; i++)
	//{
	//	//float distance = glm::length(camera.Position - windows[i]);
	//	float distance = std::distance(camera.);
	//		sorted[distance] = windows[i];
	//}
}


bool GAME::Scene1::addModel(const char* filename)
{
	skyBox = new Skybox("posx.jpg", "negx.jpg", "posy.jpg", "negy.jpg", "posz.jpg", "negz.jpg");

	skyBox->OnCreate();
	
	sphere = new Model(Vec3(0.0f, 0.0f, 0.0f), Vec3(0.0f, 0.0f, 0.0f));
	sphere->setPos(Vec3(-3.0f, 0.0f, 0.0f));
	//sphere->EnableDepth();
	sphere->OnCreateNoise();

	if (sphere->LoadMesh(filename) == false) {
		return false;
	}

	return true;
}

void Scene1::OnResize(int w_, int h_){
	windowPtr->setWindowSize(w_,h_);
	glViewport(0,0,windowPtr->getWidth(),windowPtr->getHeight());
	if (camera) delete camera;
	camera = new Camera(w_, h_, Vec3(0.0f, 0.0f, 10.0f));
	Camera::currentCamera = camera;
	Trackball::getInstance()->setWindowDimensions(windowPtr->getWidth(), windowPtr->getHeight());
}

void Scene1::Update(const float deltaTime) {
	skull->Update(deltaTime);
	sphere->Update(deltaTime);
	skyBox->Update(deltaTime);

	
}

void Scene1::Render() const{
	//glDisable(GL_DEPTH_TEST);
	glEnable(GL_DEPTH_TEST);
	//glEnable(GL_CULL_FACE);
	glDisable(GL_CULL_FACE);
	glClear( GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT );
	
	/// Draw your scene here
	//for (Model* model : models) {
	//	model->Render();
	//}
	

	skyBox->Render(camera->getProjectionMatrix(),  Trackball::getInstance()->getMatrix4(), Matrix4(1.0f));
	skull->Render();
	sphere->Render();
	SDL_GL_SwapWindow(windowPtr->getSDLWindow());



	/*for (std::map<float, MATH::Vec3>::reverse_iterator it = sorted.rbegin(); it != sorted.rend(); ++it)
	{
		
		
		model = glm::translate(model, it->second);
		shader.setMat4("model", model);
		glDrawArrays(GL_TRIANGLES, 0, 6);
	}
	*/
}

void Scene1::HandleEvents(const SDL_Event& SDLEvent){
	if (SDLEvent.type == SDL_EventType::SDL_MOUSEBUTTONDOWN) {
		Trackball::getInstance()->onLeftMouseDown(SDLEvent.button.x, SDLEvent.button.y);
	}else if(SDLEvent.type == SDL_EventType::SDL_MOUSEBUTTONUP){
		Trackball::getInstance()->onLeftMouseUp(SDLEvent.button.x, SDLEvent.button.y);
	}else if (SDLEvent.type == SDL_EventType::SDL_MOUSEMOTION && 
		SDL_GetMouseState(NULL, NULL) & SDL_BUTTON(SDL_BUTTON_LEFT)) {
		Trackball::getInstance()->onMouseMove(SDLEvent.button.x, SDLEvent.button.y);
	}
		
}

Scene1::~Scene1() {
	OnDestroy();
}

void Scene1::OnDestroy() {
	/// Cleanup Assets
	if (camera) delete camera;
	for (Model* model : models) {
		if (model) delete model;
	}
}
