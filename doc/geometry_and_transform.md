# Geometry 和 Transform


## Unity 的 Transform 结构

Transform
Vector3
Quaternion
Matrix4x4


## Transform

TRS
position / localPosition
rotation / localRotation
lossyScale / localScale

方向和矩阵, forward, up, right, worldToLocalMatrix, localToWorldMatrix

Hierarchy 树状层级
parent
GetSiblingIndex()
SetSiblingIndex()

childCount
Find()
GetChild()

空间变换函数
Translate()
{
    (translation) -> ()
    (translation, relate_to) -> ()
    (translation, space) -> ()
}
Rotate()
RotateAround()
LookAt()


## Vector3

x, y, z
常量, zero, one, forward, back, left, right, up, down
运算
常用方法, normalized, magnitude, Normalize() 


## Quaternion

x, y, z, w
常量, identity
运算
构造函数, Euler()  
{
    (vector3) -> ()
    (x, y, z) -> ()
}


## Matrix4x4

m [16] -> float
常量, zero, identity
构造函数
{
    Perpective, Ortho, Frustum
    Translate, Rotate, Scale, TRS
    LookAt
    Inverse, Transpose
}
运算
{
    MultiplyPoint()
    MultiplyPoint3x4()
    MultiplyVector()
}


## glm 中的 vector, matrix, quaternion 实现


## vector3

参考 type_vec3.hpp, type_vec3.inl

字段
x, y, z

方法
1. 重载下标索引操作符。实现 switch ... case ...
2. swizzle, 宏生成代码来实现。
3. 字段别名, (x, y, z), (r, g, b), (s, t, p)。 用 union 来实现
4. 操作符重载
```
{
    常规
    {
        +-, 标量 scalar, 矢量 vector
        */, 标量 scalar
    }
    不常规
    {
        */, 矢量。 这是对应坐标相乘，数学上好像是没有这个运算定义的。
    }
}
```
5. length()


## Matrix4x4

参考 type_mat4x4.hpp type_mat4x4.inl

字段
value [4] -> vector4

方法
1. 重载下标索引操作符。matrix4x4 -> vector4 -> float，正好满足两次索引嵌套。
2. 操作符重载
```
{
    常规
    {
        +-, 标量
        * 标量
        * 矢量 vector3, vector4
        * 矩阵 matrix4x4
        / 
    }
}
```

## quaternion

和vector4类似，不展开了。


## assimp 中的 matrix4x4 实现

matrix4x4.h

Matrix4x4::Rotate()


Matrix4x4::TRS()  代码实现，参考 matrix4x4.inl