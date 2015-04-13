#pragma once

namespace koval
{
namespace PBDynamics
{

class Vector2D
{
public:
	Vector2D();
	Vector2D(const Vector2D& val);
	Vector2D(float valX, float valY);
	~Vector2D(){};

	Vector2D& operator= (const Vector2D& rhs);
	Vector2D& operator+= (const Vector2D& rhs);
	Vector2D operator+ (const Vector2D& rhs) const;
	Vector2D& operator-= (const Vector2D& rhs);
	Vector2D operator- (const Vector2D& rhs) const;
	Vector2D& operator*= (float rhs);
	Vector2D operator* (float rhs) const;

	float length() const;
	float distance(const Vector2D& rhs) const;
	Vector2D normalVec() const;
	Vector2D invert() const;

public:
	float x;
	float y;
};

} //namespace PBDynamics
} //namespace koval


