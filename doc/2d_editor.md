# 2d_editor

如何实现 2D 编辑器

## 拆解

可视化的 2D 空间视图，包括：
1. 网格和坐标轴
2. 刻度 Label
3. 网格和刻度的多级 Tick 渐变


## 多级 Tick 渐变

采用模拟 Unity 的实现。

Ticks 数组, 1000 -> 5e-6 (0.000005)。
遍历 Ticks 数组，找到最小的世界空间点 v0(tick_x, tick_y, 0) 经过变换后到屏幕空间后 v1，v1.x >= pixel && v1.y >= pixel。

设置三级 Tick, 分别是 tick_x, tick_x - 1, tick_x - 2。

tick_x 的网格线 alpha 设置为 0.05
tick_x - 1, 0.15
tick_x - 2, 0.4
zero, 0.75

刻度 Label 的多级渐变也是类似的处理，比如，只在 tick_x - 1 显示 Label。
