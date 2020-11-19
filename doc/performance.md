# Performance


一些性能优化的备忘


## 缓存 Transform，而非每次都 GetComponent

<img src="image/performance_1.png" />


## 给 Vector 预留足够的 Capacity

<img src="image/performance_2.png" />
<img src="image/performance_3.png" />

<img src="image/performance_4.png" />
<img src="image/performance_5.png" />

耗时百分比，14.7% -> 7.09%


## 把 map 改为 unorder_map 

<img src="image/performance_6.png" />
<img src="image/performance_7.png" />

优化后

<img src="image/performance_8.png" />

整体的操作流畅度提升很明显，但是从耗时百分比来看，这几个函数似乎没有减少很多。

推测是因为 map 的操作是分散的，不仅仅是这几个地方在调用很多次，其他地方也受益了，耗时同样减少了。所以统计后的耗时对比不明显。
