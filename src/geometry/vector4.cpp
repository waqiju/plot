#include "vector4.h"
#include <cmath>
#include <cassert>


Vector4 Vector4::zero = Vector4(0, 0, 0, 0);
Vector4 Vector4::one = Vector4(1, 1, 1, 1);


float Vector4::Magnitude()
{
	return sqrt(x * x + y * y + z * z + w * w);
}

void Vector4::Normalize()
{
	float l = Magnitude();
	if (l != 0)
	{
		x /= l;
		y /= l;
		z /= l;
        w /= l;
	}
}

float& Vector4::operator[](size_t i)
{
	assert(i >= 0 && i < Length());
	switch (i)
	{
	case 0:
		return x;
	case 1:
		return y;
	case 2:
		return z;
	case 3:
		return w;
	}
}
