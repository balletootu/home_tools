#include "StdAfx.h"
#include "DistanceConstraint.h"
#include "Vectors.h"
#include "MeshNode.h"
#include "PBDConst.h"

namespace koval
{
namespace PBDynamics
{

DistanceConstraint::DistanceConstraint(MeshNode* node1, MeshNode* node2_, float stiffness)
: node1_(node1)
, node2_(node2_)
, stiffness_(DEFAULT_STIFFNESS)
{
	if ((node1_ != NULL) && (node2_ != NULL))
	{
		Vector2D p1 = node1_->getNewPosition();
		Vector2D p2 = node2_->getNewPosition();
		defDistance_ = distance_ = p1.distance(p2);
	}
}

DistanceConstraint::~DistanceConstraint(void)
{
	node1_ = NULL;
	node2_ = NULL;
}

void DistanceConstraint::reset()
{
	distance_ = defDistance_;
}

void DistanceConstraint::checkInverseCollisions()
{
	if ((NULL == node1_) || (NULL == node2_))
		return;

	if (node1_->isFixed() && node2_->isFixed())
		return;

	float w1 = node1_->getW();
	float w2 = node2_->getW();

	Vector2D p1 = node1_->getNewPosition();
	Vector2D p2 = node2_->getNewPosition();
	Vector2D direction = p1 - p2;
	float distance = direction.length();
	direction = direction.normalVec();
	float t1 = w1 / (w1 + w2) * (distance - distance_) * stiffness_;
	float t2 = w2 / (w1 + w2) * (distance - distance_) * stiffness_;

	Vector2D dp1 = direction.invert() * t1;
	Vector2D dp2 = direction * t2;

	node1_->setNewPosition(p1 + dp1);
	node2_->setNewPosition(p2 + dp2);
}

Vector2D DistanceConstraint::getBeginPoint() const
{
	return node1_->getNewPosition();
}

Vector2D DistanceConstraint::getEndPoint() const
{
	return node2_->getNewPosition();
}

} //namespace PBDynamics
} //namespace koval