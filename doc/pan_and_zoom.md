# pan_and_zoom

pan and zoom is the basic operation for edit 2d or 3d object.


## 获取用户输入

按键，包括鼠标按钮和键盘。

鼠标滚轮。

按键的接口，是轮询方式的，绑定窗口。glfwGetKey 返回是 GLFW_RELEASE 和 GLFW_PRESS 两种状态。如果需要获取 (down|press -> press -> up|release -> release) 的 down 和 up 事件，需要自行维护下上帧的所有按键状态。因为按键数量有效，维护的花费很小。

鼠标滚轮的接口，是回调的方式，也绑定窗口。


## pan

改变相机的 Transform

ui_helper.cpp  ProcessPan()


## zoom

改变 PlotRoot 的 Transform

ui_helper.cpp  ZoomPlotRoot()
