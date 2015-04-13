#pragma once

namespace koval
{
namespace PBDynamics
{

class MeshNode;
class Vector2D;

class DistanceConstraint
{
public:
	DistanceConstraint(MeshNode* node1, MeshNode* node2_, float stiffness);
	~DistanceConstraint();

	void reset();
	void checkInverseCollisions();

	Vector2D getBeginPoint() const;
	Vector2D getEndPoint() const;

private:
	MeshNode*	node1_;
	MeshNode*	node2_;
	float		distance_;
	float		defDistance_;
	float		stiffness_;
};

} //namespace PBDynamics
} //namespace koval