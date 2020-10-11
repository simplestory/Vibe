## Vibe算法C++后端

Vibe背景建模算法是一个像素级的视频建模传统方法。该算法发表于09年的IEEE会议，但其内存占用小、计算量小的特性很适合嵌入式端部署，并且有着优于混合高斯模型的性能。在参考了各种开源代码后，发现python代码的执行效率过低，并不能满足需求。为了与项目后续使用的算法进行对接，这里用C++实现了Vibe算法并利用pybind11进行封装生成python可直接调用的库文件。论文作者其实已经开源了相关C++代码，并且在实现上有几处优化。

[Telecommunications and Imaging Laboratory](http://www.telecom.ulg.ac.be/research/vibe/)（Vibe官网）

[ViBE: A powerful random technique to estimate the background in video sequences](https://ieeexplore.ieee.org/abstract/document/4959741)


### Compile it

依次执行以下命令：

```
mkdir build && cd build
cmake ..
make
```

运行结束后在`build`文件下会生成`VIBE.xxx.so`文件。该文件所在目录下即可用python直接import相应库，也可以通过`sys.path`实现在其他目录下访问该文件。

```
# Put VIBE.xxx.so in ./lib

>>> import sys
>>> sys.path.append("./lib")
>>> from VIBE import VIBE
```

可执行测试文件检测库文件是否正确生成：

```
# before this, you should put the VIBE.xxx.so in ./lib

python test.py
```

若生成背景提取后的视频则库文件生成正确。


### Usage

**在导入库后需要进行类的初始化**

```
from VIBE import VIBE

vibe = VIBE()

vibe.sampleNums = 20 # 各像素模型数
vibe.miniMatch = 6 # 最小背景模型匹配数
vibe.radis = 20  # 背景模型最大偏差
vibe.rateNum = 16  # 背景更新概率种子(1/rateNum)

vibe.Rgb2Gray(numpy.array())  # 输入三维数组，将RGB图像转为灰度图像
vibe.Initial(numpy.array())  # 输入三维数组，使用灰度图像对背景模型进行初始化

# 输入两个三维数组，利用输入的灰度图像更新模型并将结果写入输入的三维数组中
vibe.Update(numpy.array(), numpy.array())

vibe.Vibe(numpy.array())  # 输入三维数组，使用RGB图像运算Vibe算法，为Initial和Update的集成函数
```

具体可参考测试代码`test.py`

实际效果如下：

![res.gif](https://raw.githubusercontent.com/simplestory/Vibe/main/gifs/test.gif)

从上面效果看，该算法并不能很好地消除鬼影，这部分需要再进行调整。算法大致运行时间为每帧120ms，这里并没有使用作者实现中提到的优化方法，还有较大的优化空间。

### TODO

- [ ] 快速消除Ghost
- [ ] 优化C++代码，加快运行速度