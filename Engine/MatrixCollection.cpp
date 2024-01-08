#include "MatrixCollection.h"
#include <GLM/gtc/matrix_transform.hpp>
#include <GLM/gtc/matrix_inverse.hpp>
#include <algorithm>

void MatrixCollection::updateModel()
{
	worldTranslation = glm::mat4(1.f);
	worldRotation = glm::mat4(1.f);
	worldScale = glm::mat4(1.f);
	glm::vec4 location = glm::vec4(0.f, 0.f, 0.f, 1.f);
	location *= localTranslation;

	if (parentMatrix) 
	{
		if (inheritTranslation) 
		{
			if (applyParentRotationToPosition) 
			{
				location *= parentMatrix->getWorldRotation();
			}
			if (applyParentScaleToPosition) 
			{
				location *= parentMatrix->getWorldScale();
			}
			location *= parentMatrix->getWorldTranslation();
		}

		if (inheritRotation) 
		{
			worldRotation *= parentMatrix->getWorldRotation();
		}
		if (inheritScale) 
		{
			worldScale *= parentMatrix->getWorldScale();
		}
	}
	
	worldTranslation = glm::translate(worldTranslation, glm::vec3(location));
	worldRotation *= localRotation;
	worldScale *= localScale;

	for (MatrixCollection* child : children) {
		child->updateModel();
	}
}

void MatrixCollection::setParent(MatrixCollection* newParent)
{
	if (parentMatrix)
		parentMatrix->removeChild(this);
	parentMatrix = newParent;
	parentMatrix->addChild(this);
}

void MatrixCollection::addChild(MatrixCollection* childMatrix)
{
	children.push_back(childMatrix);
}

void MatrixCollection::removeChild(MatrixCollection* childMatrix)
{
	std::remove(children.begin(), children.end(), childMatrix);
}

glm::mat4 MatrixCollection::getModel()
{
	model = glm::mat4(1.f) * worldTranslation * worldRotation * worldScale;
	return model;
}

glm::mat4 MatrixCollection::getInverseTranspose()
{
	return glm::inverseTranspose(getModel());
}

void MatrixCollection::setLocalTranslation(glm::mat4 localTranslation)
{
	this->localTranslation = localTranslation;
	updateModel();
}

void MatrixCollection::setLocalRotation(glm::mat4 localRotation)
{
	this->localRotation = localRotation;
	updateModel();
}

void MatrixCollection::setLocalScale(glm::mat4 localScale)
{
	this->localScale = localScale;
	updateModel();
}
