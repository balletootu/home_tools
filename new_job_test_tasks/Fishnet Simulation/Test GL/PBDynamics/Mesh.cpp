#include "StdAfx.h"
#include "Mesh.h"

namespace koval
{
namespace PBDynamics
{

Mesh::Mesh(int horizonlaNumPoints/* = 15*/, int verticalNumPoints/* = 15*/)
{
	createMesh(horizonlaNumPoints, verticalNumPoints);
}

Mesh::~Mesh()
{
	for (TMeshNodes::iterator itr = meshNodes_.begin(); itr != meshNodes_.end(); ++itr)
	{
		delete *itr;
	}

	for (TDistanceConstraint::iterator itr = fixedConstraints_.begin(); itr != fixedConstraints_.end(); ++itr)
	{
		delete *itr;
	}
}

void Mesh::createMesh(int horizonlaNumPoints, int verticalNumPoints)
{

	float xStep = 1.0f / horizonlaNumPoints;
	float yStep = 1.0f / verticalNumPoints;
	float curXPos(-0.5f), curYPos(0.5f);

	MeshNode* pNode = NULL;
	//create nodes
	for (int i = 0; i < verticalNumPoints; ++i)
	{
		curXPos = -0.5f;
		for (int j = 0; j < horizonlaNumPoints; ++j)
		{
			pNode = new MeshNode(Vector2D(curXPos, curYPos));
			meshNodes_.push_back(pNode);
			curXPos += xStep;
		}
		curYPos -= yStep;
	}
	meshNodes_[0]->setDefaultFixed();
	meshNodes_[horizonlaNumPoints-1]->setDefaultFixed();

	DistanceConstraint* pDC;
	//create Horizontal Consyreints
	for (int i = 0; i < verticalNumPoints; ++i)
		for (int j = 0; j < horizonlaNumPoints-1; ++j)
		{
			pDC = new DistanceConstraint(meshNodes_[i*horizonlaNumPoints + j], meshNodes_[i*horizonlaNumPoints + j +1], 0.5f);
			fixedConstraints_.push_back(pDC);
		}

	//create Vertical Constraints
	for (int i = 0; i < horizonlaNumPoints; ++i)
		for (int j = 0; j < verticalNumPoints - 1; ++j)
		{
			pDC = new DistanceConstraint(meshNodes_[i + j*horizonlaNumPoints], meshNodes_[i + (j+1)*horizonlaNumPoints], 0.5f);
			fixedConstraints_.push_back(pDC);
		}
}

void Mesh::reset()
{
	for (TMeshNodes::iterator itr = meshNodes_.begin(); itr != meshNodes_.end(); ++itr)
	{
		(*itr)->reset();
	}

	for (TDistanceConstraint::iterator itr = fixedConstraints_.begin(); itr != fixedConstraints_.end(); ++itr)
	{
		(*itr)->reset();
	}

}

void Mesh::update(float elapsedTime)
{
	for (TMeshNodes::iterator itr = meshNodes_.begin(); itr != meshNodes_.end(); ++itr)
	{
		(*itr)->updateVelocity(elapsedTime);
		(*itr)->calcNewPosition(elapsedTime);
	}

	for (TDistanceConstraint::iterator itr = fixedConstraints_.begin(); itr != fixedConstraints_.end(); ++itr)
	{
		(*itr)->checkInverseCollisions();
	}

	for (TMeshNodes::iterator itr = meshNodes_.begin(); itr != meshNodes_.end(); ++itr)
	{
		(*itr)->updatePositions(elapsedTime);
	}
}

size_t Mesh::captureNode(Vector2D point)
{
	float distMin = point.distance(meshNodes_[0]->getNewPosition());
	float newDist(0.0f);
	size_t index(0), indexMin(0);

	for (TMeshNodes::iterator itr = meshNodes_.begin(); itr != meshNodes_.end(); ++itr, ++index)
	{
		newDist = point.distance((*itr)->getNewPosition());
		if (newDist < distMin)
		{
			distMin = newDist;
			indexMin = index;
		}
	}

	meshNodes_[indexMin]->setFixed();
	return indexMin;
}

bool Mesh::releaseNode(size_t index)
{
	if ((index < 0) || (index > meshNodes_.size()))
		return false;

	meshNodes_[index]->setFixed(false) ;
	return true;
}

bool Mesh::updateNodePosition(size_t index, Vector2D newPos)
{
	if ((index < 0) || (index > meshNodes_.size()))
		return false;

	meshNodes_[index]->setPosition(newPos);

	return true;
}

Mesh::TMeshNodes::const_iterator Mesh::nodesItrBegin() const
{
	return meshNodes_.begin();
}

Mesh::TMeshNodes::const_iterator Mesh::nodesItrEnd() const
{
	return meshNodes_.end();
}

Mesh::TDistanceConstraint::const_iterator Mesh::constraintsItrBegin() const
{
	return fixedConstraints_.begin();
}

Mesh::TDistanceConstraint::const_iterator Mesh::constraintsItrEnd() const
{
	return fixedConstraints_.end();
}

} //namespace PBDynamics
} //namespace koval
