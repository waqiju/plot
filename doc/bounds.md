# Bounds

## 成员数据

(center, size)
(min, max)
extents = size / 2


## 成员方法

位置关系

Constains()  (point) -> bool, 和点
IntersectRay  (ray) -> (bool, distance), 和射线
Intersects()  (bounds) -> bool, 和立方体

运算

Encapsulate()
{
    (point) -> void
    (bounds) -> void
}
