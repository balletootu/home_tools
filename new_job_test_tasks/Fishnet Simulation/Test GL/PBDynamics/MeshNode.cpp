#include "StdAfx.h"
#include "MeshNode.h"
#include "PBDConst.h"

namespace koval
{
namespace PBDynamics
{

MeshNode::MeshNode(Vector2D pos)
: position_(pos)
, newPosition_(pos)
, defaultPosition_(pos)
, velocity_(Vector2D())
, fixed_(false)
, defaultFixed_(false)
, inverseMass_(DEFAULT_MASS)
, pressure_(Vector2D())
{
}

MeshNode::~MeshNode(void)
{
}

void MeshNode::reset()
{
	newPosition_ = defaultPosition_;
	position_ = defaultPosition_;
	velocity_ = Vector2D();
	pressure_ = Vector2D();
	fixed_ = false;
	defaultFixed_ = false;
}

void MeshNode::updateVelocity(float elapsedTime) 
{
	velocity_ += Vector2D(0, GRAVITY_FORCE * elapsedTime * inverseMass_);
}

void MeshNode::calcNewPosition(float elapsedTime) 
{
	if (!fixed_) 
	{
		newPosition_ = position_ + velocity_ * elapsedTime;
	} 
}

void MeshNode::setFixed(bool state /*= true*/)
{ 
	if(!state && !defaultFixed_)
	{
		fixed_ = state;
	}
}

void MeshNode::setDefaultFixed(bool state /*= true*/) 
{
	fixed_ = defaultFixed_ = state;
}

bool MeshNode::isFixed()
{
	return fixed_;
}

float MeshNode::getW() 
{
	return fixed_ ? 0 : inverseMass_; 
}

void MeshNode::setNewPosition(const Vector2D& pos)
{
	newPosition_ = pos;
}

void MeshNode::setPosition(const Vector2D& pos)
{
	newPosition_ = pos;
	position_ = pos;
}

Vector2D MeshNode::getNewPosition()
{
	return newPosition_;
}

void MeshNode::updatePositions(float elapsedTime)
{
	velocity_ = (newPosition_ - position_) * (1/elapsedTime);
	float d = velocity_.length();

	if (d > MAX_VELOCITY)
	{
		velocity_ = velocity_.normalVec() * MAX_VELOCITY;
	}

	position_ = newPosition_;
}

} //namespace PBDynamics
} //namespace koval