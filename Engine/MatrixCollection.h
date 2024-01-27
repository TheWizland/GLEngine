#pragma once
#include <GLM/mat4x4.hpp>
#include <vector>

//Node Graph Data
class MatrixCollection
{
private:
	glm::mat4 localTranslation = glm::mat4(1.f);
	glm::mat4 localRotation = glm::mat4(1.f);
	glm::mat4 localScale = glm::mat4(1.f);
	glm::mat4 worldTranslation = glm::mat4(1.f);
	glm::mat4 worldRotation = glm::mat4(1.f);
	glm::mat4 worldScale = glm::mat4(1.f);
	glm::mat4 model = glm::mat4(1.f);
	MatrixCollection* parentMatrix = NULL;
	std::vector<MatrixCollection*> children;
	bool inheritTranslation = true;
	bool inheritRotation = true;
	bool inheritScale = true;
	bool applyParentRotationToPosition = true;
	bool applyParentScaleToPosition = true;
protected:
	void addChild(MatrixCollection* childMatrix);
	void removeChild(MatrixCollection* childMatrix);
	void updateModel();
public:
	void setParent(MatrixCollection* newParent);
	void removeParent();
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
	void translate(glm::vec3 distance);
	void translate(float x, float y, float z);
	void rotate(float distance, glm::vec3 angle);
	void rotateX(float x);
	void rotateY(float y);
	void rotateZ(float z);
	void scale(float amount);
	void scale(float x, float y, float z);
	void setInheritTranslation(bool willInherit) { inheritTranslation = willInherit; updateModel(); }
	void setInheritRotation(bool willInherit) { inheritRotation = willInherit; updateModel(); }
	void setInheritScale(bool willInherit) { inheritScale = willInherit; updateModel(); }
	void setApplyParentRotationToPosition(bool willApply) { applyParentRotationToPosition = willApply; updateModel(); }
	void setApplyParentScaleToPosition(bool willApply) { applyParentScaleToPosition = willApply; updateModel(); }
	~MatrixCollection();
};

