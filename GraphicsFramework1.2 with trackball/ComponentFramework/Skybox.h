#pragma once
#include "Model.h"
#include <GL/glew.h>





namespace GAME {
	class Skybox : public Model
	{

	private:
		const char * positive_x_image;
		const char * negative_x_image;
		const char * positive_y_image;
		const char * negative_y_image;
		const char * positive_z_image;
		const char * negative_z_image;
		class Shader *skyBoxShader;
		GLuint textureID;
		GLuint texture;
		GLuint projectionMatrixID;
		GLuint modelViewMatrixID;

	public:
		Skybox(const char * positive_x_image,
			const char * negative_x_image,
			const char * positive_y_image,
			const char * negative_y_image,
			const char * positive_z_image,
			const char * negative_z_image);
	  virtual ~Skybox();

	  virtual bool OnCreate();
	  virtual void OnDestroy();
	  virtual void Update(const float time);
	  virtual void Render(const Matrix4& projectionMatrix, const Matrix4& identityMatrix, const Matrix4& normalMatrix) const;

	};

}

