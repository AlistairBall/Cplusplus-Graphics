#ifndef MODEL_H
#define MODEL_H
#include <vector>

#include "MMath.h"
#include "Mesh.h"
#include "Entity.h"

namespace GAME {

using namespace MATH;

class Model : public Entity {

protected:

	class Shader *shader;

	Matrix4 modelMatrix;
	std::vector<Mesh*> meshes;

public:

	Model();
	Model(const Vec3 _pos, const Vec3 _orientation);
	Model(const Model&) = delete;
	Model(Model&&) = delete;
	Model& operator = (const Model&) = delete;
	Model& operator = (Model&&) = delete;

	virtual ~Model();

	void setPos(const Vec3& pos_) override;
	void setOrientation(const Vec3& orienration_) override;

	virtual bool OnCreate();
	virtual bool OnCreateNoise();
	virtual void OnDestroy();
	virtual void Render() const;
	virtual void Update(const float deltaTime);
	virtual void DisableDepth();
	virtual void EnableDepth();

	virtual bool LoadMesh(const char* filename);

protected:

	void updateModelMatrix();
	float elapsedTime;
	GLuint noiseID;
	GLuint noise;
private:

	unsigned int textureID[2];
	GLuint texture;

};
} /// end of namespace

#endif