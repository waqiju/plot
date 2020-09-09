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