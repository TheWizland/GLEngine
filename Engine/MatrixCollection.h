#pragma once
#include <GLM/mat4x4.hpp>
#include <vector>

//Node Graph Data
class MatrixCollection
{
private:
	glm::mat4 localTranslation = glm::mat4(1.f);
	glm::mat4 localRotation = glm::mat4(1.f);;
	glm::mat4 localScale = glm::mat4(1.f);;
	glm::mat4 worldTranslation;
	glm::mat4 worldRotation;
	glm::mat4 worldScale;
	glm::mat4 model;
	MatrixCollection *parentMatrix = NULL;
	std::vector<MatrixCollection*> children;
	
	void updateModel();
protected:
	void addChild(MatrixCollection* childMatrix);
	void removeChild(MatrixCollection* childMatrix);
public:
	bool inheritTranslation = true;
	bool inheritRotation = true;
	bool inheritScale = true;
	bool applyParentRotationToPosition = false;
	bool applyParentScaleToPosition = true;

	void setParent(MatrixCollection* newParent);
	glm::mat4 getModel();
	glm::mat4 getInverseTranspose();
	void setLocalTranslation(glm::mat4 localTranslation);
	void setLocalRotation(glm::mat4 localRotation);
	void setLocalScale(glm::mat4 localScale);
	glm::mat4 getLocalTranslation() { return localTranslation; }
	glm::mat4 getWorldTranslation() { return worldTranslation; }
	glm::mat4 getLocalRotation() { return localRotation; }
	glm::mat4 getWorldRotation() { return worldRotation; }
	glm::mat4 getLocalScale() { return localScale; }
	glm::mat4 getWorldScale() { return worldScale; }

};

