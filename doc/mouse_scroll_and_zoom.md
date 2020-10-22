# mouse_scroll_and_zoom


鼠标滚轮事件和视野缩放


## Plot 2D

初始的想法，有两种：

1. 改变相机的 fov 和 aspect ratio

2. 创建名为 PlotRoot 的根节点，改变根节点 Transform 的 scale。而相机的参数不变。


刚开始，采用了方案一。缩放实现比较容易，但是对于后续实现其他需求时，造成了阻碍。
{
    1. 需求是，对于正方形，已知宽，计算出高使得当相机与正方形的平面正交时，其在屏幕显示是宽高相等的。
    {
        - 如果改变相机的 fov 和 aspect ratio，即通过改变屏幕空间 1 pixel 映射到世界空间的尺寸 pixel_size_in_world，并且 pixel_size_in_world 的宽高不是等比例的。
        - 在这种情况下计算出高是比较困难的，需要数学推算，怎么计算，暂时没有想出方案。
        - 如果改变根节点的 Transform，计算出高变得简单了。
    }
    2. 非等比 pixel_size_in_world 对于未来 3D 的 Mesh 顶点计算也可能造成阻碍。
}


早先的实现是采用了想法一，绕了几天，现在决定改用想法二。所以，这里备忘下。

想法一的实现，参见 ui_helper.cpp ChangeFovAndAspect()
