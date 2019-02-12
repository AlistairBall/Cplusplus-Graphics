#include "Model.h"
#include "Shader.h"
#include "ObjLoader.h"
#include "Camera.h"
#include "SceneEnvironment.h"
#include "Trackball.h"
#include "Noise.h"
#include <map>


namespace GAME {

	Model::Model() {

	}

	Model::Model(const Vec3 pos_, const Vec3 orientation_) {
		pos = pos_;
		orientation = orientation_;
		shader = nullptr;
	}

	Model::~Model() {
		OnDestroy();
	}

	void Model::setPos(const Vec3& pos_) {
		Entity::setPos(pos_);
		updateModelMatrix();
	}

	void Model::setOrientation(const Vec3& orientation_) {
		Entity::setOrientation(orientation_);
		updateModelMatrix();
	}

	void Model::updateModelMatrix(){
		modelMatrix = MMath::translate(pos);

		/// This transform is based on Euler angles - let's do it later
		//modelMatrix = MMath::translate(pos) * MMath::rotate(orientation.z, Vec3(0.0f, 0.0f, 1.0f)) * MMath::rotate(orientation.x, Vec3(1.0f, 0.0f, 0.0f)) * MMath::rotate(orientation.y, Vec3(0.0f, 1.0f, 0.0f));
	}

	bool Model::OnCreate() {
		shader = new Shader("ToonVert.glsl", "ToonFrag.glsl", 3, 0, "vVertex", 1, "vNormal", 2, "texCoords");
		

		return true;
	}

	bool Model::OnCreateNoise() {
		shader = new Shader("NoiseVert.glsl", "reflectionFrag.glsl", 3, 0, "vVertex", 1, "vNormal", 2, "texCoords");
		elapsedTime = 0.0f;
		noise = CreateNoise3D();

		noiseID = glGetUniformLocation(shader->getProgram(), "noiseTexture");

		glUniform1i(noise, noiseID);

		return true;
	}

	

	bool Model::LoadMesh(const char* filename) {
		if (ObjLoader::loadOBJ(filename) == false) {
			return false;
		}
		/// Get the data out of the ObjLoader and into our own mesh
		meshes.push_back(new Mesh(GL_TRIANGLES, ObjLoader::vertices, ObjLoader::normals, ObjLoader::uvCoords));
		return true;
	}


	void Model::Update(const float deltaTime) {
		/// See Entity.h
		///Rotate(Vec3(0.0f, 50.0f * deltaTime, 0.0f));
		elapsedTime += deltaTime;
		
	}

	void Model::DisableDepth()
	{
		glDisable(GL_DEPTH_TEST);
	}

	void Model::EnableDepth()
	{
		glEnable(GL_DEPTH_TEST);
	}

	void Model::Render() const {

		GLint projectionMatrixID = glGetUniformLocation(shader->getProgram(), "projectionMatrix");
		GLint viewMatrixID = glGetUniformLocation(shader->getProgram(), "viewMatrix");
		GLint modelMatrixID = glGetUniformLocation(shader->getProgram(), "modelMatrix");
		GLint normalMatrixID = glGetUniformLocation(shader->getProgram(), "normalMatrix");
		GLint lightPosID = glGetUniformLocation(shader->getProgram(), "lightPos");
		GLint elapsedTimeID = glGetUniformLocation(shader->getProgram(), "elapsedTime");

		glUseProgram(shader->getProgram());

		glUniformMatrix4fv(projectionMatrixID, 1, GL_FALSE, Camera::currentCamera->getProjectionMatrix());
		glUniformMatrix4fv(viewMatrixID, 1, GL_FALSE, Camera::currentCamera->getViewMatrix());
		glUniformMatrix4fv(modelMatrixID, 1, GL_FALSE, modelMatrix * Trackball::getInstance()->getMatrix4() );

		/// Assigning the 4x4 modelMatrix to the 3x3 normalMatrix 
		/// copies just the upper 3x3 of the modelMatrix
		Matrix3 normalMatrix = modelMatrix * Trackball::getInstance()->getMatrix4();
		glUniformMatrix3fv(normalMatrixID, 1, GL_FALSE, normalMatrix);

		glUniform3fv(lightPosID, 1, SceneEnvironment::getInstance()->getLight());
		glUniform1f(elapsedTimeID, elapsedTime);

		glEnable(GL_DEPTH_TEST);
		//glEnable(GL_CULL_FACE);
		glDisable(GL_CULL_FACE);
		glEnable(GL_BLEND);
		glEnable(GL_BLEND);
		glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
		glBindFramebuffer(GL_FRAMEBUFFER, 0);
		glDrawBuffer(GL_BACK_LEFT);

		glBindTexture(GL_TEXTURE, texture);
		glActiveTexture(GL_TEXTURE0);
		glBindTexture(GL_TEXTURE, noise);
	

		for (Mesh* mesh : meshes) {
			mesh->Render();
		}


	}
	void Model::OnDestroy() {
		if (shader) delete shader;
	}
}