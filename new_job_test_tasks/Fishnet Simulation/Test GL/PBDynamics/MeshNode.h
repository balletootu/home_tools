#pragma once

#include "Vectors.h"

namespace koval
{
namespace PBDynamics
{
	
class MeshNode
{
public:
	MeshNode(Vector2D pos);
	~MeshNode();

	void reset();
	void updateVelocity(float elapsedTime);
	void calcNewPosition(float elapsedTime);
	void updatePositions(float elapsedTime);

	void setFixed(bool state = true);
	void setDefaultFixed(bool state = true);
	bool isFixed();
	float getW();
	void setNewPosition(const Vector2D& pos);
	void setPosition(const Vector2D& pos);
	Vector2D getNewPosition();

private:
	Vector2D	position_;
	Vector2D	newPosition_;
	Vector2D	defaultPosition_;
	Vector2D	velocity_;
	bool		fixed_;
	int			defaultFixed_;
	float		inverseMass_;
	Vector2D	pressure_;
};

} //namespace PBDynamics
} //namespace koval