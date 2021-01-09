#ifndef _MY_GEOM_H
#define _MY_GEOM_H

#include <math.h>

class IPoint2 {
public:
	IPoint2() {}
	~IPoint2() {}
	
	IPoint2(int ix, int iy) { x = ix; y = iy; }
	
	int x;
	int y;
};

class FPoint3 {
public:
	FPoint3() {}
	~FPoint3() {}

	FPoint3(float ix, float iy, float iz) { x = ix; y = iy; z = iz; }

	float dot(const FPoint3 &v) const { return x*v.x+y*v.y+z*v.z; }
	FPoint3 cross(const FPoint3 &v) const
	{ return FPoint3(y*v.z - z*v.y, z*v.x - x*v.z, x*v.y - y*v.x); }
	float length() const { return sqrt(x*x+y*y+z*z); }
	void normalize() 
	{ double s = sqrt(x*x+y*y+z*z); x /= s; y /= s; z /= s; }

	// assignment
	FPoint3 &operator=(const FPoint3 &v) { x = v.x; y = v.y; z = v.z; return *this; }

	// operators
	FPoint3 operator +(const FPoint3 &v) const { return FPoint3(x+v.x, y+v.y, z+v.z); }
	FPoint3 operator -(const FPoint3 &v) const { return FPoint3(x-v.x, y-v.y, z-v.z); }
	FPoint3 operator *(float s) const { return FPoint3(x*s, y*s, z*s); }
	FPoint3 operator *(double s) const { return FPoint3((float)(x*s), (float)(y*s), (float)(z*s)); }
	FPoint3 operator /(float s) const { return FPoint3(x/s, y/s, z/s); }
	FPoint3 operator -() { return FPoint3(-x, -y, -z); }
	bool operator==(const FPoint3 &v2) const
	{ return (x == v2.x && y == v2.y && z == v2.z); }
	bool operator!=(const FPoint3 &v2) const
	{ return (x != v2.x || y != v2.y || z != v2.z); }

	void operator +=(const FPoint3 &v) { x+=v.x; y+=v.y; z+=v.z; }
	void operator -=(const FPoint3 &v) { x-=v.x; y-=v.y; z-=v.z; }
	void operator *=(float s) { x*=s; y*=s; z*=s; }
	void operator /=(float s) { x/=s; y/=s; z/=s; }
	FPoint3 operator -() const { return FPoint3 (-x, -y, -z); }

	float x;
	float y;
	float z;
};

#endif