#pragma once
#include <vector>

#include "Vectors.h"
#include "MeshNode.h"
#include "DistanceConstraint.h"

namespace koval
{
namespace PBDynamics
{

class Mesh
{
public:
	typedef std::vector<MeshNode*> TMeshNodes;
	typedef std::vector<DistanceConstraint*> TDistanceConstraint;

public:
	Mesh(int horizonlaNumPoints = 15, int verticalNumPoints = 15);
	~Mesh();

	void reset();
	void update(float elapsedTime);

	size_t captureNode(Vector2D point);
	bool releaseNode(size_t index);
	bool updateNodePosition(size_t index, Vector2D newPos);

	TMeshNodes::const_iterator nodesItrBegin() const;
	TMeshNodes::const_iterator nodesItrEnd() const;

	TDistanceConstraint::const_iterator constraintsItrBegin() const;
	TDistanceConstraint::const_iterator constraintsItrEnd() const;

private:
	void createMesh(int horizonlaNumPoints, int verticalNumPoints);

private:
	TDistanceConstraint fixedConstraints_;
	TMeshNodes meshNodes_;
};

} //namespace PBDynamics
} //namespace koval