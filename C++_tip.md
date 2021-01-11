# C++ tips

## C++ std::move()

在C++11中，标准库在<utility>中提供了一个有用的函数std::move，std::move并不能移动任何东西，它唯一的功能是将一个左值强制转化为右值引用，继而可以通过右值引用使用该值，以用于移动语义。从实现上讲，std::move基本等同于一个类型转换：static_cast<T&&>(lvalue);

std::move函数可以以非常简单的方式将左值引用转换为右值引用。(左值 右值 引用 左值引用)概念 https://blog.csdn.net/p942005405/article/details/84644101

1. C++ 标准库使用比如vector::push_back 等这类函数时,会对参数的对象进行复制,连数据也会复制.这就会造成对象内存的额外创建, 本来原意是想把参数push_back进去就行了,通过std::move，可以避免不必要的拷贝操作。
2. std::move是将对象的状态或者所有权从一个对象转移到另一个对象，只是转移，没有内存的搬迁或者内存拷贝所以可以提高利用效率,改善性能.。
3. 对指针类型的标准库对象并不需要这么做.

## 数学计算函数

```c++
 double log10()
 double fmod(double a,double b) 计算浮点数的余数，就是看小数位后面的数字，可以用来判断整数
```

int rand() 生成随机数

## accumulate

求和函数，做算法题的时候使用比较方便

```c++
int sum = accumulate(vec.begin() , vec.end() , 42);  第三个参数就是多加的东西
string sum = accumulate(v.begin() , v.end() , string(" "));  
```

## 二分查找的函数

```c++
#include <algorithm>
```

二分查找的函数有 3 个：

#### lower_bound

(起始地址，结束地址，要查找的数值) 返回的是数值 **第一个** 出现的位置。

功能：函数lower_bound()在first和last中的前闭后开区间进行二分查找，返回**大于或等于val的第一个元素位置**。如果所有元素都小于val，则返回last的位置.

注意：如果所有元素都小于val，则返回last的位置，且last的位置是**越界**的！！

#### upper_bound

(起始地址，结束地址，要查找的数值) 返回的是 第一个大于待查找数值 出现的位置。

功能：函数upper_bound()返回的在前闭后开区间查找的关键字的上界，返回**大于val**的第一个元素位置

注意：返回查找元素的最后一个可安插位置，也就是“元素值>查找值”的第一个元素的位置。同样，如果val大于数组中全部元素，返回的是last。(注意：数组下标越界)

#### binary_search

(起始地址，结束地址，要查找的数值)  返回的是是否存在这么一个数，是一个**bool值**。

注意：使用二分查找的前提是数组有序

使用的时候要小心，但可以看出lower和upper的区分就是判断条件>=还是>

## 求前缀和的方便函数 partial_sum

```c++
#include <numeric> 头文件
template <class InputIterator, class OutputIterator, class BinaryOperation>
OutputIterator partial_sum (InputIterator first, 
                            InputIterator last,
                            OutputIterator result, 
                            BinaryOperation binary_op);
```

**partial_sum 对于序列 a,b,c,d 产生序列 a,a+b,a+b+c,a+b+c+d。**

output的容器一定要足够大才行，也可以用input的first

## C++ 位操作

异或 相同false 不同true

与，或正常

两个数字按照二进制，对每一位进行比较

位运算是指按二进制进行的运算。在程序中，常常需要处理二进制位的问题。C/C++语言提供了6个位操作运算符。这些运算符只能用于整型操作数，即只能用于带符号或无符号的char,short,int与long类型。

在实际应用中，建议用unsigned整型操作数，因为带符号操作数可能因为不同机器结果不同。

x ^ 0s = x             x & 0s = 0           x | 0s = x
x ^ 1s = ~x           x & 1s = x           x | 1s = 1s
x ^ x = 0               x & x = x             x | x = x  

返回的值不止0，1。比较所有位之后得到结果

位操作，左移<<，右移>> 对变量进行位操作，并不会改变这个变量的值，只有通过赋值才能得到移动后的数值。

```c++
a>>2 a不变
a>>=2 a才是变了
```

