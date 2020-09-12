#include "vector3.h"
#include <cmath>
#include <cassert>


Vector3 Vector3::zero = Vector3(0, 0, 0);
Vector3 Vector3::one = Vector3(1, 1, 1);


float Vector3::Magnitude()
{
	return sqrt(x * x + y * y + z * z);
}

void Vector3::Normalize()
{
	float l = Magnitude();
	if (l != 0)
	{
		x /= l;
		y /= l;
		z /= l;
	}
}


float& Vector3::operator[](size_t i)
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
    }
}
