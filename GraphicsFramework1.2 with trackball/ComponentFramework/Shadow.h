#ifndef SHADOW_H
#define SHADOW_H

#include "Model.h"
#include <GL/glew.h>


class Shadow
{
public:
	Shadow();
	~Shadow();


	bool Init(unsigned int WindowWidth, unsigned int WindowHeight);

	void BindForWriting();

	void BindForReading(GLenum TextureUnit);

private:
	GLuint m_fbo;
	GLuint m_shadowMap;


private:



};

#endif // !SHADOW_H

