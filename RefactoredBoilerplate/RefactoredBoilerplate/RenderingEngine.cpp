/*
 * RenderingEngine.cpp
 *
 *  Created on: Sep 10, 2018
 *      Author: John Hall
 */

#include "RenderingEngine.h"

#include <iostream>

//cpp file purposely included here because it just contains some global functions
#include "ShaderTools.h"
#include "global.h"
#include "Scene.h"

RenderingEngine::RenderingEngine() {
	shaderProgram = ShaderTools::InitializeShaders();
	if (shaderProgram == 0) {
		std::cout << "Program could not initialize shaders, TERMINATING" << std::endl;
		return;
	}
}

RenderingEngine::~RenderingEngine() {

}
void RenderingEngine::RenderScene(const std::vector<Geometry>& objects) {
	//Clears the screen to a dark grey background
	glClearColor(0.2f, 0.2f, 0.2f, 1.0f);
	glClear(GL_COLOR_BUFFER_BIT);

	// bind our shader program and the vertex array object containing our
	// scene geometry, then tell OpenGL to draw our geometry
	glUseProgram(shaderProgram);

	GLint loc = glGetUniformLocation(shaderProgram, "scale");
	glUniform1f(loc, 0.35);
	if (linearTracker == 1) {
		GLint loc = glGetUniformLocation(shaderProgram, "scale");
		glUniform1f(loc, 0.2);
		GLint linearLevel = glGetUniformLocation(shaderProgram, "mode");
		glUniform1i(linearLevel, 0);
		glPatchParameteri(GL_PATCH_VERTICES, 2);
	}
	if (mode == 0) {
		GLint loc = glGetUniformLocation(shaderProgram, "scale");
		glUniform1f(loc, 0.2);
		GLint linearLevel = glGetUniformLocation(shaderProgram, "mode");
		glUniform1i(linearLevel, 0);
		glPatchParameteri(GL_PATCH_VERTICES, 2);
	}
	if (mode == 1) {
		glUniform1f(loc, 0.35);
		GLint modeLevel = glGetUniformLocation(shaderProgram, "mode");
		glUniform1i(modeLevel, 1);
		glPatchParameteri(GL_PATCH_VERTICES, 3);
	}
	if (mode == 2) {
		GLint scaleLevel = glGetUniformLocation(shaderProgram, "scale");
		glUniform1f(scaleLevel, 0.09);
		GLint modeLevel = glGetUniformLocation(shaderProgram, "mode");
		glUniform1i(modeLevel, 2);
		glPatchParameteri(GL_PATCH_VERTICES, 4);
		//std::cout << "Here" << std::endl;
	}
	if (mode == 3) {
		glUniform1f(loc, 0.2);
		GLint modeLevel = glGetUniformLocation(shaderProgram, "mode");
		glUniform1i(modeLevel, 3);
		glPatchParameteri(GL_PATCH_VERTICES, 3);
	}
	if (mode == 4) {
		glUniform1f(loc, 0.2);
		GLint modeLevel = glGetUniformLocation(shaderProgram, "mode");
		glUniform1i(modeLevel, 4);
		glPatchParameteri(GL_PATCH_VERTICES, 4);
	}
	if (mode == 5) {
		glUniform1f(loc, 0.1);
		GLint modeLevel = glGetUniformLocation(shaderProgram, "mode");
		glUniform1i(modeLevel, 3);
		glPatchParameteri(GL_PATCH_VERTICES, 4);
	}
	if (mode == 6) {
		GLint loc = glGetUniformLocation(shaderProgram, "scale");
		GLint scrollback = glGetUniformLocation(shaderProgram, "scrollOffset");
		glUniform2f(scrollback, offsetX, 0);
		
		/* Here you can just update offsetX also... */
		offsetX += 0.01;  // I don't know what the scale is and how much a '1' unit is...
		if( offsetX > 500 ) { // 500 should be some #define or const int value.... that is 'well... 
			              // if you saved the very last offsetX from setAlex, that would be the length....
			              // x > length
			
		     offsetX = 0.0;
		}
		
		glUniform1f(loc, 0.2);
		GLint linearLevel = glGetUniformLocation(shaderProgram, "mode");
		glUniform1i(linearLevel, 0);
		glPatchParameteri(GL_PATCH_VERTICES, 2);
	}
	for (const Geometry& g : objects) {
		if (g.type == 1) {
			//then it is a line 
			GLint linearLevel = glGetUniformLocation(shaderProgram, "mode");
			glUniform1i(linearLevel, 0);
			glPatchParameteri(GL_PATCH_VERTICES, 2);
		}
		if (g.type == 2) {
			//then it is a quad 
			GLint modeLevel = glGetUniformLocation(shaderProgram, "mode");
			glUniform1i(modeLevel, 3);
			glPatchParameteri(GL_PATCH_VERTICES, 3);
		}
		if (g.type == 3) {
			//then it is a cubic 
			GLint modeLevel = glGetUniformLocation(shaderProgram, "mode");
			glUniform1i(modeLevel, 2);
			glPatchParameteri(GL_PATCH_VERTICES, 4);
		}
		glBindVertexArray(g.vao);
		glDrawArrays(g.drawMode, 0, g.verts.size());

		// reset state to default (no shader or geometry bound)
		glBindVertexArray(0);
	}
	glUseProgram(0);

	// check for an report any OpenGL errors
	CheckGLErrors();
}

void RenderingEngine::assignBuffers(Geometry& geometry) {
	//Generate vao for the object
	//Constant 1 means 1 vao is being generated
	glGenVertexArrays(1, &geometry.vao);
	glBindVertexArray(geometry.vao);

	//Generate vbos for the object
	//Constant 1 means 1 vbo is being generated
	glGenBuffers(1, &geometry.vertexBuffer);
	glBindBuffer(GL_ARRAY_BUFFER, geometry.vertexBuffer);
	//Parameters in order: Index of vbo in the vao, number of primitives per element, primitive type, etc.
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 0, (void*)0);
	glEnableVertexAttribArray(0);

	/*glGenBuffers(1, &geometry.normalBuffer);
	glBindBuffer(GL_ARRAY_BUFFER, geometry.normalBuffer);
	glVertexAttribPointer(2, 3, GL_FLOAT, GL_FALSE, 0, (void*)0);
	glEnableVertexAttribArray(2);*/

	glGenBuffers(1, &geometry.colorBuffer);
	glBindBuffer(GL_ARRAY_BUFFER, geometry.colorBuffer);
	//Parameters in order: Index of vbo in the vao, number of primitives per element, primitive type, etc.
	glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 0, (void*)0);
	glEnableVertexAttribArray(1);

	/*glGenBuffers(1, &geometry.uvBuffer);
	glBindBuffer(GL_ARRAY_BUFFER, geometry.uvBuffer);
	glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, 0, (void*)0);
	glEnableVertexAttribArray(1);*/


}

void RenderingEngine::setBufferData(Geometry& geometry) {
	//Send geometry to the GPU
	//Must be called whenever anything is updated about the object
	glBindBuffer(GL_ARRAY_BUFFER, geometry.vertexBuffer);
	glBufferData(GL_ARRAY_BUFFER, sizeof(glm::vec3) * geometry.verts.size(), geometry.verts.data(), GL_STATIC_DRAW);

	/*glBindBuffer(GL_ARRAY_BUFFER, geometry.normalBuffer);
	glBufferData(GL_ARRAY_BUFFER, sizeof(glm::vec3) * geometry.normals.size(), geometry.normals.data(), GL_STATIC_DRAW);*/

	glBindBuffer(GL_ARRAY_BUFFER, geometry.colorBuffer);
	glBufferData(GL_ARRAY_BUFFER, sizeof(glm::vec3) * geometry.colors.size(), geometry.colors.data(), GL_STATIC_DRAW);

	//glBindBuffer(GL_ARRAY_BUFFER, geometry.uvBuffer);
	//glBufferData(GL_ARRAY_BUFFER, sizeof(glm::vec2) * geometry.uvs.size(), geometry.uvs.data(), GL_STATIC_DRAW);
}

void RenderingEngine::deleteBufferData(Geometry& geometry) {
	glDeleteBuffers(1, &geometry.vertexBuffer);
	glDeleteBuffers(1, &geometry.normalBuffer);
	glDeleteBuffers(1, &geometry.colorBuffer);
	glDeleteBuffers(1, &geometry.uvBuffer);
	glDeleteVertexArrays(1, &geometry.vao);
}

bool RenderingEngine::CheckGLErrors() {
	bool error = false;
	for (GLenum flag = glGetError(); flag != GL_NO_ERROR; flag = glGetError())
	{
		std::cout << "OpenGL ERROR:  ";
		switch (flag) {
		case GL_INVALID_ENUM:
			std::cout << "GL_INVALID_ENUM" << std::endl; break;
		case GL_INVALID_VALUE:
			std::cout << "GL_INVALID_VALUE" << std::endl; break;
		case GL_INVALID_OPERATION:
			std::cout << "GL_INVALID_OPERATION" << std::endl; break;
		case GL_INVALID_FRAMEBUFFER_OPERATION:
			std::cout << "GL_INVALID_FRAMEBUFFER_OPERATION" << std::endl; break;
		case GL_OUT_OF_MEMORY:
			std::cout << "GL_OUT_OF_MEMORY" << std::endl; break;
		default:
			std::cout << "[unknown error code]" << std::endl;
		}
		error = true;
	}
	return error;
}
