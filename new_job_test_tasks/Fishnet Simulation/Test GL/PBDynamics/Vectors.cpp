#include "StdAfx.h"
#include "Vectors.h"
#include "math.h"

namespace koval
{
namespace PBDynamics
{

Vector2D::Vector2D()
: x(0)
, y(0)
{
}

Vector2D::Vector2D(const Vector2D& val)
: x (val.x)
, y (val.y)
{
}

Vector2D::Vector2D(float valX, float valY)
: x(valX)
, y(valY)
{
}

Vector2D& Vector2D::operator= (const Vector2D& rhs)
{
	x = rhs.x; 
	y = rhs.y; 
	return *this;
}

Vector2D& Vector2D::operator+= (const Vector2D& rhs) 
{
	x += rhs.x; 
	y += rhs.y; 
	return *this;
}

Vector2D Vector2D::operator+ (const Vector2D& rhs) const 
{
	Vector2D val = (*this); 
	return val += rhs;
}

Vector2D& Vector2D::operator-= (const Vector2D& rhs) 
{
	x -= rhs.x; 
	y -= rhs.y; 
	return *this;
}

Vector2D Vector2D::operator- (const Vector2D& rhs) const 
{
	Vector2D val = (*this); 
	return val -= rhs;
}

Vector2D& Vector2D::operator*= (float rhs) 
{
	x *= rhs; 
	y *= rhs; 
	return *this;
}

Vector2D Vector2D::operator* (float rhs) const
{
	Vector2D val = (*this); 
	return val *= rhs;
}

float Vector2D::length() const
{
	return sqrt(x*x + y*y);
}

float Vector2D::distance(const Vector2D& rhs) const
{
	Vector2D val = (*this);
	val -= rhs;
	return val.length();
}

Vector2D Vector2D::normalVec() const
{
	Vector2D val = (*this);
	float len = val.length();
	if (0.0f != len)
	{
		val.x /= len;
		val.y /=len;
	}

	return val;
}

Vector2D Vector2D::invert() const
{
	return Vector2D(-x, -y);
}

} //namespace PBDynamics
} //namespace koval
