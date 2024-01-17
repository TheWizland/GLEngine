#include "MatrixCollection.h"
#include <GLM/gtc/matrix_transform.hpp>
#include <GLM/gtc/matrix_inverse.hpp>
#include <algorithm>
#include <vector>

void MatrixCollection::updateModel()
{
	worldTranslation = glm::mat4(1.f);
	worldRotation = glm::mat4(1.f);
	worldScale = glm::mat4(1.f);
	glm::vec4 location = glm::vec4(0.f, 0.f, 0.f, 1.f);
	location = localTranslation * location;
	//Do not use *=, ordering of matrix operations is important.
	//a *= b will do a * b, when we want b * a
	//You will get compiler vomit if you try *= here

	if (parentMatrix) 
	{
		if (inheritTranslation) 
		{
			if (applyParentRotationToPosition) 
			{
				location = parentMatrix->getWorldRotation() * location;
			}
			if (applyParentScaleToPosition) 
			{
				location = parentMatrix->getWorldScale() * location;
			}
			location = parentMatrix->getWorldTranslation() * location;
		}

		if (inheritRotation) 
		{
			worldRotation = parentMatrix->getWorldRotation() * worldRotation;
		}
		if (inheritScale) 
		{
			worldScale = parentMatrix->getWorldScale() * worldScale;
		}
	}

	worldTranslation = glm::translate(worldTranslation, glm::vec3(location));
	worldRotation = localRotation * worldRotation;
	worldScale = localScale * worldScale;

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
	children.erase(std::remove(children.begin(), children.end(), childMatrix));
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

void MatrixCollection::translate(glm::vec3 distance)
{
	setLocalTranslation(glm::translate(localTranslation, distance));
}

void MatrixCollection::translate(float x, float y, float z)
{
	setLocalTranslation(glm::translate(localTranslation, glm::vec3(x, y, z)));
}

void MatrixCollection::rotate(float distance, glm::vec3 angle)
{
	setLocalRotation(glm::rotate(localRotation, distance, angle));
}

void MatrixCollection::rotateX(float distance)
{
	setLocalRotation(glm::rotate(localRotation, distance, glm::vec3(1, 0, 0)));
}

void MatrixCollection::rotateY(float distance)
{
	setLocalRotation(glm::rotate(localRotation, distance, glm::vec3(0, 1, 0)));
}

void MatrixCollection::rotateZ(float distance)
{
	setLocalRotation(glm::rotate(localRotation, distance, glm::vec3(0, 0, 1)));
}

void MatrixCollection::scale(float amount)
{
	setLocalScale(glm::scale(localScale, glm::vec3(amount, amount, amount)));
}

void MatrixCollection::scale(float x, float y, float z)
{
	setLocalScale(glm::scale(localScale, glm::vec3(x, y, z)));
}
