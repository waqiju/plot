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
