# 第一次作业

1.几何变换编程练习：以长方体表示机器人手的手掌和指节，变换各个手指的指节形成“握拳”状态和“松开”状态。

描述：主要通过glPushMatrix()和glPopMatrix()操作便捷地逐元素画出机器人的手，通过键盘的回调函数改变机器人手各元素绘制时的旋转角实现手指的松握，通过特殊键的回调函数改变绘制前的一组平移旋转的参数实现手的缩放旋转。

2.以简单球体表示太阳、地球和月亮，模拟太阳系的运转

描述：通过旋转和平移操作依次画出太阳、地球和月亮，每次旋转的旋转角等于时间乘以角速度，平移的距离是轨道半径，通过空闲回调函数更新时间来实现太阳系的运转。

# 第二次作业

1.分别以平坦着色和光滑着色模式绘制一个简单球体（以三角形网络或四边形网络逼近表示），以一个位置或方向动态变化的光源来照射它，表现明暗动态变化效果。

描述：球的线框模型是从一个正四面体开始通过递归细分得到的。平坦着色和光滑着色主要是在绘制每个三角形时通过glNormal3fv()设置当前法方向实现的。光源的移动是通过改变显示回调函数中光源绘制时的旋转角实现的。

2.以简单球体表示地球，让它自转，为其表面映射纹理。

描述：首先创建一个二次曲面，启用纹理并为其绑定纹理，再从这个曲面绘制一个球体，最后删除这个二次曲面对象。自转是通过空闲回调函数改变旋转角实现的，同时在键盘回调函数中通过相似的方法实现对地球的旋转操作。

# 第三次作业

1.解析PLY格式的三维模型文件，并以显示列表的形式将模型绘制出来，加上适当的光照。在绘制窗口中加入鼠标交互功能，鼠标能够将模型旋转和移动。

描述：Ply文件通过自己实现的Loader读成相应的数组并返回指针，然后通过glNormalPointer()和glVertexPointer()使用。绘制通过创建和调用显示列表加速了绘制。鼠标交互通过鼠标回调函数和鼠标移动回调函数改变，左键拖动控制模型旋转，中键控制平移，右键控制缩放，分别改变的是显示函数中绘制所有元素前的缩放平移旋转的操作参数。
