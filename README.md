# DriverCompatibilityUtils(驱动兼容工具)
因为c++中的很多库, 都会涉及到内存分配等, 而用户层和驱动层分配的方式不一样, 所以在驱动中直接使用c++的部分库会导致报错.
这个项目的目的就是提供一些常用的c++库的驱动版.
(STL的我没实现, 因为有其他大佬实现过了, 这里推荐一个https://github.com/jxy-s/stlkrn)

## 目前实现的有
1. std::function
2. std::initializer_list
3. 待补充...
