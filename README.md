# krnlib(kernel library)
- 因为c++中的很多库, 都会涉及到内存分配等, 而用户层和驱动层分配的方式不一样, 所以在驱动中直接使用c++的部分库会导致报错. 这个项目就是解决这些而生
- 必须使用C++17及以上版本
## 内容
1. 一些常用的c++库的驱动版(包括stl容器, function, initializer_list......)
2. 一些驱动中适用的方便的库
3. 异常处理
## ->[API文档](https://github.com/fuyouawa/krnlib/wiki/document)
