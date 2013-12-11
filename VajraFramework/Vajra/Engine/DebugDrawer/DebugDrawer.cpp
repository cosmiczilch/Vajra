#include "Vajra/Engine/Components/DerivedComponents/Transform/Transform.h"
#include "Vajra/Engine/Core/Engine.h"
#include "Vajra/Engine/GameObject/GameObject.h"
#include "Vajra/Framework/Core/Framework.h"
#include "Vajra/Engine/DebugDrawer/DebugDrawer.h"
#include "Vajra/Framework/Core/Framework.h"
#include "Vajra/Framework/OpenGL/OpenGLWrapper/OpenGLWrapper.h"
#include "Vajra/Framework/OpenGL/ShaderSet/ShaderSet.h"
#include "Vajra/Utilities/OpenGLIncludes.h"


DebugDrawer::DebugDrawer() {
	this->init();
}

DebugDrawer::~DebugDrawer() {
	this->destroy();
}

void DebugDrawer::init() {
	this->identityTransform = new Transform();
	this->generalPurposeTransform = new Transform();
}

void DebugDrawer::destroy() {
	if (this->identityTransform != nullptr) {
		delete this->identityTransform;
	}
	if (this->generalPurposeTransform != nullptr) {
		delete this->generalPurposeTransform;
	}
}

void DebugDrawer::drawPoint(glm::vec3 position) {
	FRAMEWORK->GetOpenGLWrapper()->SetCurrentShaderSet("simshdr");

	this->identityTransform->Draw();

	/*
	glBegin(GL_POINTS);
	glVertex3f(position.x, position.y, position.z);
	glEnd();
	 */
}

void DebugDrawer::drawCube(glm::vec3 position, float size) {
	FRAMEWORK->GetOpenGLWrapper()->SetCurrentShaderSet("simshdr");

	this->generalPurposeTransform->SetPosition(position);
	this->generalPurposeTransform->SetScale(1.0f, 1.0f, 1.0f);
	this->generalPurposeTransform->Scale(size);

	this->generalPurposeTransform->Draw();

	/*
	glBegin(GL_LINE_LOOP);

	glVertex3f(0.5f, -0.5f, 0.5f);
	glVertex3f(0.5f, -0.5f, -0.5f);
	glVertex3f(0.5f, 0.5f, -0.5f);
	glVertex3f(0.5f, 0.5f, 0.5f);
	glVertex3f(0.5f, -0.5f, 0.5f);

	glVertex3f(-0.5f, -0.5f, 0.5f);
	glVertex3f(-0.5f, -0.5f, -0.5f);
	glVertex3f(-0.5f, 0.5f, -0.5f);
	glVertex3f(-0.5f, 0.5f, 0.5f);
	glVertex3f(-0.5f, -0.5f, 0.5f);

	glVertex3f(-0.5f, 0.5f, 0.5f);
	glVertex3f(0.5f, 0.5f, 0.5f);
	glVertex3f(0.5f, 0.5f, -0.5f);
	glVertex3f(-0.5f, 0.5f, -0.5f);
	glVertex3f(-0.5f, -0.5f, -0.5f);
	glVertex3f(0.5f, -0.5f, -0.5f);

	glEnd();
	 */
}

void DebugDrawer::drawLine(glm::vec3 from, glm::vec3 to) {
	FRAMEWORK->GetOpenGLWrapper()->SetCurrentShaderSet("simshdr");
	ShaderSet* currentShaderSet = FRAMEWORK->GetOpenGLWrapper()->GetCurrentShaderSet();

	this->identityTransform->Draw();
	
	GLfloat vertices[] = {
		from.x, from.y, from.z,
		to.x, to.y, to.z
	};
	
	GLuint vbo;
	glGenBuffers(1, &vbo); checkGlError("glGenBuffers");
	glBindBuffer(GL_ARRAY_BUFFER, vbo); checkGlError("glBindBuffer");
	glBufferData(GL_ARRAY_BUFFER, sizeof(glm::vec3) * 2, vertices, GL_STATIC_DRAW); checkGlError("glBufferData");
	
	GLint positionHandle = currentShaderSet->GetHandle(SHADER_VARIABLE_VARIABLENAME_vPosition);
    glEnableVertexAttribArray(positionHandle);
    glBindBuffer(GL_ARRAY_BUFFER, vbo); checkGlError("glBindBuffer");
    glVertexAttribPointer(positionHandle,
                          3, GL_FLOAT, GL_FALSE, 0, 0);
    checkGlError("glVertexAttribPointer");
	glDrawArrays(GL_LINES, 0, 2);
	
	/*
	glEnableClientState(GL_VERTEX_ARRAY);
	glVertexPointer(3, GL_FLOAT, 0, vertices);
	glDrawArrays(GL_TRIANGLES, 0, 3);
	glDisableClientState(GL_VERTEX_ARRAY);

	glBegin(GL_LINES);
	glVertex3f(from.x, from.y, from.z);
	glVertex3f(to.x, to.y, to.z);
	glEnd();
	 */
}

void DebugDrawer::drawArrow(glm::vec3 from, glm::vec3 to) {
	this->drawLine(from, to);
	this->drawCube(to, 0.04f);
}

////////////////////////////////////////////////////////////////////////////////

void DebugDraw::DrawPoint(glm::vec3 position) {
	ENGINE->GetDebugDrawer()->drawPoint(position);
}

void DebugDraw::DrawCube(glm::vec3 position, float size) {
	ENGINE->GetDebugDrawer()->drawCube(position, size);
}

void DebugDraw::DrawLine(glm::vec3 from, glm::vec3 to) {
	ENGINE->GetDebugDrawer()->drawLine(from, to);
}

void DebugDraw::DrawLine(glm::vec3 from, glm::vec3 direction, float length) {
	ENGINE->GetDebugDrawer()->drawLine(from, from + direction * length);
}

void DebugDraw::DrawArrow(glm::vec3 from, glm::vec3 to) {
	ENGINE->GetDebugDrawer()->drawArrow(from, to);
}

void DebugDraw::DrawArrow(glm::vec3 from, glm::vec3 direction, float length) {
	ENGINE->GetDebugDrawer()->drawArrow(from, from + direction * length);
}
