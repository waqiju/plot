# plot_panel

plot_root
|--- main_panel
|--- float_panel_1
|--- float_panel_2
|--- ...

main_panel = float_panel_0
|--- space_grid


## 浮动面板

1. 固定在屏幕的指定区域，或者和主面板重叠

2. x 轴与主面板同步

3. y 轴是独立的


## 多 Panel 和平移缩放操作

平移缩放，操作的是相机。多 Panel 是 PlotRoot 下的不同结点。这样就可以实现不同 Panel 使用不同的缩放比例。


## Panel 之间的排布

panel 有两种类型： 

1. OverlayMainPanel

2. Region

OverlayMainPanel 是叠加在 main panel 之上，所以实际排布是 main_panel 和 floating_panel。

按照约定，floating 应该是自下至上的。

```
===================

    > main panel <

-------------------
    > floating 2 <
-------------------
    > floating 1 <
===================
```

留白约定：

1. floating panel 高度为 0.2 个屏幕。0.2 个屏幕的从下到上是： 0.05 的留白区和 0.15 的内容区。

2. floating panel 的内容区，上下各留 10% 的空白。

2. main panel 的上下两端各留 0.05 个屏幕的空白。
